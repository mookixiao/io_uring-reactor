//
// Created by mooki on 22-5-30.
//

#ifndef IO_URING_REACTOR_TYPES_H
#define IO_URING_REACTOR_TYPES_H

#include <vector>

class Channel;

typedef std::vector<Channel*> ChannelList;

#endif //IO_URING_REACTOR_TYPES_H
