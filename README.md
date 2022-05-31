# IO_Uring Reactor

## 存在的疑问

1. Channel的setReadCallback(const ReadCallback& readCallback)那里为什么必须是const