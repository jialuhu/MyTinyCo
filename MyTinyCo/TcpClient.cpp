//
// Created by Jialu  Hu on 2020-01-29.
//
#include "TcpClient.h"

using namespace SiNet;

TcpClient::TcpClient(SiNet::EventLoop *loop,
                     SiNet::InetAddr &listenAddr)
        : loop_(loop),
          countId_(0),
          listenAddr_(listenAddr),
          connector_(new Connector{loop, listenAddr}) {
    connector_->setNewConnectionCb(std::bind(&TcpClient::CreateConnection, this, std::placeholders::_1));

}

TcpClient::~TcpClient() {

}

void TcpClient::start() {
    connector_->connect();
}

void TcpClient::CreateConnection(int sockfd) {
    char buf[13] = {"HELLO"};
    countId_++;
    sprintf(buf, "%s_%d", buf, countId_++);
    /**
     * 此处从线程池中取出一个线程，并且用EventLoop指向
     */
    std::string s1(buf);
    conn_ = TcpConnectionPtr(new TcpConnection(loop_, s1, sockfd, listenAddr_));
    conn_->setMessageCb(OnMessageCb_);
    conn_->setCloseCb(std::bind(&TcpClient::removeConnection, this, std::placeholders::_1));
    if (ConnectionCb_) {
        loop_->runInLoop(
                [this] { this->ConnectionCb_(conn_); });
    }
}

void TcpClient::removeConnection(const SiNet::TcpConnectionPtr &conn) {
    conn->getLoop()->removeRunInLoop(conn->getChannle());
    conn->set_State(TcpConnection::CLOSED);
}