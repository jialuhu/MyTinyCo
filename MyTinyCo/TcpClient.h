//
// Created by Jialu  Hu on 2020-01-29.
//

#ifndef MYSNETCO_TCPCLIENT_H
#define MYSNETCO_TCPCLIENT_H

#include "IOthreadpool.h"
#include "Connector.h"
#include "CallBack.h"
#include "Tcpconn.h"
#include <map>
namespace SiNet {
    class TcpClient {
    public:
        TcpClient(EventLoop *loop, InetAddr &listenAddr);

        ~TcpClient();

        void CreateConnection(int sockfd);

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
        const TcpConnectionPtr& conn() const { return conn_; }

        void removeConnection(const TcpConnectionPtr &conn);

    private:
        EventLoop *loop_;
        InetAddr listenAddr_;
        Connector *connector_;
        int countId_;
        TcpConnectionPtr conn_;
        ConnectionCallback ConnectionCb_;
        OnMessageCallback OnMessageCb_;
        CloseCallback CloseCb_;
    };
}
#endif //MYSNETCO_TCPCLIENT_H
