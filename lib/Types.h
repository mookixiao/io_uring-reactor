//
// Created by mooki on 22-5-30.
//

#ifndef IO_URING_REACTOR_TYPES_H
#define IO_URING_REACTOR_TYPES_H

#include <functional>
#include <map>
#include <string>
#include <vector>

#include <sys/uio.h>

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

typedef std::function<void()> ConnectionCallback;
typedef std::function<void()> MessageCallback;

typedef std::function<void()> AcceptCallback;  // 用于监听Channel

typedef std::function<void()> ReadCallback;  // 用于普通Channel
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
