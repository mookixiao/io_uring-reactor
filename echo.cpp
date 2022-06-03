#include "EventLoop.h"
#include "TcpConnection.h"
#include "TcpServer.h"

void onConnection(const TcpConnectionPtr& conn)
{
    // 什么也不做
}

void onMessage(const TcpConnectionPtr& conn, char *buf, int size)
{
    conn->send(buf, size);
}

int main()
{
    uint16_t port = 65421;
    EventLoop loop;

    TcpServer server(&loop, port);
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.start();

    loop.loop();
}
