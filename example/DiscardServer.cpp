//
// Created by Jialu  Hu on 2020-02-08.
//
#include "TcpClient.h"
#include "Evloop.h"
#include "Buffer.h"
#include <stdio.h>
using namespace SiNet;
void onConnection(const TcpConnectionPtr &conn){
    std::cout << "Discard Server is running!\n";
}
void onMessage(const TcpConnectionPtr& conn, Buffer& buff){
    std::cout << "Discard is working\n";
    buff.retrieve(buff.readable());
}
int main(){
    EventLoop loop;
    InetAddr addr(8000,"127.0.0.1");
    TcpServer discardServer(&loop,addr);
    discardServer.setConnectionCb(std::bind(&onConnection,std::placeholders::_1));
    discardServer.setOnMessageCb(std::bind(&onMessage,std::placeholders::_1,std::placeholders::_2));
    discardServer.start();
    loop.loop();
    return 0;
}
