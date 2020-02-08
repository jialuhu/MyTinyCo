//
// Created by Jialu  Hu on 2019-08-02.
//
#include "SocketOpt.h"
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
using namespace SiNet;
int SocketOpt ::socket() {
    int socketfd = ::socket(AF_INET,SOCK_STREAM,0);
    assert(socketfd>0);
    return socketfd;
}

void SocketOpt::bind(int sockfd, struct sockaddr_in *addr) {
    int ret = ::bind(sockfd,(struct sockaddr*)addr, sizeof(struct sockaddr_in));
    assert(ret >= 0);
}
struct sockaddr *SocketOpt::sockaddr_cast(struct sockaddr_in *addr)
{
    return reinterpret_cast<struct sockaddr *>(addr);
}
int SocketOpt::connect(int sockfd, struct sockaddr_in *addr)
{
    int ret = ::connect(sockfd, sockaddr_cast(addr), sizeof(*addr));
    return ret;
}
void SocketOpt::listen(int socketfd) {
    int ret = ::listen(socketfd, 1024);
    assert(ret >= 0);
}

int SocketOpt::accept(int sockfd) {
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int connfd = ::accept(sockfd,(struct sockaddr*)&addr, &len);
    return connfd;
}

int SocketOpt::setnonblocking(int fd) {
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void SocketOpt :: socketpair(int wakeup[]){
    //int wakeupfd[2];
    int ret = ::socketpair(AF_UNIX,SOCK_STREAM,0,wakeup);
    assert(ret!=-1);
    //return wakeup;
}
