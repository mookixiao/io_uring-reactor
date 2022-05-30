//
// Created by mooki on 22-5-30.
//

#ifndef IO_URING_REACTOR_EVENTLOOP_H
#define IO_URING_REACTOR_EVENTLOOP_H

#include <memory>

#include "Channel.h"
#include "Types.h"

class IOUringPoller;

class EventLoop {
public:
    EventLoop();
    ~EventLoop();

    void poll();

private:
    // IOUringPoller
    std::shared_ptr<IOUringPoller> poller_;
    ChannelList activeChannels_;
};


#endif //IO_URING_REACTOR_EVENTLOOP_H
