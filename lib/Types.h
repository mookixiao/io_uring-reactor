//
// Created by mooki on 22-5-30.
//

#ifndef IO_URING_REACTOR_TYPES_H
#define IO_URING_REACTOR_TYPES_H

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <sys/uio.h>

#include <liburing.h>

class Channel;
class TcpConnection;

// Class
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::vector<Channel *> ChannelList;
typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;  // TcpConnection名字 <-> TcpConnectionPtr
typedef std::map<uint32_t , Channel*> ChannelMap;

// Events
const int kReadEvent = 0x01;
const int kWriteEvent = 0x02;
const int kCloseEvent = 0x04;

// Func
typedef std::function<void(int sockfd, struct sockaddr_in &)> NewConnectionCallback;

typedef std::function<void (const TcpConnectionPtr &)> ConnectionCallback;  // 用于TcpConnection
typedef std::function<void (const TcpConnectionPtr &, char *buf, int size)> MessageCallback;
typedef std::function<void (const TcpConnectionPtr &)> CloseCallback;

typedef std::function<void ()> AcceptEventCallback;  // 用于监听Channel

typedef std::function<void ()> ReadEventCallback;  // 用于普通Channel
typedef std::function<void ()> WriteEventCallback;
typedef std::function<void ()> CloseEventCallback;

typedef std::function<void ()> TimeoutEventCallback;  // 用于定时器Channel

// Requests
enum EventType { EVENT_ACCEPT, EVENT_READ, EVENT_WRITE, EVENT_BUF, EVENT_TIMEOUT, EVENT_NONE };

struct ConnInfo {
    uint32_t fd;
    uint16_t eventType;
    uint16_t bId;
};

// Socket
enum Status { STAT_NEW, STAT_ADDED, STAT_DELETED };

#endif //IO_URING_REACTOR_TYPES_H
