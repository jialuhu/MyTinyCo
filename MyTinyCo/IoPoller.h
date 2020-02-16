//
// Created by Jialu  Hu on 2019-08-02.
//

#ifndef UNTITLED_KQUEUE_H
#define UNTITLED_KQUEUE_H
#ifdef OS_LINUX
#include <sys/epoll.h>
#include <vector>
#include <map>
#include <assert.h>
#include <unistd.h>
#include "Poller.h"
#include "Noncopyable.h"
namespace SiNet {
    class Channel;
    class Epoll : public Poller {
    public:
        Epoll();

        ~Epoll();

        virtual void updateChannel(Channel* channel);

        void fillactiveChannel(int number, Channel* channel);

        virtual int loop_wait(int timeout, std::vector<Channel*>* activeChannel);

    private:
        bool Register(Channel* channel, int fd);

        bool Change(Channel* channel, int fd);

        int ReturnEvent(int& ev);

        int epfd_;
        typedef std::vector<struct kevent> KqueueList;
        typedef std::map<int, Channel*> Channel_Map;
        Channel_Map channels_;
        std::vector<struct epoll_event> EpList_;
    };
}
#elif defined(OS_MACOSX)
#include <sys/time.h>
#include <sys/types.h>
#include <sys/event.h>
#include <gmpxx.h>
#include "Channel.h"
#include "Evloop.h"
#include "Poller.h"
#include <vector>
#include <map>
namespace SiNet {
    class IoPoller : public Poller {
    public:
        IoPoller();

        ~IoPoller();

        virtual void updateChannel(Channel* channel);

        void fillactiveChannel(int number, Channel* channel);

        virtual int loop_wait(int timeout, std::vector<Channel*>* activeChannel);

    private:
        bool Register(Channel* channel, int fd);

        bool Change(Channel* channel, int fd);

        int ReturnEvent(int& ev);

        int kqfd;
        typedef std::vector<struct kevent> KqueueList;
        typedef std::map<int, Channel*> Channel_Map;

        Channel_Map channels_;
        KqueueList kqfds_;

    };
}
#endif
#endif //UNTITLED_KQUEUE_H
