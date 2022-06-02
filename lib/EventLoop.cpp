//
// Created by mooki on 22-5-30.
//

#include "Channel.h"
#include "EventLoop.h"
#include "Limits.h"

#include "IOUringPoller.h"

EventLoop::EventLoop()
{
    if (io_uring_queue_init(MAX_CONNECTIONS, &ring_, 0) < 0) {
        perror("io_uring_queue_init");
    }

    poller_ = std::make_shared<IOUringPoller>(this, &ring_);
}

EventLoop::~EventLoop()
{
    io_uring_queue_exit(&ring_);
}

void EventLoop::loop()
{
    while(true) {
        activeChannels_.clear();
        poller_->poll(activeChannels_);
        for (auto it = activeChannels_.begin(); it != activeChannels_.end(); ++it) {
            (*it)->handleEvent();
        }
    }
}