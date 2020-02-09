//
// Created by Jialu  Hu on 2020-02-09.
//
#include "Log.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include <sys/time.h>
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

void log::addwritebuff(const char *str,size_t len) {
    std::lock_guard<std::mutex> lock(Mutex_);
    writebuffer_.append(str,len);
    if(writebuffer_.readable() >= 10){
        CondVar_.notify_one();
    }
}

void log::writefile() {
    int ret = ::write(fd_, flushbuffer_.peek(), flushbuffer_.readable());
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
    time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm * ptminfo = localtime(&tt);
    char dates[60] = { 0 };
    sprintf(dates, "%02d-%02d-%02d %02d:%02d:%02d",
            ptminfo->tm_year + 1900, ptminfo->tm_mon + 1,
            ptminfo->tm_mday, ptminfo->tm_hour,
            ptminfo->tm_min, ptminfo->tm_sec);
    char level_[10];
    if(level == log::debug){
        sprintf(level_,"debug");
    }else if(level == log::errnos){
        sprintf(level_,"errnos");
    }else if(level == log::fatal){
        sprintf(level_,"fatal");
    }
    char *function = new char[strlen(func)];
    sprintf(function,"%s",func);
    char *filePath = new char[strlen(file)];
    sprintf(filePath,"%s",file);
    char buf[4096];
    sprintf(buf,"[Level]%s  [Time] %s  [Function] %s  [FilePath] %s [Line] %d  [Content] ",
            level_,dates,function,filePath,line);
    int len = strlen(buf);
    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(buf+len,(4094-len),fmt,args);
    sprintf(buf+len+n,"\n");
    buf[strlen(buf+1)] = '\0';
    std::cout << "buf: ";
    std::cout << buf << std::endl;
    delete [] function;
    delete [] filePath;
    addwritebuff(buf,strlen(buf));
}