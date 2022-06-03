//
// Created by mooki on 22-5-30.
//

#include "Channel.h"

#include <cstring>

#include <arpa/inet.h>

#include "EventLoop.h"
#include "Limits.h"

#include <iostream>

extern char bufs[][MAX_MESSAGE_LEN];

Channel::Channel(int sockfd, struct io_uring *ring)
        : fd_(sockfd),
        ring_(ring), sqe_(nullptr), cqe_(nullptr),
        info_((struct ConnInfo *) malloc(sizeof(struct ConnInfo))),
        eventType_(EVENT_NONE)
{
    info_->channel = this;
}

Channel::~Channel()
{
    delete info_;
}

void Channel::handleEvent() {
    switch (eventType_) {
        case EVENT_ACCEPT:  // 针对监听套接字
            acceptCallback_();
            break;
        case EVENT_READ:
            bId_ = cqe_->flags >> 16;
            readCallback_();
            break;
        case EVENT_WRITE:
            bId_ = cqe_->flags >> 16;
            writeCallback_();
            break;
        case EVENT_BUF:
            if ( cqe_->res < 0) {
                perror("handleEvent: EVENT_BUF");
            }
            break;
        case EVENT_NONE:
            perror("handleEvent: EVENT_NONE");
            break;
    }
}

void Channel::addListen(struct sockaddr_in &peerAddr, socklen_t *peerAddrLen)  // 针对监听套接字
{
    info_->eventType = EVENT_ACCEPT;

    sqe_ = io_uring_get_sqe(ring_);

    io_uring_prep_accept(sqe_, fd_, (struct sockaddr *)&peerAddr, peerAddrLen, 0);
    io_uring_sqe_set_data(sqe_, info_);

    io_uring_submit(ring_);
}

void Channel::addRead()  // TODO: 使用固定buffer不够灵活
{
    info_->eventType = EVENT_READ;

    sqe_ = io_uring_get_sqe(ring_);

    io_uring_prep_recv(sqe_, fd_, nullptr, MAX_MESSAGE_LEN, 0);
    io_uring_sqe_set_flags(sqe_, IOSQE_BUFFER_SELECT);
    sqe_->buf_group = BGID;
    io_uring_sqe_set_data(sqe_, info_);

    io_uring_submit(ring_);
}

void Channel::addWrite(char *buf, int size)
{
    info_->eventType = EVENT_WRITE;

    sqe_ = io_uring_get_sqe(ring_);

    io_uring_prep_send(sqe_, fd_, buf, size, 0);
    io_uring_sqe_set_data(sqe_, info_);

    io_uring_submit(ring_);
}

void Channel::addBuffer()  // TODO: 使用固定buffer不够灵活
{
    info_->eventType = EVENT_BUF;

    sqe_ = io_uring_get_sqe(ring_);

    io_uring_prep_provide_buffers(sqe_, bufs[bId_], MAX_MESSAGE_LEN, 1, BGID, bId_);

    io_uring_submit(ring_);
}