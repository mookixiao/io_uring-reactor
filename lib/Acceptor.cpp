//
// Created by mooki on 22-5-30.
//

#include "Acceptor.h"

#include <strings.h>

#include <functional>

#include "Limits.h"

Acceptor::Acceptor(EventLoop *loop, struct io_uring &ring, int port)
        : ownerLoop_(loop),
        ring_(ring),
        acceptChannel_(ownerLoop_, ring_),
        clientAddrLen(sizeof clientAddr)
{
    // 监听套接字
    acceptSocket_ = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if (acceptSocket_ < 0) {
        perror("socket");
    }

    int opt = 1;
    if (::setsockopt(acceptSocket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt) < 0) {
        perror("setsockopt");
    }

    struct sockaddr_in addr;
    bzero(&addr, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    if (::bind(acceptSocket_, (struct sockaddr*)&addr, sizeof addr) < 0) {
        perror("bind");
    }

    // Channel
    acceptChannel_.setAcceptCallback(std::bind(&Acceptor::handleNewConnection, this));
}

void Acceptor::listen()
{
    // 监听
    if (::listen(acceptSocket_, BACKLOG) < 0) {
        perror("listen");
    }

    // 使用io_uring进行监视
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring_);
    io_uring_prep_accept(sqe, acceptSocket_, (struct sockaddr *)&clientAddr, &clientAddrLen, 0);
    auto req = (struct Request *)malloc(sizeof(struct Request));
    req->eventType = EVENT_ACCEPT;
    io_uring_sqe_set_data(sqe, req);

    acceptChannel_.enableListen();
}

void Acceptor::handleNewConnection()
{
//    struct sockaddr_in clientAddr;

    newConnectionCallback();
}