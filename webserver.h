#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <sys/socket.h>
#include <cassert>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <signal.h>
#include <string.h>
#include "./timer/lst_timer.h"

using namespace std;

const int MAX_FD = 65536;           //最大文件描述符
const int MAX_EVENT_NUMBER = 10000; //最大事件数
const int TIMESLOT = 5;             //最小超时单位


class WebServer
{
public:
    WebServer();
    ~WebServer();
    void init(int port , string user, string passWord, string databaseName,
              int log_write , int opt_linger, int trigmode, int sql_num,
              int thread_num, int close_log, int actor_model);
    void trig_mode();


    void eventListen();   //创建listen的套接字
    void eventLoop();     //服务器的主线程，一直在这里循环
    bool dealclinetdata();
    bool dealwithsignal(bool &timeout, bool &stop_server);

public:
    //基础
    int m_port;
    char *m_root;
    int m_listenfd;
    int m_epollfd;
    // int m_actormodel;

    int m_pipefd[2];        //套接字对的数组，用来进行进程间通信,发生时钟中断等时，回调函数向管道中写信息，通过epoll进行统一管理。



    //数据库相关
    //connection_pool *m_connPool;
    string m_user;         //登陆数据库用户名
    string m_passWord;     //登陆数据库密码
    string m_databaseName; //使用数据库名


    //epoll_event相关
    epoll_event events[MAX_EVENT_NUMBER];

    int m_TRIGMode;
    int m_LISTENTrigmode;
    int m_CONNTrigmode;


    //定时器相关
    client_data *users_timer;
    Utils utils;
};
#endif