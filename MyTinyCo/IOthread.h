//
// Created by Jialu  Hu on 2019-10-27.
//

#ifndef UNTITLED_EVENTLOOPTHREAD_H
#define UNTITLED_EVENTLOOPTHREAD_H
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Noncopyable.h"
namespace SiNet {
    class EventLoop;

    class EventLoopthread : private Noncopyable{
    public:
        EventLoopthread();

        ~EventLoopthread();

        EventLoop *GetStartLoop();

    private:
        void threadFunc();

        EventLoop *loop_;
        std::thread thread_;
        std::mutex mutex_;
        std::condition_variable condVar_;

    };
}

#endif //UNTITLED_EVENTLOOPTHREAD_H
