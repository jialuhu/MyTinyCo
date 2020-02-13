//
// Created by Jialu  Hu on 2019-10-27.
//
#include "IOthread.h"
#include "Evloop.h"
using namespace SiNet;
EventLoopthread::EventLoopthread():
        loop_(nullptr),
        thread_([this]{this->threadFunc();})
{
    thread_.detach();
}

EventLoopthread::~EventLoopthread() {
    if(loop_){
        loop_->quit();
    }
}

EventLoop* EventLoopthread::GetStartLoop() {
    std::unique_lock<std::mutex> lock(mutex_);
    //等待新的线程获取到该线程的loop地址
    while (!loop_){
        condVar_.wait(lock);
    }
    return loop_;
}

void EventLoopthread ::threadFunc() {
    EventLoop thread_loop;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        loop_ = &thread_loop;
        condVar_.notify_one();
    }
    thread_loop.loop();
}
