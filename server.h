#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <sys/socket.h>
#include <cassert>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>

const int MAX_EVENT_NUMBER = 10000; //最大事件数
const int TIMESLOT = 5;             //最小超时单位


class WebServer
{
public:
    WebServer();
    ~WebServer();
    void init(int port);


    void eventListen();

public:
    //基础
    int m_port;
    int m_listenfd;
    int m_epollfd;

    // Utils utils;
};
#endif