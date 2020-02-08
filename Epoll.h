//
// Created by Jialu  Hu on 2020-01-23.
//

#ifndef MYSNETCO_EPOLL_H
#define MYSNETCO_EPOLL_H
#include <sys/epoll.h>
#include <vector>
#include <map>
#include <assert.h>
#include <unistd.h>
#include "Poller.h"
#include "noncopyable.h"
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
#endif //MYSNETCO_EPOLL_H
