//
// Created by mooki on 22-5-31.
//

#ifndef IO_URING_REACTOR_TCPCONNECTION_H
#define IO_URING_REACTOR_TCPCONNECTION_H

#include <string>

#include "Channel.h"
#include "EventLoop.h"

class TcpConnection : std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(EventLoop *loop, struct io_uring *ring, std::string &name,
                  int sockfd, struct sockaddr_in &localAddr, struct sockaddr_in &peerAddr);

    void setCloseCallback(const CloseCallback &cb) { closeCallback_ = cb; }

    void setConnectionCallback(const ConnectionCallback &cb) { connectionCallback_ = cb; }
    void setMessageCallback(const MessageCallback &cb) { messageCallback_ = cb; }

    void connectEstablished();
    void send(char *buf, int size);

    std::string name() { return name_; }

private:
    void handleRead();
    void handleWrite();

    void handleClose();

    enum State { kConnecting, kConnected, kDisconnecting, kDisconnected};
    State state_;
    void setState(State s) { state_ = s; };

    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    CloseCallback closeCallback_;

    // TcpConnection
    EventLoop *ownerLoop_;
    struct io_uring *ring_;
    struct io_uring_sqe *sqe_;
    struct io_uring_cqe *cqe_;
    std::string name_;

    // 相配的Channel
    Channel *channel_;
};


#endif //IO_URING_REACTOR_TCPCONNECTION_H
