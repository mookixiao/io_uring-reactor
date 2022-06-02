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
                                     channel_(new Channel(sockfd, ring_))
{
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this));
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
}

void TcpConnection::connectEstablished()
{
    setState(kConnected);
    channel_->addRead();  // 重启读事件
    connectionCallback_(shared_from_this());
}

// 在messageCallback_中被调用
void TcpConnection::send(char *buf, int size)
{
    channel_->addWrite(buf, size);
}

// 提供给Channel作为各个事件的回调
void TcpConnection::handleRead()
{
    int size = channel_->getCqe()->res;

    messageCallback_(shared_from_this(), bufs[channel_->getBId()], size);
}

void TcpConnection::handleWrite()
{
    // 设置缓冲区
    channel_->addBuffer();

    // 重启读事件监听
    channel_->addRead();
}

void TcpConnection::handleClose()
{
    closeCallback_(shared_from_this());
}