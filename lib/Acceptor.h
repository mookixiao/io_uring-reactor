//
// Created by mooki on 22-5-30.
//

#ifndef IO_URING_REACTOR_ACCEPTOR_H
#define IO_URING_REACTOR_ACCEPTOR_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <string>

#include "liburing.h"

#include "Channel.h"
#include "Limits.h"

class Acceptor {
public:
    explicit Acceptor(EventLoop *loop, struct io_uring *ring, int port);

    void listen();

    std::string toHostPort();

    void setNewConnectionCallback(const NewConnectionCallback &cb) { newConnectionCallback = cb; };

private:
    void handleNewConnection();  // 接受新连接

    EventLoop *loop_;
    struct io_uring *ring_;
    struct io_uring_sqe *sqe_;
    struct io_uring_cqe *cqe_;

    int acceptSocket_;
    struct sockaddr_in addr_;
    Channel acceptChannel_;

    struct sockaddr_in newPeerAddr_;

    NewConnectionCallback newConnectionCallback;
};


#endif //IO_URING_REACTOR_ACCEPTOR_H
