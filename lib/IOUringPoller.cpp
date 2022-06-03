//
// Created by mooki on 22-5-30.
//

#include "IOUringPoller.h"

#include <iostream>

#include <liburing.h>

#include "Channel.h"

void IOUringPoller::poll(ChannelList &channels) {
    if (io_uring_wait_cqe(ring_, &cqe_) < 0) {
        perror("io_uring_wait_cqe");
    }

    auto info = (struct ConnInfo *)cqe_->user_data;
    Channel *channel = info->channel;
    channel->setConnInfo(info);
    channel->setEventType(info->eventType);
    channel->setCqe(cqe_);
    channels.push_back(channel);
}
