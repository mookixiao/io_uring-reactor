//
// Created by mooki on 22-5-30.
//

#include "Channel.h"

#include <arpa/inet.h>

#include "EventLoop.h"
#include "Limits.h"

extern char bufs[][MAX_MESSAGE_LEN];

void Channel::handleEvent() {
    auto eventInfo = reinterpret_cast<struct ConnInfo *>(&cqe_->user_data);

    if (eventInfo->eventType == EVENT_ACCEPT) {
        acceptCallback_();
    }
    else if (eventInfo->eventType == EVENT_READ) {
        bId_ = cqe_->flags >> 16;
        readCallback_();
    }
    else if (eventInfo->eventType == EVENT_WRITE) {
        bId_ = cqe_->flags >> 16;
        writeCallback_();
    }
    else if (eventInfo->eventType == EVENT_BUF) {
        if ( cqe_->res < 0) {
            perror("handleEvent: EVENT_BUF");
        }
    }
    else if (eventInfo->eventType == EVENT_TIMEOUT) {
        timeoutCallback_();
    }
    else {
        perror("handleEvent: Unknown Event Type");
    }

    io_uring_cqe_seen(ring_, cqe_);
}

// TODO: 以下逻辑移到Acceptor更好
void Channel::addListen(struct sockaddr_in &peerAddr, socklen_t *peerAddrLen)  // 针对监听套接字
{
    sqe_ = io_uring_get_sqe(ring_);
    io_uring_prep_accept(sqe_, fd_, (struct sockaddr *)&peerAddr, peerAddrLen, 0);
    fillConnInfo(&sqe_->user_data, EVENT_ACCEPT);
    io_uring_submit(ring_);
}

// TODO: 以下逻辑移到TcpConnection更好
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

void Channel::addTimeout(struct __kernel_timespec *time, const TimeoutEventCallback &cb)
{
    setTimeoutCallback(cb);

    sqe_ = io_uring_get_sqe(ring_);
    io_uring_prep_timeout(sqe_, time, 1, IORING_TIMEOUT_ABS);
    fillConnInfo(&sqe_->user_data, EVENT_TIMEOUT);
    io_uring_submit(ring_);
}

void Channel::fillConnInfo(unsigned long long *info, EventType type) const
{
    auto *connInfo = reinterpret_cast<struct ConnInfo *>(info);
    connInfo->fd = fd_;
    connInfo->eventType = type;
    connInfo->bId = bId_;
}

void Channel::update() {
    loop_->updateChannel(this);
}
