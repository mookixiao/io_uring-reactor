//
// Created by mooki on 22-5-30.
//

#include "IOUringPoller.h"

#include <liburing.h>

#include "Channel.h"

IOUringPoller::IOUringPoller() {

}

IOUringPoller::~IOUringPoller() {

}

void IOUringPoller::poll(ChannelList &activeChannel) {
    io_uring_submit_and_wait(&ring, 1);

    int head;
    int cnt = 0;
    struct io_uring_cqe *cqe;
    io_uring_for_each_cqe(&ring, head, cqe) {
        ++cnt;
        auto req = (struct Request *)cqe->user_data;
        activeChannel.push_back(req->channel);
}

void IOUringPoller::updateChannel(Channel *channel) {
    Status status = channel->status();
    if (status == STAT_NEW) {

    }
}
