//
// Created by Jialu  Hu on 2019-08-03.
//

#ifndef UNTITLED_SOCKET_H
#define UNTITLED_SOCKET_H
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <assert.h>
#include "SocketOpt.h"
namespace SiNet {
    class Socket : Noncopyable{
    public:
        explicit Socket(int fd,bool setNoBlock) : sockfd_(fd) {
            if(setNoBlock)
                SocketOpt::setnonblocking(sockfd_);
            assert(sockfd_ > 0);
        }

        ~Socket() { ::close(sockfd_); };

        int sockfd() {
            return sockfd_;
        }
        void setTcpNotDelay(bool on){
            int option = on ? 1 : 0;
            ::setsockopt(sockfd_,IPPROTO_TCP,TCP_NODELAY,&option, sizeof(option));
        }
        void setKeepAlive(bool on){
            int optval = on ? 1 : 0;
            ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE,
                         &optval, (sizeof optval));
            // FIXME CHECK
        }
    private:
        const int sockfd_;
    };
}

#endif //UNTITLED_SOCKET_H
