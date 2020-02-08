//
// Created by Jialu  Hu on 2019-08-01.
//
#include "Channel.h"
#include "Evloop.h"
using namespace SiNet;
Channel ::Channel(EventLoop *eventLoop, int fd) :
        eloop_(eventLoop),
        fd_(fd),
        event_(0),
        revents_(0),
        isclose(false),
        iswrite(false),
        indx(-1)
        {

}

Channel :: ~Channel(){
}

//更新时间表
void Channel :: update() {
    eloop_ ->updateChannel(this);
}

//执行回调函数
void Channel :: handleEvent() {
    //eventHanding保证在处理事件期间Channel不被析构
    eventHanding_ = true;
    //客户端关闭链接
    if((revents_ & EREAD) && !is_close()) {
        if (ReadCallback) {
            ReadCallback();
        }
    }
    if((revents_ & EWRITE) && !is_close()) {
        if (WriteCallback) {
            WriteCallback();
        }
    }
    if((revents_ & EERROR) || (is_close())){
        if(CloseCallback){
            CloseCallback();
        }
    }
    eventHanding_ = false;

    //eventHanding保证在处理事件期间Channel不被析构
}

