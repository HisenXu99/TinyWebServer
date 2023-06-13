#include "server.h"
#include <stdio.h>

void setnonblockingmode(int);

WebServer::WebServer()
{

}

WebServer::~WebServer()
{

}

void WebServer::init(int port)
{
    m_port = port;
    // m_user = user;
    // m_passWord = passWord;
    // m_databaseName = databaseName;
    // m_sql_num = sql_num;
    // m_thread_num = thread_num;
    // m_log_write = log_write;
    // m_OPT_LINGER = opt_linger;
    // m_TRIGMode = trigmode;
    // m_close_log = close_log;
    // m_actormodel = actor_model;
}

void WebServer::eventListen()
{
    m_listenfd=socket(PF_INET,SOCK_STREAM,0);
    assert(m_listenfd);

    //优雅关闭连接
    int ret = 0;
    sockaddr_in address;              //这里加不加struct有什么区别？
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(m_port);

    int flag = 1;
    //经历TIME_WAIT的过程,后继续重用该socket
    setsockopt(m_listenfd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag));
    ret = bind(m_listenfd,(struct sockaddr *)&address,sizeof(address));
    assert(ret >= 0);
    ret = listen(m_listenfd, 5);
    assert(ret >= 0);

    // utils.init(TIMESLOT);

    //epoll创建内核事件表
    epoll_event events[MAX_EVENT_NUMBER];
    m_epollfd = epoll_create(5);    //size对象被忽视，设置成几都行
    assert(m_epollfd != -1);



    //Hisen
 
    int clnt_sock;
    struct sockaddr_in clnt_adr;
    socklen_t adr_sz;
    int event_cnt;
    epoll_event listen_event;
    char buf[30];
    setnonblockingmode(m_listenfd);
    listen_event.events = EPOLLIN;        // 监听服务器套接字的输入事件
    listen_event.data.fd = m_listenfd;
    epoll_ctl(m_epollfd, EPOLL_CTL_ADD, m_listenfd, &listen_event);

    while(1){
        event_cnt = epoll_wait(m_epollfd, events, 5, -1);
        if (event_cnt == -1) {
            puts("epoll_wait() error");
            break;
        }
        puts("return epoll_wait");
        for (int i = 0; i < event_cnt; i++) {
            if (events[i].data.fd == m_listenfd) {
                adr_sz = sizeof(clnt_adr);
                clnt_sock = accept(m_listenfd, (struct sockaddr *)&clnt_adr, &adr_sz);
                setnonblockingmode(clnt_sock);    // 将用于连接客户的套接字设为非阻塞模式
                listen_event.events = EPOLLIN | EPOLLET; // 以边缘触发方式监听用于连接客户的套接字上的输入事件
                listen_event.data.fd = clnt_sock;
                epoll_ctl(m_epollfd, EPOLL_CTL_ADD, clnt_sock, &listen_event);
                printf("connected client: %d \n", clnt_sock);
            }
            else{
                while(1)
                {
                    int str_len=read(events[i].data.fd,buf,30);
                    if(str_len==0){
                        epoll_ctl(m_epollfd,EPOLL_CTL_DEL,events[i].data.fd,NULL);
                        close(events[i].data.fd);
                        printf("closed client: %d \n",events[i].data.fd);
                        break;
                    }
                    else if(str_len<0){
                        break;
                    }
                    else{
                        write(events[i].data.fd,buf,str_len);
                    }

                }
            }
        }
    }
    close(m_listenfd);
    close(m_epollfd);

}

void setnonblockingmode(int fd) {
    int flag = fcntl(fd, F_GETFL, 0);   //fcntl函数，也就是file control，提供了对文件描述符的各种操作。
    fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}