//
// Created by mooki on 22-5-31.
//

#ifndef IO_URING_REACTOR_TCPSERVER_H
#define IO_URING_REACTOR_TCPSERVER_H

#include <liburing.h>

#include "Acceptor.h"
#include "Types.h"

class EventLoop;

class TcpServer {
public:
    TcpServer(EventLoop *loop, uint16_t port);

    void start();

    void setConnectionCallback(const ConnectionCallback &cb) { connectionCallback_ = cb; }
    void setMessageCallback(const MessageCallback &cb) { messageCallback_ = cb; }

private:
    void newConnection(int connfd, struct sockaddr_in &peerAddr);
    void removeConnection(const TcpConnectionPtr &conn);

    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;

    EventLoop *loop_;
    Acceptor acceptor_;
    ConnectionMap connections_;

    std::string name_;
    int connId_;

    struct io_uring *ring_;
};


#endif //IO_URING_REACTOR_TCPSERVER_H
