//
// Created by mooki on 22-6-5.
//

#ifndef IO_URING_REACTOR_TIMER_H
#define IO_URING_REACTOR_TIMER_H

#include <liburing.h>

#include "Channel.h"
#include "Types.h"

class EventLoop;

class Timer {
public:
    Timer(EventLoop *loop);

    void setTimeoutCallback(TimeoutEventCallback cb) { timerEventCallback_ = cb; };

    void runAt(struct timeval tv, const TimeoutEventCallback &cb);
    void runAfter(double secs, const TimeoutEventCallback &cb);

private:
    TimeoutEventCallback timerEventCallback_;

    Channel timerChannel_;

    EventLoop *loop_;
    struct io_uring *ring_;
    struct io_uring_sqe *sqe_;
};


#endif //IO_URING_REACTOR_TIMER_H
