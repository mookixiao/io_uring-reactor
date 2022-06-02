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
    explicit Channel(struct io_uring *ring, int sockfd)
            : ring_(ring), fd_(sockfd)  {}

    void handleEvent();

    // 监听套接字
    void setAcceptCallback(const AcceptEventCallback& cb) { acceptCompleteCallback_ = cb; }

    void addListen(struct sockaddr_in &peerAddr);

    // 一般套接字
    void setReadCallback(const ReadEventCallback& cb) { readCallback_ = cb; };
    void setWriteCallback(const WriteEventCallback& cb) { writeCallback_ = cb; };
    void setCloseCallback(const CloseCallback& cb) { closeCallback_ = cb; };

    void addRead();
    void addWrite(char *buf, int size);
    void addBuffer(char *buf, int size);

    // 状态
    int getFd() { return fd_; }

    struct io_uring_cqe* getCqe() { return cqe_; }
    void setCqe(struct io_uring_cqe *cqe) { cqe_ = cqe; }

    struct ConnInfo* getConnInfo() { return info_; }
    void setConnInfo(struct ConnInfo *info) { info_ = info; }

    EventType getEventType() { return eventType_; }
    void setEventType(EventType type) { eventType_ = type; };

private:
    AcceptEventCallback acceptCompleteCallback_;

    ReadEventCallback readCallback_;
    WriteEventCallback writeCallback_;
    CloseCallback closeCallback_;

    int fd_;

    struct io_uring *ring_;
    struct io_uring_sqe *sqe_;
    struct io_uring_cqe *cqe_;
    struct ConnInfo *info_;

    EventType eventType_;
};


#endif //IO_URING_REACTOR_CHANNEL_H
