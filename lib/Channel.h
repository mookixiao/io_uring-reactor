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
    Channel(int sockfd, struct io_uring *ring);
    ~Channel();

    void handleEvent();

    // 监听套接字
    void setAcceptCallback(const AcceptEventCallback& cb) { acceptCallback_ = cb; }

    void addListen(struct sockaddr_in &peerAddr, socklen_t *peerAddrLen);

    // 一般套接字
    void setReadCallback(const ReadEventCallback& cb) { readCallback_ = cb; };
    void setWriteCallback(const WriteEventCallback& cb) { writeCallback_ = cb; };
    void setCloseCallback(const CloseCallback& cb) { closeCallback_ = cb; };

    void addRead();
    void addWrite(char *buf, int size);
    void addBuffer();

    // 状态
    int getFd() const { return fd_; }

    struct io_uring_cqe* getCqe() const { return cqe_; }
    void setCqe(struct io_uring_cqe *cqe) { cqe_ = cqe; }

    int getBId() const { return bId_; }

    struct ConnInfo* getConnInfo() const { return info_; }
    void setConnInfo(struct ConnInfo *info) { info_ = info; }

    EventType getEventType() const { return eventType_; }
    void setEventType(EventType type) { eventType_ = type; };

private:
    AcceptEventCallback acceptCallback_;

    ReadEventCallback readCallback_;
    WriteEventCallback writeCallback_;
    CloseCallback closeCallback_;

    int fd_;

    struct io_uring *ring_;
    struct io_uring_sqe *sqe_;
    struct io_uring_cqe *cqe_;
    int bId_;

    struct ConnInfo *info_;
    EventType eventType_;
};


#endif //IO_URING_REACTOR_CHANNEL_H
