//
// Created by Jialu  Hu on 2020-02-08.
//
#include "TcpClient.h"
#include "Evloop.h"
#include "Buffer.h"
#include <stdio.h>
using namespace SiNet;
void onConnectionTime(const TcpConnectionPtr& conn){
    time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm * ptminfo = localtime(&tt);
    char date[60] = { 0 };
    sprintf(date, "%02d-%02d-%02d %02d:%02d:%02d",
            ptminfo->tm_year + 1900, ptminfo->tm_mon + 1,
            ptminfo->tm_mday, ptminfo->tm_hour,
            ptminfo->tm_min, ptminfo->tm_sec);
    std::cout << std::string(date) << std::endl;
    conn->send(date);
    ::close(conn->getChannle()->fd());
    conn->getChannle()->disableAll();
}
int main(){
    EventLoop loop;
    InetAddr Addr(8000,"127.0.0.1");
    TcpServer dayTimeServer(&loop,Addr);
    dayTimeServer.setConnectionCb(std::bind(&onConnectionTime,std::placeholders::_1));
    dayTimeServer.start();
    loop.loop();
    return 0;
}
