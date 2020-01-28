//
// Created by Jialu  Hu on 2020-01-28.
//

#ifndef MYSNETCO_TIMEEVENTSET_H
#define MYSNETCO_TIMEEVENTSET_H

#include <sys/time.h>
#include <set>
#include "TimeEvent.h"
#include "EventLoop.h"
namespace SiNet{
    //表示一个定时事件的集合
    struct TimeEventCmp : Noncopyable{
        bool operator()(const std::shared_ptr<TimeEvent>& l,
                        const std::shared_ptr<TimeEvent>& r) const
        {
            return l->Return_expire() < r->Return_expire();
        }
    };

    class TimeEventSet : Noncopyable{
    public:
        explicit TimeEventSet(EventLoop* loop)
                :loop_(loop),
                TimeEventId_(0){

        }
        ~TimeEventSet(){

        }
        int64_t getTimeout(){
            if(TimeEvents_.empty()){
                return -1;
            }
            auto t = TimeEvents_.begin()->get()->Return_expire() -SiNet::nowTime();
            return t>0?t:-1;
        }
        size_t addTimeEvent(TimeEvent *timer);
        void deleteTimeEvent(std::multiset<std::shared_ptr<TimeEvent>, TimeEventCmp>::iterator it,
                size_t id);
        void cancleTimeEvent(size_t id);
        void exeTask();
    private:
        EventLoop* loop_;
        size_t TimeEventId_;
        std::multiset<std::shared_ptr<TimeEvent>,TimeEventCmp> TimeEvents_;
        std::map<size_t, std::shared_ptr<TimeEvent>> IdMaps_;

    };
}
#endif //MYSNETCO_TIMEEVENTSET_H
