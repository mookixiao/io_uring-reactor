//
// Created by mooki on 22-5-30.
//

#ifndef IO_URING_REACTOR_CHANNEL_H
#define IO_URING_REACTOR_CHANNEL_H

#include <liburing.h>

#include "Types.h"

class EventLoop;

class Channel {
public:
    Channel(EventLoop *loop, struct io_uring *ring, int sockfd = 999)
            : loop_(loop), fd_(sockfd), ring_(ring), sqe_(nullptr), cqe_(nullptr), bId_(0) {}

    void handleEvent();

    void update();

    // 监听套接字
    void setAcceptCallback(const AcceptEventCallback& cb) { acceptCallback_ = cb; }

    void addListen(struct sockaddr_in &peerAddr, socklen_t *peerAddrLen);

    // 一般套接字
    void setReadCallback(const ReadEventCallback &cb) { readCallback_ = cb; };
    void setWriteCallback(const WriteEventCallback &cb) { writeCallback_ = cb; };
    void setCloseCallback(const CloseEventCallback &cb) { closeCallback_ = cb; };

    void addRead();
    void addWrite(char *buf, int size);
    void addBuffer();

    // 定时器
    void setTimeoutCallback(const TimeoutEventCallback &timeoutEventCallback) { timeoutCallback_ = timeoutEventCallback; };

    void addTimeout(struct __kernel_timespec *time, const TimeoutEventCallback &cb);

    int getFd() const { return fd_; }

    int getBId() const { return bId_; }

    struct io_uring_cqe* getCqe() const { return cqe_; }
    void setCqe(struct io_uring_cqe *cqe) { cqe_ = cqe; }

private:
    void fillConnInfo(unsigned long long *info, EventType type) const;

    // 套接字
    AcceptEventCallback acceptCallback_;

    ReadEventCallback readCallback_;
    WriteEventCallback writeCallback_;
    CloseEventCallback closeCallback_;

    // 定时器
    TimeoutEventCallback timeoutCallback_;

    EventLoop *loop_;

    int fd_;

    struct io_uring *ring_;
    struct io_uring_sqe *sqe_;
    struct io_uring_cqe *cqe_;
    uint16_t bId_;
};


#endif //IO_URING_REACTOR_CHANNEL_H
