//
// Created by Jialu  Hu on 2020-01-29.
//
#include "Channel.h"
#include "Connector.h"
#include "InetAddr.h"
using namespace SiNet;

void Connector::connect(){
    addr_.Init();
    int sockfd = socket_.sockfd();
    int ret = ::connect(sockfd, (struct sockaddr*)addr_.inetaddr(), sizeof(struct sockaddr_in));
    std::cout << "ret: " << ret << std::endl;
    if (ret == 0) {
    // 如果服务端和客户端在同一台主机连接会立即建立
        if (connected_) return;
        if (newConnectionCb_) {
            newConnectionCb_(sockfd,addr_);
            connected_ = true;
        } else{
            loop_->quit();
        }
    } else if (ret < 0) {
        if (errno == EINPROGRESS) {
            // 连接正在建立
            loop_->runAfter(1000 * _waitTime, [this]{ this->timeout(); });
            ConnectorChannle_->setReadCallback(
                    [this, sockfd]{ this->check(sockfd); });
            ConnectorChannle_->setWriteCallback(
                    [this, sockfd]{ this->check(sockfd); });
            ConnectorChannle_->enableWriting();
        }else{
            std::cout << "Connector is wrong\n";
        }
    }else{
        std::cout << "Connector is wrong\n";
    }
}

// 在Mac OS上使用poll判断sockfd的状态，好像有时会可读，有时会可写
// 所以我们在可读可写情况下都使用getsockopt来获取sockfd的状态
void Connector::connected(int sockfd)
{
    loop_->removeRunInLoop(ConnectorChannle_);
    ConnectorChannle_.reset();
    if (newConnectionCb_) {
        newConnectionCb_(sockfd,addr_);
        connected_ = true;
    } else{
        loop_->quit();
    }
}
void Connector::check(int sockfd)
{
    socklen_t err;
    socklen_t len = sizeof(err);
    if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &err, &len) < 0){
        std::cout << errno << std::endl;
    }
    connected(sockfd);
}
