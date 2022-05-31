//
// Created by mooki on 22-5-31.
//

#include "TcpConnection.h"
#include "TcpServer.h"

void defaultConnectionCallback() {}  // 默认连接事件回调函数，什么也不做
void defaultMessageCallback() {}  // 默认信息事件回调函数，什么也不做

TcpServer::TcpServer(EventLoop *loop, int port)
        : ownerLoop_(loop),
        acceptor_(ownerLoop_, &ring, port),
        connId_(0),
        connectionCallback_(defaultConnectionCallback),
        messageCallback_(defaultMessageCallback)
{
    acceptor_.setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

void TcpServer::newConnection(int sockfd, struct sockaddr_in &peerAddr)
{
    // 全局唯一连接名
    char buf[10];
    snprintf(buf, sizeof buf, "#%d", connId_);
    ++connId_;
    std::string connName(buf);

    // 本端
    struct sockaddr_in localAddr;
    socklen_t localAddrLen = sizeof localAddr;
    ::getsockname(sockfd, (sockaddr *)&localAddr, &localAddrLen);

    TcpConnectionPtr conn(new TcpConnection(loop_, connName, sockfd, localAddr, peerAddr));
    connections_[connName] = conn;
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection, this, std::placeholders::_1));
    conn->connectEstablished();
}