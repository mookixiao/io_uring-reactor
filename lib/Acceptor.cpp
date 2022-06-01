//
// Created by mooki on 22-5-30.
//

#include "Acceptor.h"

#include <strings.h>

#include <functional>

#include "Limits.h"

Acceptor::Acceptor(EventLoop *loop, struct io_uring *ring, int port)
        : ownerLoop_(loop),
        ring_(ring),
        acceptSocket_(::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0)),
        acceptChannel_(ownerLoop_, ring_, acceptSocket_),
        clientAddrLen(sizeof clientAddr)
{
    // 监听套接字
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

    // 设置io_uring缓冲区
    struct io_uring_sqe *sqe;
    struct io_uring_cqe *cqe;

    sqe = io_uring_get_sqe(ring_);
    io_uring_prep_provide_buffers(sqe, buf, MAX_MESSAGE_LEN, BUFFERS_COUNT, BGID, 0);
    io_uring_submit(ring_);

    io_uring_wait_cqe(ring_, &cqe);
    if (cqe->res < 0) {
        perror("io_uring_wait_cqe");
    }
    io_uring_cqe_seen(ring_, cqe);

    // 使用io_uring进行监视
    io_uring_get_sqe(ring_);
    io_uring_prep_accept(sqe, acceptSocket_, (struct sockaddr *)&clientAddr, &clientAddrLen, 0);
    auto req = (struct Request *)malloc(sizeof(struct Request));
    req->eventType = EVENT_ACCEPT;
    io_uring_sqe_set_data(sqe, req);

    // 设置
    acceptChannel_.enableListen();
}

void Acceptor::handleNewConnection()
{
    int connFd;
    struct sockaddr_in peerAddr;

    struct io_uring_cqe *cqe;
    if (io_uring_wait_cqe(ring_, &cqe) < 0) {
        perror("io_uring_wait_cqe");
    }
    if ((connFd = cqe->res) < 0) {
        perror("io_uring_wait_cqe, cqe->res < 0");
    }

    newConnectionCallback(connFd, peerAddr);
}