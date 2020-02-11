//
// Created by Jialu  Hu on 2019-08-04.
//

#ifndef UNTITLED_CALLBACK_H
#define UNTITLED_CALLBACK_H
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
namespace SiNet {
    class TcpConnection;

    class Buffer;

    typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
    typedef std::function<void(const TcpConnectionPtr &conn)> ConnectionCallback;
    typedef std::function<void(const TcpConnectionPtr &, Buffer &buf)> OnMessageCallback;
    typedef std::function<void(const TcpConnectionPtr &)> WriteCompleteCallback;
    typedef std::function<void(const TcpConnectionPtr &)> CloseCallback;

    //TimeEvent
    typedef std::function<void()> TimeEventCallback;
}
#endif //UNTITLED_CALLBACK_H
