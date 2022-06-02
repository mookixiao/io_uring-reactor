//
// Created by mooki on 22-5-30.
//

#include "IOUringPoller.h"

#include <liburing.h>

#include "Channel.h"

IOUringPoller::~IOUringPoller() {

}

void IOUringPoller::poll(ChannelList &channels) {
    if (io_uring_wait_cqe(ring_, &cqe_) < 0) {
        perror("io_uring_wait_cqe");
    }

    auto req = (struct ConnInfo *)cqe_->user_data;
    Channel *channel = req->channel;
    channel->setRes(cqe_->res);  // 设置返回值
    channel->setEventType(req->eventType);  // 设置事件类型
    channels.push_back(channel);  // 交由EventLoop处理
}
