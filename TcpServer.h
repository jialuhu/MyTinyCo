//
// Created by Jialu  Hu on 2019-08-04.
//

#ifndef UNTITLED_TCPSERVER_H
#define UNTITLED_TCPSERVER_H


#include "IOthreadpool.h"
#include "Acceptor.h"
#include "CallBack.h"
#include "Tcpconn.h"
#include <map>
namespace SiNet {
    class Buffer;

    class TcpServer {
    public:
        explicit TcpServer(EventLoop *loop, InetAddr &listenAddr);

        ~TcpServer();

        void newConnection(int sockfd);

        void start();

        void quit();

        void setConnectionCb(ConnectionCallback cb) {
            ConnectionCb_ = std::move(cb);
        }

        void setOnMessageCb(OnMessageCallback cb) {
            OnMessageCb_ = std::move(cb);
        }

        void setCloseCb(CloseCallback cb) {
            CloseCb_ = std::move(cb);
        }

        void setThreadNumber(int threadnumber);

        EventLoop *getNextLoop();

        void removeConnection(const TcpConnectionPtr &conn);

    private:
        EventLoopthreadPool *ioThreadPool_;
        EventLoop *loop_;
        InetAddr listenAddr_;
        Acceptor *acceptor_;
        ConnectionCallback ConnectionCb_;
        OnMessageCallback OnMessageCb_;
        WriteCompleteCallback WriteCompleteCb_;
        CloseCallback CloseCb_;
        int countId;
        std::map<std::string, TcpConnectionPtr> connections_;
    };
}
#endif //UNTITLED_TCPSERVER_H

