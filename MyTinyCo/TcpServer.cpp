//
// Created by Jialu  Hu on 2019-08-04.
//
#include "TcpServer.h"
#include "IOthread.h"
#include "Log.h"
using namespace SiNet;
TcpServer::TcpServer(EventLoop *loop, InetAddr &listenAddr):
            loop_(loop),
            listenAddr_(listenAddr),
            ioThreadPool_(new EventLoopthreadPool),
            countId(0),
            acceptor_(new Acceptor(loop,listenAddr)){
    acceptor_->setNewConnectionCallback( std::bind(&TcpServer::newConnection,
            this, std::placeholders::_1));
}

TcpServer::~TcpServer() {
    logInfo("The destructor of TcpServer.");
}

void TcpServer::setThreadNumber(int threadnumber)  {
    ioThreadPool_->SetThreadNumber(threadnumber);
}

void TcpServer::start() {
    acceptor_->listen();

}

void TcpServer::quit(){
    loop_ -> quit();
}
EventLoop* TcpServer::getNextLoop() {
    if(ioThreadPool_->GetThreadNumber()>0){
        return ioThreadPool_->GetioLoop();
    }else{
        return loop_;
    }
}
void TcpServer::newConnection(int connfd) {
    logDebug("this is new Connection.");
    //此处建立相对应的连接事件，并且设置读写事件
     char buf[13]={"HELLO"};
     countId++;
     sprintf(buf,"%s_%d",buf,countId++);
     /**
      * 此处从线程池中取出一个线程，并且用EventLoop指向
      */
     EventLoop *New_loop = getNextLoop();
     std::string s1(buf);
    /*多线程需要设置loop_为线程池中的线程*/
     TcpConnectionPtr conn(new TcpConnection(New_loop,s1,connfd,listenAddr_));
     connections_[s1]= conn;
     /**
      * ConnectionCb_绑定的是WebServer的Connection
      * 同理OnMessageCb_也一样
      * **/
     conn->setConnectionCb(ConnectionCb_);
     conn->setMessageCb(OnMessageCb_);
     conn->setWriteCompleteCb(WriteCompleteCb_);
     conn->setCloseCb(std::bind(&TcpServer::removeConnection,this,std::placeholders::_1));
    /**
     * 用线程池的线程runInLoop
     */
      New_loop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr &conn){
    /**
     *为什么在此处，多线程执行的话需要由主循环结束?
     * Part1:因为在newconnection中的时候，已经将HTTP请求的处理交给了新创建的IO线程处理了，所以在新的IO线程中，将会
     * 触发结束套接字的功能模块，那此时该代码块将有可能会被多个线处理。
     * Part2：所以避免乱套，需要加入主循环EventLoop中，依次被关闭套接字
     * 此处需要对于多线程进行一定修改
     */
     /**
      * 与单线程不同*/

     conn->getLoop()->removeRunInLoop(conn->getChannle());
     conn->set_State(TcpConnection::CLOSED);
     //map中删除连接
     int n = connections_.erase(conn->name());
     assert(n==1);
}
