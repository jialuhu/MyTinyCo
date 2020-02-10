
// Created by Jialu  Hu on 2019-07-31.
//
#include <fcntl.h>
#include "Evloop.h"
#include "Channel.h"
#include "Kqueue.h"
#ifdef OS_LINUX
#include "Epoll.h"
#elif defined(OS_MACOSX)
#include "Kqueue.h"
#endif
#include "Poller.h"
#include "Log.h"

using namespace SiNet;
class IgnoreSignal{
public:
    IgnoreSignal(){
        ::signal(SIGPIPE,SIG_IGN);
    }
};

IgnoreSignal ignoreSignal;

thread_local EventLoop* t_loopInThisThread = nullptr;
EventLoop ::EventLoop() :
        quit_(false),
        looping_(false),
        callingPendingFunctors_(false),
        TimeEventSet_(new TimeEventSet{this}),
        threadId_(std::this_thread::get_id()),
        wakeupChannel_()
       {
        #ifdef OS_LINUX
        Poller_.reset(new Epoll);
        #elif defined(OS_MACOSX)
        Poller_.reset(new Kqueue());
        #endif
    //判断是否是属于本线程
    if(t_loopInThisThread){
        std::cout << "This thread is used\n";
    }
    else{
        t_loopInThisThread = this;
    }

}

EventLoop :: ~EventLoop(){
    assert(!looping_);
    t_loopInThisThread = nullptr;
}

void EventLoop::quit() {
    quit_ = true;
}

void EventLoop::updateChannel(Channel* channel) {
    assert(channel->ownerloop()==this);
    Poller_ -> updateChannel(channel);
}

void EventLoop::loop() {
    logDebug("this is loop test");
    initwake();
    assert(!looping_);
    looping_ = true;
    quit_ = false;
    while(!quit_){
        activeChannel_.clear();
        int64_t timeout = TimeEventSet_->getTimeout();
        int n = Poller_->loop_wait(timeout,&activeChannel_);
        if(n>0){
            //auto ends = activeChannel_.end();
            for(auto it=activeChannel_.begin(); it != activeChannel_.end(); it++){
                (*it) -> handleEvent();
            }
        }else if(n==0){
            TimeEventSet_->exeTask();
        }
        doPendingFunctors();
    }

    looping_ = false;
}
void EventLoop::runInLoop(const Functor &cb) {
    //如果是当前IO线程调用，则直接可以执行，即同步执行；
    //如果不是在当前线程调用的runInLoop，则加入任务队列，IO线程会被唤醒执行该回调
    if(isInLoopThread()){
        cb();
    }
    else{
        /*RunInLoop不在当前线程，加入队列稍后执行*/
        queueInLoop(cb);
    }
}

void EventLoop::doPendingFunctors() {
    //避免阻塞，提高效率
    std::vector<Functor> functors;
    //因为调用queueInloop的时候，如果是在functors里调用的话，需要唤醒IO线程，防止阻塞在kqueue上
    callingPendingFunctors_ = true;
    {
        std::lock_guard<std::mutex> mlock(mutex_);
        functors.swap(pendingFunctors_);
    }
    for (size_t i = 0; i < functors.size(); ++i)
    {
        functors[i]();
    }
    callingPendingFunctors_ = false;
}
void EventLoop::initwake() {
    //初始化唤醒事件的fd以及唤醒事件channel
    //SocketOpt::socketpair(wakeupFd);
    //将可读事件作为唤醒事件
    SocketOpt::socketpair(wakeupFd);

    wakeupChannel_ = std::shared_ptr<Channel>(new Channel(this,wakeupFd[0]));
    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleread,this));
    wakeupChannel_->enableReading();;
}
void EventLoop::wakeup() {
    uint64_t one = 1;
    //向wakeupFd[1]写入8个字节，唤醒阻塞线程
    size_t n = ::write(wakeupFd[1],&one, sizeof(one));
    if(n != sizeof(one)){
        std::cout << "EventLoop wakeup is wrong bites: " << sizeof(one) << std::endl;
    }
}

void EventLoop::handleread() {
    uint64_t one;
    size_t n = ::read(wakeupFd[0], &one, sizeof(one));
    if(n != sizeof(one)){
        std::cout << "EventLoop handleread is wrong bites: " << sizeof(one) << std::endl;
    }
}
/**为什么要在这两种情况下有必要唤醒IO线程？
 * Part1：如果调用queueInLoop不是在IO线程中，此时的IO线程正在阻塞在多路复用的调用中，比如epoll，poll此类函数，
 * 正在等待有活跃的事件发生的时候，如果不唤醒IO线程打破阻塞状态，那么程序将不会执行到IO线程的发生事件执行回调函数，
 * 即doPendingFunctors不会执行，也就是functors不会执行。
 * Part2：如果调用queueInLoop是在IO线程中，而此时的IO线程正在执行doPendingFunctors中的functors，而在functors，
 * 中又进行了调用queueInLoop，此时需要再次触发IO线程下一次的唤醒，保证不会阻塞在IO线程上。
 * Part3：只有在Handleevent中调用queueInLoop才不需要被唤醒，因为接下来将会调用doPendingFunctors。
 * @param cb
 */
void EventLoop::queueInLoop(const Functor &cb) {
    {
        //其中，可以在IO线程调用queueInLoop也可以由于Function在其他线程中进行执行
        //所以该函数暴露给了其他线程，再加入任务队列的时候，需要加锁
        std::lock_guard<std::mutex> mlock(mutex_);
        pendingFunctors_.push_back(cb);
    }
    if(!isInLoopThread() || callingPendingFunctors_){
        wakeup();
    }
}

void EventLoop::removeRunInLoop(const std::shared_ptr<Channel> channel_){
    runInLoop([this, channel_]{ this->removeChannelInLoop(channel_); });
}
void EventLoop::removeChannelInLoop(const std::shared_ptr<Channel> channel_){
    channel_->disableAll();
}

size_t EventLoop::runAfter(int64_t timeout, TimeEventCallback TimeEventCb){
    auto howlong = Time::nowTime()+timeout;
    TimeEvent *timer  = new TimeEvent{howlong,0,std::move(TimeEventCb)};
    return TimeEventSet_->addTimeEvent(timer);
}

size_t EventLoop::runAt(TimeEvent *timerAt, TimeEventCallback TimeEventCb){
    timerAt->setTimeInterval(0);
    return TimeEventSet_->addTimeEvent(timerAt);
}

size_t EventLoop::runAferEve(int64_t interval, TimeEventCallback TimeEventCb){
    auto howlong = Time::nowTime()+interval;
    TimeEvent *timer = new TimeEvent{howlong,interval,std::move(TimeEventCb)};
    return TimeEventSet_->addTimeEvent(timer);
}