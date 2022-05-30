//
// Created by mooki on 22-5-30.
//

#include "EventLoop.h"

#include "IOUringPoller.h"

EventLoop::EventLoop()
        : poller_(new IOUringPoller)
{
}

void EventLoop::poll()
{
    while(true) {
        activeChannels_.clear();
        poller_->poll(activeChannels_);
    }
}