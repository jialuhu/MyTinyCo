/*#include <iostream>
#include "TcpServer.h"
using namespace SiNet;
int main() {
    EventLoop p;
    InetAddr addr(8888);
    TcpServer server(&p,addr);
    //std::cout << "Hello, World!" << std::endl;

    server.start();
    //std::cout << "Hello, World!" << std::endl;
    std::cout << "END World!" << std::endl;

    p.loop();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}*/
/*#include "WebServer.h"
#include "GetConfig.h"
using namespace SiNet;
int main(){
    GetConfig GetConfig_ ("/Users/jialuhu/CLionProjects/webserver/httped.conf");
    GetConfig_.ReadConfig();
    GetConfig_.display();
    EventLoop loop;
    InetAddr addr(GetConfig_.Return_Listen());
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
}*/
#include "TcpClient.h"
#include "Taskthreadpool.h"
#include "Evloop.h"
#include "Buffer.h"
#include "Log.h"
#include <stdio.h>
using namespace SiNet;
/*
 * //echo_server
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
//echo_client
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
}*/

/*
 *
 *discard_server
void onConnection(const TcpConnectionPtr &conn){
    std::cout << "Discard Server is satared!\n";
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

 discar_client
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
}*/

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
    log::getLogger().start();
    EventLoop loop;
    InetAddr Addr(8000,"127.0.0.1");
    TcpServer dayTimeServer(&loop,Addr);
    dayTimeServer.setConnectionCb(std::bind(&onConnectionTime,std::placeholders::_1));
    dayTimeServer.start();
    int fd = 0;
    logDebug("this is main test");
    logDebug("this is main test");
    logErrnos("this is errnos test");
    logFatal("this is fatal test");
    loop.loop();
    return 0;
}

