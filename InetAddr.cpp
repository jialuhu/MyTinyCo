//
// Created by Jialu  Hu on 2019-08-03.
//

#include "InetAddr.h"
#include <strings.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace SiNet;
InetAddr::InetAddr(int port,const char* c_addr):port_(port),c_addr_(c_addr) {
    //bzero(&address_, sizeof(address_));
}
InetAddr::InetAddr(int port) : port_(port){
    bzero(&address_, sizeof(address_));
}

InetAddr::~InetAddr() {

}

void InetAddr :: Init(){
    address_.sin_family = AF_INET;
    address_.sin_port = htons(port_);
    //address_.sin_addr.s_addr = htons(INADDR_ANY);
    //address_.sin_addr.s_addr = inet_addr(c_addr_);
    if (inet_pton(AF_INET, c_addr_, &address_.sin_addr) <= 0){

    }
}
