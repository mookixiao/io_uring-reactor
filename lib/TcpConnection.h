//
// Created by mooki on 22-5-31.
//

#ifndef IO_URING_REACTOR_TCPCONNECTION_H
#define IO_URING_REACTOR_TCPCONNECTION_H

#include <string>

#include "Channel.h"
#include "EventLoop.h"

class TcpConnection {
public:
    TcpConnection(EventLoop *loop, struct io_uring *ring, std::string &name,
                  int sockfd, struct sockaddr_in &localAddr, struct sockaddr_in &peerAddr);

private:
    // TcpConnection
    EventLoop *ownerLoop_;
    struct io_uring *ring_;
    std::string name_;

    // 相配的Channel
    Channel channel_;
};


#endif //IO_URING_REACTOR_TCPCONNECTION_H
