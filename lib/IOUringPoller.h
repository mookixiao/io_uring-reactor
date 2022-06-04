//
// Created by mooki on 22-5-30.
//

#ifndef IO_URING_REACTOR_IOURINGPOLLER_H
#define IO_URING_REACTOR_IOURINGPOLLER_H

#include <liburing.h>

#include "Types.h"

class Channel;
class EventLoop;

class IOUringPoller {
public:
    IOUringPoller(EventLoop *loop, struct io_uring *ring) : ownerLoop_(loop), ring_(ring), cqe_(nullptr) {}

    void poll(ChannelList& channels);
    void updateChannel(Channel *channel);

private:
    EventLoop* ownerLoop_;
    ChannelMap channels_;  // 建立fd和Channel *的映射关系

    struct io_uring* ring_;
    struct io_uring_cqe *cqe_;
};


#endif //IO_URING_REACTOR_IOURINGPOLLER_H
