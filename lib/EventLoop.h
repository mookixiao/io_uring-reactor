//
// Created by mooki on 22-5-30.
//

#ifndef IO_URING_REACTOR_EVENTLOOP_H
#define IO_URING_REACTOR_EVENTLOOP_H

#include <memory>

#include "Types.h"

class Channel;
class IOUringPoller;

class EventLoop {
public:
    EventLoop();
    ~EventLoop();

    void poll();

    void updateChannel(Channel *channel);

private:
    // IOUringPoller
    std::shared_ptr<IOUringPoller> poller_;
    ChannelList activeChannels_;
};


#endif //IO_URING_REACTOR_EVENTLOOP_H
