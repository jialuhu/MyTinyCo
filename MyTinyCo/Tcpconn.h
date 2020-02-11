//
// Created by Jialu  Hu on 2019-08-04.
//

#ifndef UNTITLED_TCPCONNECTION_H
#define UNTITLED_TCPCONNECTION_H
#include <boost/any.hpp>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include "InetAddr.h"
#include "TcpServer.h"
#include "Buffer.h"
#include "Noncopyable.h"
namespace SiNet {
    class TcpConnection : public std::enable_shared_from_this<TcpConnection>, Noncopyable {
    public:
        //记录连接的状态
        enum State {
            CONNECTING,
            CONNCTED,
            CLOSED,
            CLOSING,
        };
        State ReturnState(){
            return conn_state;
        }
        InetAddr ReturnAddr(){
            return LocalAddr_;
        }
        TcpConnection(EventLoop *loop, std::string cooname, int connfd, InetAddr &local);

        ~TcpConnection();

        void setConnectionCb(ConnectionCallback cb) {
            connectionCb_ = std::move(cb);
        }

        void setMessageCb(OnMessageCallback cb) {
            onMessageCb_ = std::move(cb);
        }

        void setCloseCb(CloseCallback cb) {
            CloseCb_ = std::move(cb);
        }

        void setWriteCompleteCb(WriteCompleteCallback cb){
            WriteCompleteCb_ = std::move(cb);
        }
        //避免连续发包出现延时
        void setTcpNotDelay(bool on){
            socket_->setTcpNotDelay(on);
        }

        void setKeepAlive(bool on){
            socket_->setKeepAlive(on);
        }
        //设置具体的连接对象
        void setContext(const boost::any context) {
            context_ = std::move(context);
        }

        //返回具体的连接对象
        boost::any &getContext() {
            return context_;
        }

        const std::string &name() const {
            return conn_name_;
        }

        int sockfds() {
            return connfd_;
        }
        std::shared_ptr<Channel>& getChannel() { return channel_; }
        //进行用户的回调
        void connectEstablished();

        void set_HandleErrno(int fd, std::string &head);

        void set_State(State state_){
            conn_state = state_;
        }
        void connDestroyed();
        std::shared_ptr<Channel> getChannle() const { return channel_;}
        EventLoop *getLoop() const { return loop_; }

        void send(const void *v, size_t len);
        void send(const std::string& s);
        void send(const char *s);
        void sendInNotIoThread(const std::string& data);
        void send(const char *s, size_t len);

    private:
        typedef std::function<void(TcpConnection &)> ConnectionCallbacks;

        void HandleRead();

        void HandleWrite();

        void HandleClose();

        void HandleErrno();

        void sendInLoop(const char *data, size_t len);

        std::shared_ptr<Channel> channel_;
        EventLoop *loop_;
        std::string conn_name_;
        int connfd_;
        InetAddr LocalAddr_;
        std::unique_ptr<Socket> socket_;
        State conn_state;
        ConnectionCallback connectionCb_;
        OnMessageCallback onMessageCb_;
        WriteCompleteCallback WriteCompleteCb_;
        CloseCallback CloseCb_;
        Buffer input_;
        std::string respond_head;
        Buffer output_;
        boost::any context_;

    };
}
#endif //UNTITLED_TCPCONNECTION_H

