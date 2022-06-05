//
// Created by mooki on 22-6-5.
//

#include "Timer.h"

#include <sys/time.h>

#include "EventLoop.h"

Timer::Timer(EventLoop *loop)
        : timerChannel_(loop, loop->ring()) {}

void Timer::runAt(struct timeval tv, const TimeoutEventCallback &cb)
{
    auto ts = reinterpret_cast<struct __kernel_timespec *>(malloc(sizeof(struct __kernel_timespec)));
    ts->tv_sec = tv.tv_sec;
    ts->tv_nsec = tv.tv_usec * 1000;
    timerChannel_.addTimeout(ts, cb);

    timerChannel_.update();
}

void Timer::runAfter(double secs, const TimeoutEventCallback &cb) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    tv.tv_sec += static_cast<int>(secs);
    tv.tv_usec += static_cast<long>((secs - static_cast<int>(secs)) * 1000 * 1000);

    runAt(tv, cb);
}