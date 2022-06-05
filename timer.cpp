#include <ctime>

#include "EventLoop.h"
#include "Timer.h"

void printCurrentTime()
{
    time_t nowTime;
    struct tm *nowLocalTime;
    nowTime = time(nullptr);
    nowLocalTime = localtime(&nowTime);
    printf("%d-%d-%d %d:%d:%d\n",
           nowLocalTime->tm_year + 1900, nowLocalTime->tm_mon + 1, nowLocalTime->tm_mday,
           nowLocalTime->tm_hour, nowLocalTime->tm_min, nowLocalTime->tm_sec);
}

int main()
{
    EventLoop loop;

    Timer timer(&loop);
    printCurrentTime();
    timer.runAfter(3.5, printCurrentTime);

    loop.loop();
}
