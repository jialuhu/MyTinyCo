//
// Created by Jialu  Hu on 2019-08-02.
//

#ifndef UNTITLED_SOCKETOPT_H
#define UNTITLED_SOCKETOPT_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <assert.h>
#include <fcntl.h>
#include "Log.h"
namespace SiNet {
    namespace SocketOpt {
        int socket();

        void bind(int sockfd, struct sockaddr_in *addr);

        void listen(int socketfd);

        int accept(int sockfd);

        int setnonblocking(int fd);

        void socketpair(int wakeup[]);

        struct sockaddr *sockaddr_cast(struct sockaddr_in *addr);

        int connect(int sockfd, struct sockaddr_in *addr);
    }
}
#endif //UNTITLED_SOCKETOPT_H
