//
// Created by Jialu  Hu on 2020-01-28.
//
#include "TimeEventSet.h"
using namespace SiNet;
size_t TimeEventSet::addTimeEvent(TimeEvent *timer){
    size_t id = TimeEventId_++;
    timer->setId(id);
    loop_->runInLoop([this,timer,id]{
        auto it = std::shared_ptr<TimeEvent>(timer);
        TimeEvents_.insert(it);
        IdMaps_.insert(std::make_pair(TimeEventId_, it));
    });
    return TimeEventId_;
}

void TimeEventSet::deleteTimeEvent(std::multiset<std::shared_ptr<TimeEvent>, TimeEventCmp>::iterator it,
                 size_t id){
    TimeEvents_.erase(it);
    IdMaps_.erase(id);
}

void TimeEventSet::cancleTimeEvent(size_t id) {
    auto it = IdMaps_.find(id);
    if(it != IdMaps_.end()){
        //有多个元素重复，不能简单find
        auto lower = TimeEvents_.lower_bound(it->second);
        auto upper  = TimeEvents_.upper_bound(it->second);
        for(auto its = lower; its != upper; its++){
            if(its->get()->Return_TimeId()==id){
                deleteTimeEvent(its,id);
            }
        }
    }
}
void TimeEventSet::exeTask() {
    auto timenow = SiNet::nowTime();
    while (true){
        auto it = TimeEvents_.begin()->get();
        auto expire = it->Return_expire();
        //超时间
        if(timenow >= expire){
            auto id = it->Return_TimeId();
            it->Return_TimeCb()();
            //有重复间隔时间任务
            if(it->Return_interval()>0) {
                TimeEvent *timer = new TimeEvent(it->Return_expire() + timenow,
                                                 it->Return_interval(), it->Return_TimeCb());
                deleteTimeEvent(TimeEvents_.begin(), id);
                addTimeEvent(timer);
            }else{
                deleteTimeEvent(TimeEvents_.begin(),id);
            }
        }else if(TimeEvents_.empty() || timenow < expire){
            break;
        }
    }
}
