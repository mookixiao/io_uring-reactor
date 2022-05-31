//
// Created by mooki on 22-5-30.
//

#ifndef IO_URING_REACTOR_TYPES_H
#define IO_URING_REACTOR_TYPES_H

#include <functional>
#include <vector>

#include <sys/uio.h>

class Channel;

// Class
typedef std::vector<Channel *> ChannelList;

// Events
const int kReadEvent = 0x01;
const int kWriteEvent = 0x02;
const int kCloseEvent = 0x04;

// Func
typedef std::function<void()> NewConnectionCallback;

typedef std::function<void()> AcceptCallback;

typedef std::function<void()> ReadCallback;
typedef std::function<void()> WriteCallback;
typedef std::function<void()> CloseCallback;

// Requests
enum EventType { EVENT_ACCEPT, EVENT_READ, EVENT_WRITE };

struct Request {
    Channel *channel;
    EventType eventType;
    int iovecCnt;
    int clientSocket;
    struct iovec iov[];
};

// Socket
enum Status { STAT_NEW, STAT_ADDED, STAT_DELETED };

#endif //IO_URING_REACTOR_TYPES_H
