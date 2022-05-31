//
// Created by mooki on 22-5-31.
//

#ifndef IO_URING_REACTOR_LIMITS_H
#define IO_URING_REACTOR_LIMITS_H

// 监听
#define BACKLOG             512

// 连接
#define MAX_CONNECTIONS     4096
#define MAX_MESSAGE_LEN     2048
#define BUFFERS_COUNT       MAX_CONNECTIONS

#endif //IO_URING_REACTOR_LIMITS_H
