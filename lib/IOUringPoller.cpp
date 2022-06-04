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

    uint32_t fd = reinterpret_cast<struct ConnInfo *>(&cqe_->user_data)->fd;
    Channel *channel = channels_[fd];
    channel->setCqe(cqe_);
    channels.push_back(channel);
}

void IOUringPoller::updateChannel(Channel *channel) {
    channels_[channel->getFd()] = channel;
}