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

    void setConnectionCallback(const ConnectionCallback &cb) { connectionCallback_ = cb; }
    void setMessageCallback(const MessageCallback &cb) { messageCallback_ = cb; }
    void setCloseCallback(const CloseCallback &cb) { closeCallback_ = cb; }

    void connectEstablished();

    std::string name() { return name_; }

private:
    void handleRead(struct io_uring_cqe *cqe);
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
    std::string name_;

    // 相配的Channel
    Channel *channel_;
};


#endif //IO_URING_REACTOR_TCPCONNECTION_H