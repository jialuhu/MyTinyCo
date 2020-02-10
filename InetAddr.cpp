//
// Created by Jialu  Hu on 2019-08-03.
//

#include "InetAddr.h"
#include <strings.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Log.h"

using namespace SiNet;
InetAddr::InetAddr(int port,const char* c_addr):port_(port),c_addr_(c_addr) {

}
InetAddr::InetAddr(int port) : port_(port){
    bzero(&address_, sizeof(address_));
}

InetAddr::~InetAddr() {

}

void InetAddr :: Init(){
    address_.sin_family = AF_INET;
    address_.sin_port = htons(port_);
    if (inet_pton(AF_INET, c_addr_, &address_.sin_addr) <= 0){
        logFatal("InetAddr::Init Inet_pton is wrong.");
        exit(0);
    }
}
