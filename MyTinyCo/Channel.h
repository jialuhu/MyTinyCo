//
// Created by Jialu  Hu on 2019-08-01.
//

#ifndef TINY_WEB_CHANNEL_H
#define TINY_WEB_CHANNEL_H

#include <boost/function.hpp>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <iostream>
#include "Noncopyable.h"
namespace SiNet {
    class EventLoop;

    class Channel : std::enable_shared_from_this<Channel> ,Noncopyable {
    public:

        enum EventType {
            EREAD     = 0x01,
            EWRITE    = 0x02,
            EERROR    = 0x04,
        };
        typedef boost::function<void()> EventCallBack;

        Channel(EventLoop *eventLoop, int fd);

        ~Channel();

        //返回查看是否是该eventloop对象
        EventLoop *ownerloop() {
            return eloop_;
        }

        //设置读回调
        void setReadCallback(EventCallBack cb) {
            ReadCallback = std::move(cb);
        }

        void setCloseCallback(EventCallBack cb) {
            CloseCallback = std::move(cb);
        }

        //设置写回调
        void setWriteCallback(EventCallBack cb) {
            WriteCallback = std::move(cb);
        }

        //设置错误回调
        void setErnnoCallback(EventCallBack cb) {
            ErnnoCallback = std::move(cb);
        }

        //注册可读事件
        void enableReading() {
            event_ |= EREAD;
            update();
        }

        void updateChannle(){
            update();
        }

        //注册可写事件
        void enableWriting() {
            event_ |= EWRITE;
            update();
        }

        //注册不可读写事件
        void disableAll() {
            isclose = true;
            update();
        }

        bool isReading(){
            return event_ & EREAD;
        }

        bool isWriting() {
            return event_ & EWRITE;
        }

        void disableReading(){
            event_ &= ~EREAD;
        }

        //不可再写
        void disableWriting() {
            event_ &= ~EWRITE;
            update();
        }

        void set_index(int index) {
            indx = index;
        }

        void set_iswrite(int is) {
            iswrite = is;
        }

        void set_isclose(bool ev) {
            isclose = ev;
        }

        void set_revent(int ev) {
            revents_ = ev;
        }

        void set_flags(int flag) {
            flags_ = flag;
        }

        bool is_close() {
            return isclose;
        }

        bool is_write() {
            return iswrite;
        }

        int fd() {
            return fd_;
        }

        int index() {
            return indx;
        }

        int event() {
            return event_;
        }

        int flags() {
            return flags_;
        }

        void handleEvent();

    private:

        void update();

        EventCallBack ReadCallback;
        EventCallBack WriteCallback;
        EventCallBack ErnnoCallback;
        EventCallBack CloseCallback;

        EventLoop *eloop_;
        int fd_;
        int event_;
        bool isclose;
        bool iswrite;
        int revents_;
        int indx;
        int flags_;
        bool eventHanding_;
    };
}

#endif //TINY_WEB_CHANNEL_H
