//
// Created by Jialu  Hu on 2019-08-03.
//

#ifndef UNTITLED_ACCEPTOR_H
#define UNTITLED_ACCEPTOR_H

#include "Evloop.h"
#include "Channel.h"
#include "Socket.h"
#include "InetAddr.h"
#include "SocketOpt.h"
namespace SiNet {
    class Acceptor : Noncopyable {
//在acceptor类中进行该服务器套接字的绑定和监听，并设置注册回调函数
    public:
        typedef std::function<void(int socketfd,
                                     InetAddr &addr)> NewConnectionCallBack;

        Acceptor(EventLoop *loop, InetAddr &addr);

        ~Acceptor();

        void listen();

        void setNewConnectionCallback(NewConnectionCallBack _cb) {
            _newConnectionCb = std::move(_cb);
        }

    private:
        //在Channel中注册该事件，并且绑定
        void handleAccept();

        EventLoop *loop_;
        NewConnectionCallBack _newConnectionCb;
        Socket socket_;
        InetAddr addr_;
        std::shared_ptr<Channel> AcceptorChannle_;
    };
}
#endif //UNTITLED_ACCEPTOR_H
