//
// Created by mooki on 22-5-30.
//

#include "Channel.h"

#include <arpa/inet.h>

#include "Limits.h"

// Debug
#include <iostream>

extern char bufs[][MAX_MESSAGE_LEN];

void Channel::handleEvent() {
    auto connInfo = reinterpret_cast<struct ConnInfo *>(cqe_->user_data);

    if (connInfo->eventType == EVENT_ACCEPT) {
        acceptCallback_();
    }
    else if (connInfo->eventType == EVENT_READ) {
        bId_ = cqe_->flags >> 16;
        readCallback_();
    }
    else if (connInfo->eventType == EVENT_WRITE) {
        bId_ = cqe_->flags >> 16;
        writeCallback_();
    }
    else if (connInfo->eventType == EVENT_BUF) {
        if ( cqe_->res < 0) {
            perror("handleEvent: EVENT_BUF");
        }
    }
    else {
        perror("handleEvent: Unknown Event Type");
    }

    io_uring_cqe_seen(ring_, cqe_);
}

void Channel::addListen(struct sockaddr_in &peerAddr, socklen_t *peerAddrLen)  // 针对监听套接字
{
    sqe_ = io_uring_get_sqe(ring_);
    io_uring_prep_accept(sqe_, fd_, (struct sockaddr *)&peerAddr, peerAddrLen, 0);
    fillConnInfo(&sqe_->user_data, EVENT_ACCEPT);
    io_uring_submit(ring_);
}

void Channel::addRead()  // TODO: 使用固定buffer不够灵活
{
    sqe_ = io_uring_get_sqe(ring_);
    io_uring_prep_recv(sqe_, fd_, nullptr, MAX_MESSAGE_LEN, 0);
    io_uring_sqe_set_flags(sqe_, IOSQE_BUFFER_SELECT);
    sqe_->buf_group = BGID;
    fillConnInfo(&sqe_->user_data, EVENT_READ);
    io_uring_submit(ring_);
}

void Channel::addWrite(char *buf, int size)
{
    sqe_ = io_uring_get_sqe(ring_);
    io_uring_prep_send(sqe_, fd_, buf, size, 0);
    fillConnInfo(&sqe_->user_data, EVENT_WRITE);
    io_uring_submit(ring_);
}

void Channel::addBuffer()  // TODO: 使用固定buffer不够灵活
{
    sqe_ = io_uring_get_sqe(ring_);
    io_uring_prep_provide_buffers(sqe_, bufs[bId_], MAX_MESSAGE_LEN, 1, BGID, bId_);
    fillConnInfo(&sqe_->user_data, EVENT_BUF);
    io_uring_submit(ring_);
}

void Channel::fillConnInfo(unsigned long long int *info, EventType type) const
{
    auto *connInfo = reinterpret_cast<struct ConnInfo *>(info);
    connInfo->fd = fd_;
    connInfo->eventType = type;
    connInfo->bId = bId_;
}