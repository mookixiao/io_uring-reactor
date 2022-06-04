# io_uring Reactor

## 1 整体设计

首先，需要明确的一点是io_uring是异步IO模型，这在编程中和同步IO模型是很不一样的。前者中，操作系统（或者说内核）向程序通知的是存在IO事件待处理，而具体的IO操作仍需要用户通过read()、write()等系统调用或库函数完成，后者中，操作系统向程序通知的是IO事件已完成，此时目标数据已经读入到/写出自提前指定好的缓冲区，程序接下来只需要执行后续业务逻辑处理即可。

## 1.1 主要数据结构

class EventLoop：实现事件驱动的基础，负责收割事件

class TcpServer：用于编写服务器程序，管理Acceptor、TcpConnection

class Acceptor：用于建立新连接，管理监听套接字对应的Channel

class TcpConnection：管理已有连接，管理连接套接字对应的Channel

class Channel：管理fd（包括监听套接字、连接套接字）

struct ConnInfo：在io_uring事件提交和接收过程中传递信息

## 1.2 主要逻辑

### 1.2.1 收割事件 - 相当于epoll中的poll操作

从io_uring_wait()返回的struct io_uring_cqe *中获取已完成的事件类型 -> 调用对应事件处理函数进行处理

* 事件处理函数已经提前设置好，如读回调、写回调、自定义回调等

### 1.2.2 socket特有

#### 开启监听

创建（socket()）并命名（bind()）监听套接字 -> 开始监听（listen()） -> **通过io_uring_prep_accept()使内核接管监听套接字**  -> **等待io_uring_wait()返回，即存在新连接等待处理**

#### 建立新连接

从io_uring_wait()返回的struct io_uring_cqe *中获取新的连接套接字fd -> **通过io_uring_prep_send()使内核接管连接套接字** -> **等待io_uring_wait()返回，即已经有数据被读入到缓冲区中等待处理**

#### 处理连接对应Channel上的事件

### 1.2.3 timer特有



## 2 Channel实验方案

### 2.1 socket

<img src='./' width='80%'>

### 2.2 timer



## 3 性能测试

### 3.1 测试环境

> 测试在虚拟机下进行，结果仅供参考

CPU：Intel Core i5 8300H

内存：8GB 2667Mhz

硬盘：浦科特M9PeG 512GB

操作系统：Ubuntu 20.04.4

Linux内核版本：5.13.0

liburing版本：2022年5月25日clone自[Github上axboe的liburing代码仓库](https://github.com/axboe/liburing)

### 3.2 测试结果





## 4 工具

## 4 工具

## 工具

1. 性能测试程序：[]()
2. 作为对比的基于epoll的echo服务器：[]()

## 5 参考资料

## 参考资料

1. [Github上chenshuo的muduo代码仓库](https://github.com/chenshuo/muduo)
2. [Github上chenshuo的recipes代码仓库](https://github.com/chenshuo/recipes)
3. [Github上frevib的io_uring-echo-server代码仓库](https://github.com/chenshuo/recipes)
4. [长文梳理Muduo库核心代码及优秀编程细节剖析](https://blog.csdn.net/T_Solotov/article/details/124044175?spm=1001.2014.3001.5506)
5. [AIO 的新归宿：io_uring](https://zhuanlan.zhihu.com/p/62682475)
6. [浅析开源项目之io_uring](https://zhuanlan.zhihu.com/p/361955546)
7. [Lord of the io_uring](https://unixism.net/loti/)
8. [Efficient IO with io_uring](https://kernel.dk/io_uring.pdf)





## 6 TODO

1. 加入多线程支持，实现One Loop Per Thread

2. 当前是全局缓存方案，要改成类似muduo中的缓存方案

3. 断开连接处理逻辑不完整

   

## 存在的疑问
