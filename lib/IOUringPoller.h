//
// Created by mooki on 22-5-30.
//

#ifndef IO_URING_REACTOR_IOURINGPOLLER_H
#define IO_URING_REACTOR_IOURINGPOLLER_H

#include <liburing.h>

#include "Types.h"

class EventLoop;

class IOUringPoller {
public:
    IOUringPoller();
    ~IOUringPoller();

    void poll(ChannelList& activeChannels);

private:

    EventLoop* ownerLoop_;

    struct io_uring ring;
    struct io_uring_cqe* cqe;
};


#endif //IO_URING_REACTOR_IOURINGPOLLER_H
