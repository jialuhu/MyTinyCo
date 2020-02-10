//
// Created by Jialu  Hu on 2019-08-02.
//
#ifdef OS_LINUX
#include "Epoll.h"
#include "Channel.h"
using namespace SiNet;
const int MAXSIZE = 1024;
Epoll::Epoll():epfd_(){
    epfd_ = epoll_create(1);

    EpList_.resize(MAXSIZE);
}

Epoll::~Epoll() {
    ::close(epfd_);
}
void Epoll::fillactiveChannel(int number, Channel* channel){

}
int Epoll::loop_wait(int timeout, std::vector<Channel*>* activeChannel){
    int number = epoll_wait(epfd_, &EpList_[0], EpList_.size(), timeout);
    if(number>0) {
        for (int i = 0; i < number; i++) {
            int socketfd = EpList_[i].data.fd;
            int event = EpList_[i].events;
            assert(channels_.find(socketfd) != channels_.end());
            auto actChannel = channels_[socketfd];
            assert(actChannel->fd() == socketfd);
            actChannel->set_revent(ReturnEvent(event));
            activeChannel->push_back(actChannel);
        }
    }
    return number;
}

bool Epoll::Register(Channel* channel,int fd) {
    struct epoll_event ev[1];
    if(channel->event() & Channel::EREAD){
        ev[0].events |= EPOLLIN;
    }
    if(channel->event() & Channel::EWRITE){
        ev[0].events |= EPOLLOUT;
    }
    ev[0].data.fd = fd;
    if (epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev[0]) < 0){
        std::cout << "EPOLL_CTL_ADD is wrong." << std::endl;
        return false;
    }
    EpList_.push_back(ev[0]);
    int idx = static_cast<int>(EpList_.size())-1;
    channel->set_index(idx);
    channels_[fd] = channel;
    return true;
}
bool Epoll::Change(Channel* channel,int fd){
    struct epoll_event changes[1];
    if(channel->is_close()){
        if(channel->index() == (EpList_.size()-1))
        {
            //删除vector尾元素
            EpList_.pop_back();
        }
        else{
            int tmp_fd = EpList_[EpList_.size()-1].data.fd;
            int tmp_index = channel->index();
            iter_swap(EpList_.begin()+channel->index(), EpList_.end()-1);
            EpList_.pop_back();
            channels_[tmp_fd]->set_index(tmp_index);
        }
        channels_.erase(channel->fd());
        if (epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, NULL) < 0){
            std::cout << "EPOLL_CTL_DEL is wrong." << std::endl;
            return false;
        }
    }else {
        int chfd = -1;
        chfd = channel->fd();
        if (channel->event() & Channel::EREAD) {
            changes[0].events |= EPOLLIN;
            changes[0].data.fd = chfd;
        }
        if (channel->event() & Channel::EWRITE) {
            changes[0].events |= EPOLLOUT;
            changes[0].data.fd = chfd;
        }
        if (epoll_ctl(epfd_, EPOLL_CTL_MOD, chfd, &changes[0]) < 0){
            std::cout << "EPOLL_CTL_MOD is wrong." << std::endl;
            return false;
        }
    }
    return true;
}
void Epoll::updateChannel(Channel* channel) {
    int idx = channel->index();
    int kfd = channel -> fd();
    if(idx<0){
        assert(channels_.find(kfd) == channels_.end());
        //该事件还未出现在事件注册表中,如新建立的链接
        //对该事件进行注册
        assert(Register(channel,epfd_)==true);
    }
        //更新事件表
    else{
        assert(channels_.find(kfd)!=channels_.end());
        assert(channels_[kfd]==channel);
        assert(idx >= 0 && idx < EpList_.size());
        //更改事件
        assert(Change(channel,epfd_)==true);
    }
}
int Epoll::ReturnEvent(int &event) {
    int res = 0;
    if(event & EPOLLIN){
        res |= Channel::EREAD;
    }
    if(event & EPOLLOUT){
        res |= Channel::EWRITE;
    }
    if(event & EPOLLERR){
        res |= Channel::EERROR;
    }
    return res;
}
#elif defined(OS_MACOSX)
#include "Kqueue.h"
#include "Log.h"
using namespace SiNet;
Kqueue::Kqueue():kqfd(::kqueue()){
    assert(kqfd>0);
}

