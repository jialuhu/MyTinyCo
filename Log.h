//
// Created by Jialu  Hu on 2020-02-09.
//

#ifndef MYSNETCO_LOG_H
#define MYSNETCO_LOG_H

#include <thread>
#include <functional>
#include <condition_variable>
#include "Noncopyable.h"
#include "Buffer.h"

namespace SiNet {
    class log : private Noncopyable {
    public:
        enum LogLevel { debug = 0, info, warn, errnos,fatal};

        log();
        ~log();
        void start();
        void threadFun();
        void outputcontent(int level, const char *file, int line,
                const char *func, const char* date,const char *fmt, ...);
        void addwritebuff(const char *str, size_t len);
        void writefile();
        void createfile();
        LogLevel getloglevel(){
            return loglevel_;
        }
        void setLevel(SiNet::log::LogLevel level);
        static log &getLogger();
        static void stop();
        void wakeup();
    private:

        int writebyte;
        int fd_;
        LogLevel loglevel_;
        std::string filepath_;
        Buffer writebuffer_;
        Buffer flushbuffer_;
        std::thread thread_;
        std::mutex Mutex_;
        std::condition_variable CondVar_;
    public:
        static const int maxbuffsize_ = 1024 * 512;
        static const int maxfilesize = 1024*1024;
    };
}

#define logDebug(...) \
    if (SiNet::log::debug >= SiNet::log::getLogger().getloglevel()) \
        SiNet::log::getLogger().outputcontent(SiNet::log::debug, __FILE__, __LINE__, __func__, __DATE__,__VA_ARGS__)
#define logInfo(...) \
    if (SiNet::log::info >= SiNet::log::getLogger().getloglevel()) \
        SiNet::log::getLogger().outputcontent(SiNet::log::debug, __FILE__, __LINE__, __func__, __DATE__,__VA_ARGS__)
#define logWarn(...) \
    if (SiNet::log::warn >= SiNet::log::getLogger().getloglevel()) \
        SiNet::log::getLogger().outputcontent(SiNet::log::debug, __FILE__, __LINE__, __func__, __DATE__,__VA_ARGS__)
#define logErrnos(...) \
    if (SiNet::log::errnos >= SiNet::log::getLogger().getloglevel()) \
        SiNet::log::getLogger().outputcontent(SiNet::log::debug, __FILE__, __LINE__, __func__, __DATE__,__VA_ARGS__)
#define logFatal(...) \
    if (SiNet::log::fatal >= SiNet::log::getLogger().getloglevel()) \
        SiNet::log::getLogger().outputcontent(SiNet::log::debug, __FILE__, __LINE__, __func__, __DATE__,__VA_ARGS__)
#endif //MYSNETCO_LOG_H
