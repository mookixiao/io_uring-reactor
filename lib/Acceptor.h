//
// Created by mooki on 22-5-30.
//

#ifndef IO_URING_REACTOR_ACCEPTOR_H
#define IO_URING_REACTOR_ACCEPTOR_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "Channel.h"
#include "liburing.h"

class Acceptor {
public:
    explicit Acceptor(int port, struct io_uring &ring);

    void listen();

private:
    void handleNewConnection();  // 接受新连接

    int acceptSocket_;
    struct io_uring ring_;
    Channel acceptChannel_;

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen;
//    NewConnectionCallback newConnectionCallback;
};


#endif //IO_URING_REACTOR_ACCEPTOR_H
