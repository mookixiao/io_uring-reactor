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
    IOUringPoller();
    ~IOUringPoller();

    void poll(ChannelList& activeChannels);

    void updateChannel(Channel *channel);

private:

    EventLoop* ownerLoop_;

    struct io_uring ring;
};


#endif //IO_URING_REACTOR_IOURINGPOLLER_H
