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

// Requests
enum EventType { EVENT_ACCEPT, EVENT_READ, EVENT_WRITE, EVENT_PROV_BUF };

struct ConnInfo {
    Channel *channel;
    EventType eventType;
    int sockfd;  // 对于监听套接字指本端，对于连接套接字指对端
    int bId;  // 普通套接字专用
};

// Socket
enum Status { STAT_NEW, STAT_ADDED, STAT_DELETED };

#endif //IO_URING_REACTOR_TYPES_H
