//
// Created by Jialu  Hu on 2020-02-09.
//
#include "Log.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
using namespace SiNet;
log::log():fd_(-1),
            writebyte(0),
            thread_([this]{this->threadFun();}),
            loglevel_(log::debug){
    start();
}
log::~log() {
    ::close(fd_);
    thread_.join();
}

void log::start() {
    mkdir("./.log",0777);
    createfile();
}

void log::createfile() {
    std::cout << "crete the file\n";
    filepath_.clear();
    filepath_ = filepath_+"./.log/";
    time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm * ptminfo = localtime(&tt);
    char date[60] = { 0 };
    sprintf(date, "%02d-%02d-%02d %02d:%02d:%02d",
            ptminfo->tm_year + 1900, ptminfo->tm_mon + 1,
            ptminfo->tm_mday, ptminfo->tm_hour,
            ptminfo->tm_min, ptminfo->tm_sec);
    filepath_ += date;
    filepath_ +=".log";
    std::cout << "filepath: " << filepath_ << std::endl;
    int fd = open(filepath_.c_str(), O_RDWR|O_APPEND|O_CREAT,0664);
    if(fd < 0 ){

    }
    fd_ = fd;
    writebyte = 0;
}
log &log::getLogger() {
    static log logger;
    return logger;
}
void log::threadFun() {
    while(true){
        {
            std::unique_lock<std::mutex> lock(Mutex_);
            CondVar_.wait(lock);
            if(writebuffer_.readable()>0){
                writebuffer_.swap(flushbuffer_);
            }
            if(flushbuffer_.readable()>0){
                writefile();
            }
        }
    }
}

void log::addwritebuff(std::string &content) {
    int len = content.size();
    std::lock_guard<std::mutex> lock(Mutex_);
    writebuffer_.append(content.c_str(),len);
    if(writebuffer_.readable() >= 10){
        CondVar_.notify_one();
    }
}

void log::writefile() {
    std::cout << "fd is " << fd_ << " flush is " << flushbuffer_.c_str() << std:: endl;
    char buf[256]={"sdfsdfsfdjlfjsd"};
    //int ret = ::write(fd_, buf, strlen(buf));
    int ret = ::write(fd_, flushbuffer_.peek(), flushbuffer_.readable());
    std::cout << "fd is " << fd_ << " ret is " << ret << std:: endl;
    writebyte += flushbuffer_.readable();
    flushbuffer_.retrieve(flushbuffer_.readable());
    if(writebyte == maxfilesize){
        ::close(fd_);
        createfile();
    }
}

void log::outputcontent(int level, const char *file,
        int line, const char *func,
        const char *date, const char *fmt,
                        ...){
    std::cout << "level:" << level << "  file:" << file << "  line:" << line << " date:"
    << date << "  fmt:" << fmt << std:: endl;
    std::string str{""};
    if(level==log::debug){
        str += "[debug]: ";
    }
    str += date;
    str += "  ";
    str += func;
    str += "  ";
    str += '['+fmt+']';
    str += "  ";
    getLogger().addwritebuff(str);
}