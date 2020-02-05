//
// Created by Jialu  Hu on 2020-01-29.
//

#include <sys/time.h>
#include "NowTime.h"
using namespace SiNet;
int64_t Time::nowTime()
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
