//
// Created by mooki on 22-5-31.
//

#include "TcpConnection.h"
#include "TcpServer.h"

TcpServer::TcpServer(EventLoop *loop, int port)
        : ownerLoop_(loop),
        ring_(loop->ring()),
        acceptor_(ownerLoop_, ring_, port),
        connId_(0)
{
    acceptor_.setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

void TcpServer::start()
{
    acceptor_.listen();
}

void TcpServer::newConnection(int connfd, struct sockaddr_in &peerAddr)  // 在acceptChannel_中被调用
{
    // 全局唯一连接名
    char buf[10];
    snprintf(buf, sizeof buf, "#%d", connId_);
    ++connId_;
    std::string connName(buf);

    // 本端
    struct sockaddr_in localAddr;
    socklen_t localAddrLen = sizeof localAddr;
    ::getsockname(connfd, (sockaddr *)&localAddr, &localAddrLen);

    // 远端
    TcpConnectionPtr conn(new TcpConnection(
            ownerLoop_, ring_, connName, connfd, localAddr, peerAddr));
    connections_[connName] = conn;
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection, this, std::placeholders::_1));
    conn->connectEstablished();
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
    if (connections_.erase(conn->name()) != 1) {
        perror("erase");
    }
}