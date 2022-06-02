//
// Created by mooki on 22-5-30.
//

#include <cstring>

#include <arpa//inet.h>

#include "Channel.h"
#include "EventLoop.h"
#include "Limits.h"

void Channel::handleEvent() {
    switch (eventType_) {
        case EVENT_ACCEPT:  // 监听套接字
            acceptCompleteCallback_();
            break;
        case EVENT_READ:  // 连接套接字
            readCallback_();
            break;
        case EVENT_WRITE:  // 连接套接字
            writeCallback_();
            break;
        case EVENT_SER_BUF_COMPLETE:
            if ( cqe_->res < 0) {
                perror("EVENT_SER_BUF_COMPLETE");
            }
    }
}

void Channel::addListen(struct sockaddr_in &peerAddr)  // 针对监听套接字
{
    sqe_ = io_uring_get_sqe(ring_);

    socklen_t peerAddrLen = sizeof peerAddr;
    io_uring_prep_accept(sqe_, fd_, (struct sockaddr *)&peerAddr, &peerAddrLen, 0);
    auto *info = (struct ConnInfo *)malloc(sizeof(struct ConnInfo));
    info->channel = this;
    info->eventType = EVENT_ACCEPT;
    info->sockfd = fd_;
    io_uring_sqe_set_data(sqe_, info);

    io_uring_submit(ring_);
}

void Channel::addRead()
{
    sqe_ = io_uring_get_sqe(ring_);

    io_uring_prep_recv(sqe_, fd_, nullptr, MAX_MESSAGE_LEN, 0);
    sqe_->buf_group = BGID;
    auto *req = (struct ConnInfo *)malloc(sizeof(struct ConnInfo));
    req->channel = this;
    req->sockfd = fd_;
    req->eventType = EVENT_READ;
    io_uring_sqe_set_data(sqe_, req);

    io_uring_submit(ring_);
}

void Channel::addWrite(char *buf, int size)
{
    sqe_ = io_uring_get_sqe(ring_);

    io_uring_prep_send(sqe_, fd_, buf, size, 0);
    info_->eventType = EVENT_WRITE;
    io_uring_sqe_set_data(sqe_, info_);

    io_uring_submit(ring_);
}

void Channel::addBuffer(char *buf, int size)
{
    sqe_ = io_uring_get_sqe(ring_);
    io_uring_prep_provide_buffers(sqe_, bufs, MAX_MESSAGE_LEN, MAX_CONNECTIONS, BGID, 0);
    io_uring_submit(ring_);

    io_uring_wait_cqe(ring_, &cqe_);
    if (cqe_->res < 0) {
        perror("io_uring_wait_cqe");
    }
    io_uring_cqe_seen(ring_, cqe_);
}