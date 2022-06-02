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
        case EVENT_ACCEPT_COMPLETE:  // 监听套接字
            acceptCompleteCallback_();
            break;
        case EVENT_READ_COMPLETE:  // 连接套接字
            readCallback_();
            break;
        case EVENT_WRITE_COMPLETE:  // 连接套接字
            writeCallback_();
            break;
        case EVENT_SER_BUF_COMPLETE:
            if ( < 0) {
                perror("EVENT_SER_BUF_COMPLETE");
            }
    }
}

void Channel::addListen(struct sockaddr_in &peerAddr)  // 针对监听套接字
{
    sqe_ = io_uring_get_sqe(ring_);

    socklen_t peerAddrLen = sizeof peerAddr;
    io_uring_prep_accept(sqe_, sockfd_, (struct sockaddr *)&peerAddr, &peerAddrLen, 0);
    auto *info = (struct ConnInfo *)malloc(sizeof(struct ConnInfo));
    info->channel = this;
    info->eventType = EVENT_ACCEPT_COMPLETE;
    info->sockfd = sockfd_;
    io_uring_sqe_set_data(sqe_, info);

    io_uring_submit(ring_);
}

void Channel::addRead()
{
    sqe_ = io_uring_get_sqe(ring_);

    io_uring_prep_recv(sqe_, sockfd_, nullptr, MAX_MESSAGE_LEN, 0);
    sqe_->buf_group = BGID;
    auto *req = (struct ConnInfo *)malloc(sizeof(struct ConnInfo));
    req->channel = this;
    req->sockfd = sockfd_;
    req->eventType = EVENT_READ_COMPLETE;
    io_uring_sqe_set_data(sqe_, req);

    io_uring_submit(ring_);
}

void Channel::addWrite()
{

}