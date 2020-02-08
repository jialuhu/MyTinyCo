//
// Created by Jialu  Hu on 2020-02-08.
//
#include "TcpClient.h"
#include "Evloop.h"
#include "Buffer.h"
#include <stdio.h>
using namespace SiNet;
void onConnection(const TcpConnectionPtr& conn){
    while(true){
        std::string str;
        std::cin >> str;
        conn->send(str);
    }
}
int main(){
    EventLoop loop;
    InetAddr addr(8000,"127.0.0.1");
    TcpClient discardClient(&loop,addr);
    discardClient.setConnectionCb(std::bind(&onConnection,std::placeholders::_1));
    discardClient.start();
    loop.loop();
    return 0;
}