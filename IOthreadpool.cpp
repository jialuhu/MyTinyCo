//
// Created by Jialu  Hu on 2019-10-30.
//
#include "IOthreadpool.h"
#include <iostream>
using namespace SiNet;
void EventLoopthreadPool::SetThreadNumber(int thread_number){
    ThreadNumber_ = thread_number;
    start();
}

void EventLoopthreadPool::start() {
    for(int i=0; i<ThreadNumber_; i++){
        EventLoopthread *one_thread = new EventLoopthread;
        IothreadPool_.push_back(one_thread);
        //每个线程进入loop状态
        EventIOLoops_.push_back(one_thread->GetStartLoop());
    }
}

EventLoop* EventLoopthreadPool::GetioLoop() {
    EventLoop *itLoop;
    if(!EventIOLoops_.empty()){
        itLoop = EventIOLoops_[NextIndex_++];
        if(NextIndex_>=ThreadNumber_){
            NextIndex_ = 0;
        }
    }
    return itLoop;

}