Kqueue::~Kqueue(){
    ::close(kqfd);
}
bool Kqueue::Register(Channel* channel,int fd) {
    struct kevent changes[1];
    EV_SET(&changes[0], fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kqfds_.push_back(changes[0]);
    int idx = static_cast<int>(kqfds_.size())-1;
    channel->set_index(idx);
    channels_[fd] = channel;
    //Register 将 fd 注册到 kq 中
    // 注册的方法是通过 kevent() 将 eventlist 和 neventlist 置成 NULL 和 0 来达到的
    int ret = kevent(kqfd, changes, 1, nullptr, 0, nullptr);
    if (ret == -1){
        return false;
    }
    return true;
}

bool Kqueue::Change(Channel* channel,int fd){
    struct kevent changes[1];
    if(channel->is_close()){
        if(channel->index() == (kqfds_.size()-1))
        {
            //删除vector尾元素
            kqfds_.pop_back();
        }
        else{
            int tmp_fd = kqfds_[kqfds_.size()-1].ident;
            int tmp_index = channel->index();
            iter_swap(kqfds_.begin()+channel->index(), kqfds_.end()-1);
            kqfds_.pop_back();
            channels_[tmp_fd]->set_index(tmp_index);
        }
        channels_.erase(channel->fd());
        //对kqueue有困惑
        EV_SET(&changes[0], fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
        EV_SET(&changes[0], fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    }else {
        if (channel->event() & Channel::EREAD) {
            EV_SET(&changes[0], fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
        }
        if (channel->event() & Channel::EWRITE) {
            EV_SET(&changes[0], fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
        }
        int ret = kevent(kqfd, changes, 1, nullptr, 0, nullptr);
        if (ret == -1) {
            logFatal("Kqueue::Change is wrong of kevent");
            return false;
        }
    }
    return true;
}

void Kqueue::updateChannel(Channel* channel) {
    int idx = channel->index();
    int kfd = channel -> fd();
    if(idx<0){
        assert(channels_.find(kfd) == channels_.end());
        //该事件还未出现在事件注册表中,如新建立的链接
        //对该事件进行注册
        assert(Register(channel,kfd)==true);
    }
        //更新事件表
    else{
        assert(channels_.find(kfd)!=channels_.end());
        assert(channels_[kfd]==channel);
        assert(idx>=0 && idx<kqfds_.size());
        assert(Change(channel,kfd)==true);
    }
}
const int MAX_EVENT_COUNT = 5000;

int Kqueue::ReturnEvent(int &ev) {
    int rets = 0;
    if(ev == EVFILT_READ){
        rets = Channel::EREAD;
    }
    else if(ev == EVFILT_WRITE){
        rets = Channel::EWRITE;

    }
    return rets;
}

int Kqueue::loop_wait(int timeout, std::vector<Channel*>* activeChannel){
    for(auto it=channels_.begin();it!=channels_.end();it++){
        int id = it->first;
    }
    struct timespec t;
    if(timeout>0){
        t.tv_sec = timeout / 1000;
        t.tv_nsec = (timeout % 1000) * 1000 * 1000;
    }
    struct kevent krevents[MAX_EVENT_COUNT];
    int ret = ::kevent(kqfd, nullptr, 0, krevents, MAX_EVENT_COUNT, timeout >= 0 ? &t : nullptr);
    if(ret>0) {
        for (int i=0; i<ret; i++) {
            int socketfd = krevents[i].ident;
            int setrv = krevents[i].filter;
            assert(channels_.find(socketfd) != channels_.end());
            auto actChannel = channels_[socketfd];
            assert(actChannel->fd() == socketfd);
            actChannel->set_revent(ReturnEvent(setrv));
            actChannel->set_flags(krevents[i].flags);
            activeChannel->push_back(actChannel);
        }
    }
    else if(ret==0){
        //std::cout << "Nothing happend\n";
    }
    else{
        //std::cout << "Kqueue::kqueue\n";
    }
    return ret;
}
#endif

