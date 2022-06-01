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
    explicit Channel(EventLoop *loop, struct io_uring* ring, int sockfd)
            : ownerLoop_(loop), ring_(ring), sockfd_(sockfd), listening_(false), events_(0), revents_(0)  {}

    // 监听套接字
    void setAcceptCallback(const AcceptEventCallback& cb) { acceptCallback_ = cb; }

    void enableListen() { listening_ = true; io_uring_submit(ring_); };

    // 一般套接字
    void setReadCallback(const ReadEventCallback& cb) { readCallback_ = cb; };
    void setWriteCallback(const ReadEventCallback& cb) { writeCallback_ = cb; };
    void setCloseCallback(const CloseCallback& cb) { closeCallback_ = cb; };

    void enableReading() { events_ |= kReadEvent; update(); }
    void enableWriting() { events_ |= kWriteEvent; }

    void handleEvent();

private:
    void update();

    AcceptEventCallback acceptCallback_;

    ReadEventCallback readCallback_;
    WriteEventCallback writeCallback_;
    CloseCallback closeCallback_;

    EventLoop *ownerLoop_;
    struct io_uring *ring_;

    int sockfd_;

    bool listening_;
    EventType eventType;

    int events_;  // 对什么事件感兴趣
    int revents_;  // 当前发生了什么事件
};


#endif //IO_URING_REACTOR_CHANNEL_H
