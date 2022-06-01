//
// Created by mooki on 22-5-31.
//

#include "TcpConnection.h"

TcpConnection::TcpConnection(EventLoop *loop,
                             struct io_uring *ring,
                             std::string &name,
                             int sockfd,
                             struct sockaddr_in &localAddr,
                             struct sockaddr_in &peerAddr)
                                     : ownerLoop_(loop),
                                     ring_(ring),
                                     name_(name),
                                     channel_(new Channel(ownerLoop_, ring_, sockfd))
{
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this, std::placeholders::_1));
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
    channel_->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
}

void TcpConnection::connectEstablished()
{
    setState(kConnected);
    channel_->enableReading();
    connectionCallback_(shared_from_this());
}

void TcpConnection::handleRead(struct io_uring_cqe *cqe)
{

//    ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
//    if (n > 0) {
//        messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
//    } else if (n == 0) {
//        handleClose();
//    } else {
//        perror("readFD");
//    }
}

void TcpConnection::handleWrite()
{
    if (channel_->isWriting()) {
        ssize_t n = ::write(channel_->fd(), outputBuffer_.peek(), outputBuffer_.readableBytes());
        if (n > 0) {
            outputBuffer_.retrieve(n);
            if (outputBuffer_.readableBytes() == 0) {
                channel_->disableWriting();
                if (state_ == kDisconnecting) {
                    shutdownInLoop();
                }
            } else {
                perror("more data to write");
            }
        } else {
            perror("handleWrite");
        }
    }
}

void TcpConnection::handleClose()
{
    channel_->disableAll();
    closeCallback_(shared_from_this());
}