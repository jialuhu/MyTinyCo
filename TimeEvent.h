//
// Created by Jialu  Hu on 2020-01-28.
//

#ifndef MYSNETCO_TIMEEVENT_H
#define MYSNETCO_TIMEEVENT_H

#include "CallBack.h"
#include "Time.h"
namespace SiNet{

    //一个定时事件
    class TimeEvent{
    public:
        TimeEvent(int64_t Expire, int64_t Interval, TimeEventCallback TimeEventCb)
                : Id_(0),Expire_(Expire),Interval_(Interval),
                TimeEventCb_(std::move(TimeEventCb)){
        }
        ~TimeEvent(){

        }
        int64_t Return_expire(){
            return Expire_;
        }
        int64_t Return_interval(){
            return Interval_;
        }
        int Return_TimeId(){
            return Id_;
        }
        TimeEventCallback Return_TimeCb(){
            return TimeEventCb_;
        }
        void setTimeExpire(int64_t TimeExpire){
            Expire_ = TimeExpire;
        }
        void setTimeInterval(int64_t TimeInterval){
            Interval_ = TimeInterval;
        }
        void setTimeEventCb(TimeEventCallback TimeEventCb){
            TimeEventCb_ = std::move(TimeEventCb);
        }
        void setId(size_t id){
            Id_= id;
        }
    private:
        size_t Id_;//表示该定时事件的id号
        int64_t Expire_;//到期时间
        int64_t Interval_;//重复间隔时间
        TimeEventCallback TimeEventCb_;//定时回调任务
    };
}
#endif //MYSNETCO_TIMEEVENT_H
