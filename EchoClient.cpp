//
// Created by Jialu  Hu on 2020-02-08.
//
#include "TcpClient.h"
#include "Evloop.h"
#include "Buffer.h"
#include <stdio.h>
using namespace SiNet;
void onMessage(const TcpConnectionPtr& conn, Buffer& buf)
{
    std::cout << buf.c_str() << std::endl;
    buf.retrieve(buf.readable());
}
void InputStream(TcpClient& client_){
    char buf[256];
    while(std::cin.getline(buf,256)){
        buf[strlen(buf)+1]='\0';
        ssize_t n = ::write(client_.conn()->getChannle()->fd(),buf,strlen(buf));
    }
}
int main(){
    EventLoopthread loop;
    InetAddr Addr(8000, "127.0.0.1");
    TcpClient client_(loop.GetStartLoop(),Addr);
    client_.setOnMessageCb(std::bind(&onMessage,std::placeholders::_1,std::placeholders:: _2));
    client_.start();
    InputStream(client_);
    return 0;
}