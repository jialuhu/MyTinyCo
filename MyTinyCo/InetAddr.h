//
// Created by Jialu  Hu on 2019-08-03.
//

#ifndef UNTITLED_INETADDR_H
#define UNTITLED_INETADDR_H
#include <netinet/in.h>
namespace SiNet {
    class InetAddr{
    public:
        InetAddr(int port, const char* c_addr);
        InetAddr(int port);
        ~InetAddr();

        void Init();

        struct sockaddr_in *inetaddr() {
            return &address_;
        }
        struct sockaddr_in& inetAddr() { return address_; }
        struct sockaddr_in address_;
        const int port_;
        __const char* c_addr_;
    };
}

#endif //UNTITLED_INETADDR_H
