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
    explicit Channel(EventLoop *loop, struct io_uring& ring)
            : ownerLoop_(loop), ring_(ring), listening(true), status_(STAT_NEW), events_(0), revents_(0)  {}

    // 监听套接字
    void setAcceptCallback(const AcceptCallback& cb) { acceptCallback_ = cb; }

    void enableListen() { listening = true; io_uring_submit(&ring_); };

    // 一般套接字
    void setReadCallback(const ReadCallback& cb) { readCallback_ = cb; };
    void setWriteCallback(const ReadCallback& cb) { writeCallback_ = cb; };
    void setCloseCallback(const CloseCallback& cb) { closeCallback_ = cb; };

    void enableReading() { events_ |= kReadEvent; update(); }
    void enableWriting() { events_ |= kWriteEvent; }

    void handleEvent();

    Status status() { return status_;}

private:
    void update();

    AcceptCallback acceptCallback_;

    ReadCallback readCallback_;
    WriteCallback writeCallback_;
    CloseCallback closeCallback_;

    EventLoop *ownerLoop_;
    struct io_uring ring_;

    EventType eventType;

    bool listening;

    int events_;  // 对什么事件感兴趣
    int revents_;  // 当前发生了什么事件
};


#endif //IO_URING_REACTOR_CHANNEL_H
