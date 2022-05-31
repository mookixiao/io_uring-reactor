//
// Created by mooki on 22-5-30.
//

#include "Channel.h"
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
        for (auto it = activeChannels_.begin(); it != activeChannels_.end(); ++it) {
            (*it)->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel *channel) {
    poller_->updateChannel(channel);
}