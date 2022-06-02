//
// Created by mooki on 22-5-31.
//

#include "TcpConnection.h"

#include "Limits.h"

extern char bufs[][MAX_MESSAGE_LEN];

TcpConnection::TcpConnection(EventLoop *loop,
                             struct io_uring *ring,
                             std::string &name,
                             int sockfd,
                             struct sockaddr_in &localAddr,
                             struct sockaddr_in &peerAddr)
                                     : ownerLoop_(loop),
                                     ring_(ring),
                                     name_(name),
                                     channel_(new Channel(ring_, sockfd))
{
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this));
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
}

void TcpConnection::send(char *buf, int size)  // messageCallback_中调用
{
    // TODO: 这一部分移动到Channel中更合适
    sqe_ = io_uring_get_sqe(ring_);

    io_uring_prep_send(sqe_, channel_->fd(), buf, size, 0);
    auto info = (struct ConnInfo*)malloc(sizeof(struct ConnInfo));
    info->channel = channel_;
    info->eventType = EVENT_WRITE;
    info->sockfd = channel_->fd();
    io_uring_sqe_set_data(sqe_, info);

    io_uring_submit(ring_);
}

void TcpConnection::connectEstablished()
{
    setState(kConnected);
    channel_->addRead();  // 重启读事件
    connectionCallback_(shared_from_this());
}

void TcpConnection::handleRead()
{
    struct io_uring_cqe *cqe = channel_->cqe();
    auto info = (struct ConnInfo *)cqe->user_data;
    int bufId = info->bId;
    int size = cqe->res;
    messageCallback_(shared_from_this(), bufs[bufId], size);
}

void TcpConnection::handleWrite()
{
    // 重置缓冲区
    sqe_ = io_uring_get_sqe(ring_);

    int bufId = channel_->cqe()->flags >> 16;
    io_uring_prep_provide_buffers(sqe_, bufs[bufId], MAX_MESSAGE_LEN, 1, BGID, bufId);
    auto *info = (struct ConnInfo *)channel_->cqe()->user_data;
    info->eventType = EVENT_PROV_BUF;

    io_uring_submit(ring_);

    // 重启读事件监听
    channel_->addRead();
}