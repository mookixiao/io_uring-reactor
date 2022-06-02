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
    IOUringPoller(EventLoop *loop, struct io_uring *ring) : ownerLoop_(loop), ring_(ring) {}
    ~IOUringPoller();

    void poll(ChannelList& channels);

private:
    EventLoop* ownerLoop_;
    struct io_uring* ring_;
    struct io_uring_cqe *cqe_;
};


#endif //IO_URING_REACTOR_IOURINGPOLLER_H
