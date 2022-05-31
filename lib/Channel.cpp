//
// Created by mooki on 22-5-30.
//

#include "Channel.h"
#include "EventLoop.h"

void Channel::handleEvent()
{
    switch (eventType) {
        case EVENT_ACCEPT:  // 监听套接字
            acceptCallback_();
            break;
        case EVENT_READ:  // 连接套接字
            readCallback_();
            break;
        case EVENT_WRITE:  // 连接套接字
            writeCallback_();
            break;
    }
}

void Channel::update() {
    ownerLoop_->updateChannel(this);
}
