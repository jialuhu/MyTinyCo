//
// Created by Jialu  Hu on 2019-08-04.
//
#include "WebServer.h"
#include "Evloop.h"
#include "Acceptor.h"
#include "HttpContent.h"
using namespace SiNet;
WebServer::WebServer(EventLoop *loop, InetAddr &addr):loop_(loop),
    server_(loop,addr){
    server_.setConnectionCb(std::bind(&WebServer::onConnection,this,std::placeholders::_1));
    server_.setOnMessageCb(std::bind(&WebServer::onMessage,this,std::placeholders::_1,std::placeholders::_2));
}
WebServer::~WebServer(){

}
void WebServer::start() {
    server_.start();
}

void WebServer::quit_server() {
    server_.quit();
}

void WebServer::onMessage(const TcpConnectionPtr &conn, Buffer &buf) {
    auto &httpcontent = boost::any_cast<HttpContent&>(conn->getContext());
    httpcontent.setConfig(SP_,CGIP_,DOCP_,B_CGI_);
    httpcontent.doit(conn,buf);
}

void WebServer::onConnection(const TcpConnectionPtr &conn) {
    //建立新的http事件
    conn->setContext(HttpContent());
}

#include "WebServer.h"
#include "./config/GetConfig.h"

int main(){
    GetConfig GetConfig_ ("../example/config/httped.conf");
    GetConfig_.ReadConfig();
    EventLoop loop;
    InetAddr addr(GetConfig_.Return_Listen(),GetConfig_.Return_ADDR());
    WebServer server(&loop,addr);
    server.setConfig(GetConfig_.Return_ServerRoot(),
                     GetConfig_.Return_CGIPath(),
                     GetConfig_.Return_DocumentPath(),
                     GetConfig_.Return_CGI()
    );
    server.setThread(0);
    server.start();
    loop.loop();
    return 0;
}
