//
// Created by mooki on 22-5-30.
//

#include "Acceptor.h"

#include <strings.h>

#include <functional>

#include "Limits.h"

char bufs[MAX_CONNECTIONS][MAX_MESSAGE_LEN];

Acceptor::Acceptor(EventLoop *loop, struct io_uring *ring, int port)
        : ownerLoop_(loop),
        ring_(ring),
        acceptSocket_(::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0)),
        acceptChannel_(ring_, acceptSocket_)
{
    // 准备监听套接字
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

    // 准备缓存
    sqe_ = io_uring_get_sqe(ring_);
    io_uring_prep_provide_buffers(sqe_, bufs, MAX_MESSAGE_LEN, MAX_CONNECTIONS, BGID, 0);
    io_uring_submit(ring_);

    io_uring_wait_cqe(ring_, &cqe_);
    if (cqe_->res < 0) {
        perror("io_uring_wait_cqe");
    }
    io_uring_cqe_seen(ring_, cqe_);

    // 准备Channel
    acceptChannel_.setAcceptCallback(std::bind(&Acceptor::handleNewConnection, this));
}

void Acceptor::listen()
{
    // 监听
    if (::listen(acceptSocket_, BACKLOG) < 0) {
        perror("listen");
    }

    acceptChannel_.addListen(newPeerAddr_);  // 每次有新连接时，远端信息会存放到此peerAddr_中
}

void Acceptor::handleNewConnection()
{
    // 建立新连接并交由TcpServer管理
    int connFd;
    if (io_uring_wait_cqe(ring_, &cqe_) < 0) {
        perror("io_uring_wait_cqe");
    }
    if ((connFd = cqe_->res) < 0) {
        perror("io_uring_wait_cqe, cqe->res < 0");
    }

    newConnectionCallback(connFd, newPeerAddr_);

    // 重新开始监听
    acceptChannel_.addListen(newPeerAddr_);
}
