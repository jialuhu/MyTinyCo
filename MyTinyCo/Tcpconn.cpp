//
// Created by Jialu  Hu on 2019-08-04.
//
#include "Tcpconn.h"
#include <sys/stat.h>
#include <sys/mman.h>
using namespace SiNet;
TcpConnection::TcpConnection(EventLoop *loop,
        std::string conn_name, int connfd, InetAddr &local):
        loop_(loop),
        conn_name_(conn_name),
        connfd_(connfd),
        socket_(new Socket(connfd,true)),
        LocalAddr_(local),
        conn_state(CONNECTING),
        channel_(new Channel(loop,connfd)){
    channel_->setReadCallback([this]{this->HandleRead();});
    channel_->setWriteCallback([this]{this->HandleWrite();});
    channel_->setErnnoCallback([this]{this->HandleErrno();});
    channel_->setCloseCallback([this]{this->HandleClose();});
    channel_->updateChannle();
}
TcpConnection::~TcpConnection() {
}
void TcpConnection::connectEstablished() {
    assert(conn_state==CONNECTING);
    conn_state = CONNCTED;
    channel_->enableReading();
    //设置用户回调,进行事件注册
    if(connectionCb_)
        connectionCb_(shared_from_this());
}

void TcpConnection::HandleRead(){
    //此处用Buffer读取出接收到的数据，然后交给onMessageCb进行处理，
    //在onMessage中进行数据分析
    int saveErrno = 0;
    int bytes = input_.readFd(channel_->fd(),saveErrno);
    if(bytes>0){
        if(onMessageCb_)
            onMessageCb_(shared_from_this(),input_);
        else{
            input_.retrieve(input_.readable());
        }
    }
    else{
        HandleClose();
    }
}

void TcpConnection::HandleClose() {
  if(CloseCb_){
      loop_->runInLoop(std::bind(CloseCb_, shared_from_this()));
  }
}
void TcpConnection::connDestroyed(){
    loop_->assertInLoopThread();
    channel_->disableAll();
}

void TcpConnection::HandleErrno() {

    HandleClose();
}

void TcpConnection::HandleWrite(){
    if (conn_state == CLOSED)
        return;
    if (channel_->isWriting()) {
        ssize_t n = ::write(channel_->fd(), output_.peek(),output_.readable());
        if (n >= 0) {
            if (output_.readable() == n) {
                output_.retrieve(n);
                channel_->disableWriting();
                HandleClose();
            }else{
                output_.update(n);
            }
        } else {
            // 对端尚未与我们建立连接或者对端已关闭连接
            if (errno == ECONNRESET || errno == EPIPE) {
                HandleErrno();
                return;
            }
        }
    }

}

void TcpConnection::sendInLoop(const char *data, size_t len)
{
    ssize_t n = 0;
    size_t remainBytes = len;

    if (conn_state == CLOSED)
        return;
    //有数据可能是上次没有发送完的数据。不能发送,会造成乱序。
    if (!channel_->isWriting() && output_.readable() == 0) {
        n = write(channel_->fd(), data, len);
       if (n >= 0) {
            remainBytes = len - n;
            if (remainBytes == 0) {
                if (conn_state == CLOSING){
                    HandleClose();
                    }
            }
        } else {
            if (errno == ECONNRESET || errno == EPIPE) {
                HandleClose();
                return;
            }
        }
    }
    if (remainBytes > 0) {
        output_.append(data + n, len - n);
        if (!channel_->isWriting()){
            channel_->enableWriting();
        }
    }
}

void TcpConnection::send(const char *s, size_t len)
{
    if (loop_->isInLoopThread()) {
        // 在本线程直接发送即可
        sendInLoop(s, len);
    } else {
        // 跨线程必须将数据拷贝一份，防止数据失效
        loop_->runInLoop(
                std::bind(&TcpConnection::sendInNotIoThread, this,
                          std::string(s, len)));
    }
}
void TcpConnection::sendInNotIoThread(const std::string& data)
{
    sendInLoop(data.data(), data.size());
}

void TcpConnection::send(const char *s)
{
    send(s, strlen(s));
}

void TcpConnection::send(const std::string& s)
{
    send(s.data(), s.size());
}


