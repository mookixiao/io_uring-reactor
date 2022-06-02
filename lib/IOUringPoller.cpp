//
// Created by mooki on 22-5-30.
//

#include "IOUringPoller.h"

#include <liburing.h>

#include "Channel.h"

IOUringPoller::~IOUringPoller() {

}

void IOUringPoller::poll(ChannelList &channels) {
    if (io_uring_wait_cqe(ring_, &nowCqe_) < 0) {
        perror("io_uring_wait_cqe");
    }

    auto info = (struct ConnInfo *)nowCqe_->user_data;
    Channel *channel = info->channel;
    channel->setCqe(nowCqe_);
    channel->setConnInfo(info);
    channel->setEventType(info->eventType);
    channels.push_back(channel);
}
