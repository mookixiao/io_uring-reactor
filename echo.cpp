#include "EventLoop.h"
#include "InetAddress.h"
#include "TcpServer.h"
#include "Timestamp.h"

void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime)
{
    conn->send(buf->retrieveAsString());
}

int main()
{
    InetAddress listenAddr(10001);  // 0.0.0.0:10001
    EventLoop loop;

    TcpServer server(&loop, listenAddr);
    server.setMessageCallback(onMessage);
    server.start();

    loop.loop();
}
