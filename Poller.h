//
// Created by Jialu  Hu on 2020-01-23.
//

#ifndef MYSNETCO_POLLER_H
#define MYSNETCO_POLLER_H

#include "Noncopyable.h"
namespace SiNet {
    class Channel;
    struct Poller : public Noncopyable{
        Poller(){}

        virtual ~Poller() {

        }

        virtual int loop_wait(int timeout, std::vector<Channel*> *activeChannel) = 0;

        virtual void updateChannel(Channel* ch) = 0;
    };
}
#endif //MYSNETCO_POLLER_H
