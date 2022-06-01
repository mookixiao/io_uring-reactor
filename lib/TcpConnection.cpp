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
                                     channel_(ownerLoop_, ring_, sockfd)
{

}

void TcpConnection::connectEstablished()
{
    setState(kConnected);
    channel_->enableReading();
    connectionCallback_(shared_from_this());
}

void TcpConnection::connectDestroyed()
{
    setState(kDisconnected);
    channel_->disableAll();
    connectionCallback_(shared_from_this());

    loop_->removeChannel(get_pointer(channel_));
}