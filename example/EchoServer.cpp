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
    conn->send(buf.peek(), buf.readable());
    buf.retrieve(buf.readable());
}
int main()
{
    EventLoop loop;
    InetAddr  Addr(8000,"127.0.0.1");
    TcpServer echoServer_(&loop,Addr);
    echoServer_.setOnMessageCb(std::bind(&onMessage,std::placeholders::_1,std::placeholders::_2));
    echoServer_.start();
    loop.loop();
    return 0;
}
