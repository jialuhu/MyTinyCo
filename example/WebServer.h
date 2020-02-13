//
// Created by Jialu  Hu on 2019-08-02.
//

#ifndef UNTITLED_WEBSERVER_H
#define UNTITLED_WEBSERVER_H

#include "InetAddr.h"
#include "TcpServer.h"
#include "CallBack.h"
#include "Evloop.h"
#include <iostream>
#include <netinet/in.h>
class GetConfig;

class WebServer{
public:
    WebServer(SiNet::EventLoop *loop, SiNet::InetAddr &addr);
    ~WebServer();
    void onMessage(const SiNet::TcpConnectionPtr &conn, SiNet::Buffer &buf);
    void onConnection(const SiNet::TcpConnectionPtr &conn);
    void start();
    void setThread(size_t number){
        server_.setThreadNumber(number);
    }
    void setConfig(const char* SP, const char* CGIP, const char* DOCP,bool B_CGI){
        SP_.insert(0,SP);
        CGIP_.insert(0,CGIP);
        DOCP_.insert(0,DOCP);
        B_CGI_ = B_CGI;
    }
    void quit_server();
private:
    SiNet::EventLoop *loop_;
    SiNet::TcpServer server_;
    std::string SP_;
    std::string CGIP_;
    std::string DOCP_;
    bool B_CGI_;
};

#endif //UNTITLED_WEBSERVER_H
