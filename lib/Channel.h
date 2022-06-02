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
            : ring_(ring), sockfd_(sockfd)  {}

    void handleEvent();

    // 监听套接字
    void setAcceptCallback(const AcceptEventCallback& cb) { acceptCompleteCallback_ = cb; }

    void addListen(struct sockaddr_in &peerAddr);

    // 一般套接字
    void setReadCallback(const ReadEventCallback& cb) { readCallback_ = cb; };
    void setWriteCallback(const WriteEventCallback& cb) { writeCallback_ = cb; };
    void setCloseCallback(const CloseCallback& cb) { closeCallback_ = cb; };

    void addRead();
    void addWrite();

    // 状态
    int fd() { return sockfd_; }
    struct ConnInfo *info() { return info_; }
    int res() { return res_; }

    void setConnInfo(struct ConnInfo *info) { info_ = info; }
    void setRes(int res) { res_ = res; }
    void setEventType(EventType type) { eventType_ = type; };

private:
    AcceptEventCallback acceptCompleteCallback_;

    ReadEventCallback readCallback_;
    WriteEventCallback writeCallback_;
    CloseCallback closeCallback_;

    struct io_uring *ring_;
    struct io_uring_sqe *sqe_;
    struct ConnInfo *info_;
    int res_;

    int sockfd_;
    EventType eventType_;
};


#endif //IO_URING_REACTOR_CHANNEL_H
