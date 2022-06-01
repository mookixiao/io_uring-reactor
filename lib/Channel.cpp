//
// Created by mooki on 22-5-30.
//

#include <cstring>

#include "Channel.h"
#include "EventLoop.h"
#include "Limits.h"

void Channel::handleEvent() {
    switch (eventType) {
        case EVENT_ACCEPT:  // 监听套接字
            acceptCallback_();
            break;
        case EVENT_READ:  // 连接套接字
            readCallback_(cqe_);
            break;
        case EVENT_WRITE:  // 连接套接字
            writeCallback_();
            break;
    }
}

void Channel::enableListen()
{
    // 将io_uring监听请求提交
    io_uring_submit(ring_);
}

void Channel::enableReading()
{
    sqe_ = io_uring_get_sqe(ring_);

    io_uring_prep_recv(sqe_, sockfd_, nullptr, MAX_MESSAGE_LEN, 0);
    sqe_->buf_group = BGID;
    auto *req = (struct ConnInfo *)malloc(sizeof(struct ConnInfo));
    req->channel = this;
    req->clientSocket = sockfd_;
    req->eventType = EVENT_READ;
    io_uring_sqe_set_data(sqe_, req);

    io_uring_submit(ring_);
}

void Channel::enableWriting(int numBytes)
{
    sqe_ = io_uring_get_sqe(ring_);

    io_uring_prep_send(sqe_, sockfd_, bufs[], numBytes, 0);
    io_uring_sqe_set_flags(sqe, flags);
    auto *req = (struct ConnInfo *)malloc(sizeof(struct ConnInfo));
    io_uring_sqe_set_data(sqe_, req);

    io_uring_submit(ring_);
}