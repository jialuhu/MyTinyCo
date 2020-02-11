//
// Created by Jialu  Hu on 2020-01-29.
//

#ifndef MYSNETCO_CONNECTOR_H
#define MYSNETCO_CONNECTOR_H

#include "Evloop.h"
#include "InetAddr.h"
#include "SocketOpt.h"
#include "Channel.h"
#include "Socket.h"
namespace SiNet{
    class Connector{
    public:
        typedef std::function<void(int socketfd,
                                   InetAddr &addr)> NewConnectionCallBack;
        Connector(EventLoop* loop,InetAddr& addr)
                    :loop_(loop),
                    connected_(false),
                    addr_(addr),
                    _waitTime(2),
                    socket_(SocketOpt::socket(),false),
                    ConnectorChannle_(new Channel{loop,socket_.sockfd()}){
            addr_.Init();
        }
        ~Connector(){

        }
        void timeout()
        {
            if (!connected_) {
                if (_waitTime == _waitMaxTime){

                }
                _waitTime *= 2;
                loop_->runAfter(1000 * _waitTime, [this]{ this->timeout(); });
            }
        }
        void connect();
        void check(int sockfd);
        void connected(int sockfd);
        void setNewConnectionCb(NewConnectionCallBack cb_){
            newConnectionCb_ = std::move(cb_);
        }

    private:
        static const int _waitMaxTime = 16;
        static const int _waitAllTime = 30;
        EventLoop* loop_;
        bool connected_;
        Socket socket_;
        InetAddr addr_;
        std::shared_ptr<Channel> ConnectorChannle_;
        NewConnectionCallBack newConnectionCb_;
        int _waitTime;

    };
}
#endif //MYSNETCO_CONNECTOR_H