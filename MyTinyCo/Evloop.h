//
// Created by Jialu  Hu on 2019-07-31.
//

#ifndef TINY_WEB_EventLoop_H
#define TINY_WEB_EventLoop_H

#include <iostream>
#include <atomic>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <map>
#include <vector>
#include "Noncopyable.h"
#include "TimeEventSet.h"
#include "SocketOpt.h"
namespace SiNet {

    class Epoll;

    class Channel;

    class IoPoller;

    class Poller;

    typedef std::function<void()> Functor;

    class EventLoop : private Noncopyable {
    public:
        EventLoop();

        ~EventLoop();

        void loop();

        void quit();

        void updateChannel(Channel* channel);

        bool isInLoopThread() {
            return std::this_thread::get_id() == threadId_;
        }

        void initwake();

        void runInLoop(const Functor &cb);

        void doPendingFunctors();

        void handleread();

        void queueInLoop(const Functor &cb);

        void wakeup();

        void assertInLoopThread() {
            if (!isInLoopThread()) {
                std::cout << "Thread is non in this pthread!\n";
                exit(0);
            }
        }

        void removeRunInLoop(const std::shared_ptr<Channel> channel_);
        void removeChannelInLoop(const std::shared_ptr<Channel> channel_);

        size_t runAfter(int64_t timeout, TimeEventCallback TimeEventCb);

        size_t runAt(TimeEvent *timerAt, TimeEventCallback TimeEventCb);

        size_t runAferEve(int64_t interval, TimeEventCallback TimeEventCb);
    private:
        std::unique_ptr<Poller> Poller_;
        //std::unique_ptr <IoPoller> Kqueue_;
        std::shared_ptr<Channel> wakeupChannel_;
        std::vector<Channel*> activeChannel_;
        //wakeupFd[0]读
        //wakeupFd[1]写
        int wakeupFd[2];
        bool quit_;
        bool looping_;
        bool callingPendingFunctors_;
        //定时器任务
        std::unique_ptr<TimeEventSet> TimeEventSet_;
        std::thread::id threadId_;
        std::mutex mutex_;
        std::vector<Functor> pendingFunctors_;


    };
}



#endif //TINY_WEB_EventLoop_H
