//
// Created by mooki on 22-5-30.
//

#include <cstring>

#include <arpa//inet.h>

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

void Channel::enableListen(struct sockaddr_in &peerAddr)  // 针对监听套接字
{
    sqe_ = io_uring_get_sqe(ring_);

    socklen_t peerAddrLen = sizeof peerAddr;
    io_uring_prep_accept(sqe_, sockfd_, (struct sockaddr *)&peerAddr, &peerAddrLen, 0);
    auto *info = (struct ConnInfo *)malloc(sizeof(struct ConnInfo));
    info->channel = this;
    info->eventType = EVENT_ACCEPT;
    info->clientSocket = sockfd_;
    io_uring_sqe_set_data(sqe_, info);

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

void Channel::enableWriting()
{

}