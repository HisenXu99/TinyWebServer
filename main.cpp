#include "webserver.h"
#include "config.h"

int main(int argc, char *argv[])
{
    //需要修改的数据库信息,登录名,密码,库名
    string user = "root";
    string passwd = "";
    string databasename = "yourdb";

    //命令行解析
    Config config;                   //config类中是一些基础的配置，包括出发方式的选择，以及一些参数，构造函数中给了默认的参数
    config.parse_arg(argc, argv);    //对输入参数进行处理


    WebServer server;

    //初始化
    server.init(config.PORT, user, passwd, databasename, config.LOGWrite, 
                config.OPT_LINGER, config.TRIGMode,  config.sql_num,  config.thread_num, 
                config.close_log, config.actor_model);   //todo基本上是对config中参数的初始化,这里应该传入config对象的引用，而不应该传入一堆值。


    //监听客户端的连接
    server.eventListen();

    //运行循环，主线程一直在这里执行
    server.eventLoop();
}