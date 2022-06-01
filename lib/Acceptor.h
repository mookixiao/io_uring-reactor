//
// Created by mooki on 22-5-30.
//

#ifndef IO_URING_REACTOR_ACCEPTOR_H
#define IO_URING_REACTOR_ACCEPTOR_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "liburing.h"

#include "Channel.h"
#include "Limits.h"

class Acceptor {
public:
    explicit Acceptor(EventLoop *loop, struct io_uring *ring, int port);

    void listen();

    void setNewConnectionCallback(const NewConnectionCallback &cb) { newConnectionCallback = cb; };

private:
    void handleNewConnection();  // 接受新连接

    EventLoop *ownerLoop_;
    struct io_uring *ring_;

    // 缓冲区
    char buf[MAX_CONNECTIONS][MAX_MESSAGE_LEN];

    int acceptSocket_;
    Channel acceptChannel_;

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen;

    NewConnectionCallback newConnectionCallback;
};


#endif //IO_URING_REACTOR_ACCEPTOR_H
