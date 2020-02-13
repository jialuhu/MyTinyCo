//
// Created by Jialu  Hu on 2019-08-03.
//
#include "Acceptor.h"
using namespace SiNet;
Acceptor::Acceptor(EventLoop *loop,
        InetAddr &addr)
        :loop_(loop),
        addr_(addr),
        AcceptorChannle_(new Channel{loop,socket_.sockfd()}),
        socket_(SocketOpt::socket(),true)
        {
    addr_.Init();
    SocketOpt::bind(socket_.sockfd(),addr_.inetaddr());
    AcceptorChannle_->setReadCallback([this]{ this->handleAccept();});
    logInfo("Acceptor is created.");
}

Acceptor::~Acceptor() {
    logInfo("Acceptor is destructor.");

}

void Acceptor::listen() {
    //开启监听套接字
    SocketOpt::listen(socket_.sockfd());
    //向channel中注册可读事件,进行accept
    AcceptorChannle_->enableReading();
}

void Acceptor::handleAccept() {
    int connfd = SocketOpt::accept(socket_.sockfd());
    SocketOpt::setnonblocking(connfd);
    if(connfd > 0){
        if(_newConnectionCb){
            _newConnectionCb(connfd,addr_);
        }else{
            ::close(connfd);
        }
    }
}