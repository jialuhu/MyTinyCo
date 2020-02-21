# MyTinyCo
* <a href="#1">一、简介 </a>
* <a href="#2">二、开发概述 </a>
* <a href="#3">三、使用概述 </a>
* <a href="#4">四、性能测试 </a>


## <a name="1">一、简介</a>
该网络库是一个事件驱动型TCP网络库，支持MacOS和Linux平台使用。
- MacOS:Apple LLVM version 9.0.0 (clang-900.0.39.2)测试通过。
- Linux:Ubuntu gcc version 7.4.0测试通过

## <a name="2">二、开发概述
### 2.1 背景
本项目的初衷最初是设计一个能够运行在MacOS和Linux上的TCP网络库，以供使用。

### 2.2 MyTinyCo解剖
#### 2.2.1 主要概述图
![image](https://github.com/jialuhu/MyTinyCo/blob/master/doc/Rector.png)

#### 2.2.2 事件循环驱动模块设计概述
该模块主要是一个基于IO多路复用和Reactor事件模型设计的事件循环。它能确保所有注册的事件都在EventLoop对象所在的线程中执行，不用考虑事件的并发。

#### 2.2.3 事件分发器模块设计概述
事件源在进行相应的事件处理函数注册后，可以在事件分发器模块得到相应回应。当IO多路复用检测到事件时，事件分发器会根据读写事件执行相应回调函数。

#### 2.2.4 事务处理器模块设计概述
为了充分利用处理器，除了任务线程可以参与计算处理外，IO线程在不忙碌时也可以参与部分计算。

#### 2.2.5 网络模块设计概述
网络模块主要处理一些网络连接细节。其中采用的是异步IO多路复用结合线程池的方式进行处理。

#### 2.2.6 应用缓冲区设计概述
为了内存的方便管理以及满足缓冲区自增长模式的需求，应用缓冲区采用vector来设计。

#### 2.2.7 计时器设计概述
计时器设计由于跨平台的不同，采用IO多路复用的超时参数进行设计。

#### 2.2.8 日志设计概述
日志设置了基础相应级别，并实现了异步发送，对前后端进行了解耦操作。

## <a name="3">三、使用概述
### 3.1 多平台支持
- MacOS
- Linux

### 3.2 支持如下功能模式
- 支持定时器功能
- 支持Reactor模型
- 支持非阻塞连接
- 支持带有双缓冲异步日志
- 支持前后台分离的IO读写操作
- 支持one loop per thread + 单线程模式
- 支持one loop per thread + IO线程模式
- 支持one loop per thread + IO线程池模式
- 支持one loop per thread + 任务线程池模式

### 3.3 特点

- 采用C++11新特性减少Boost库的依赖
- 利用异步I/O多路复用实现并发
- 控制了内存泄漏
- 双平台使用
- 处理速度快
- 稳定性高
- 使用简洁

### 3.5 使用示例

例如：echo-server
```
void onMessage(const TcpConnectionPtr& conn, Buffer& buf)
{
    conn->send(buf.peek(), buf.readable());
    buf.retrieve(buf.readable());
}
int main()
{
    EventLoop loop;
    InetAddr  Addr(8000,"127.0.0.1");
    TcpServer echoServer_(&loop,Addr);
    echoServer_.setOnMessageCb(std::bind(&onMessage,std::placeholders::_1,std::placeholders::_2));
    echoServer_.start();
    loop.loop();
    return 0;
}
```


### 3.6 目录结构
#### 3.6.1 MyTinyCo
该目录存放的是网络库的源代码,文件名称与其功能如下：

- Acceptor-----------封装了accept事件
- Buffer-------------封装了缓冲区
- Channel------------封装了事件分发器
- Connector----------封装了connect事件
- Evloop-------------封装了事件驱动循环
- InetAddr-----------封装了网络地址及端口处理
- IoPoller-----------封装了IO多路复用函数
- IOthread-----------封装了IO线程
- IOthreadpool-------封装了IO线程池
- Log----------------封装了异步日志
- NowTime------------封装了时间基本操作
- Poller-------------作为IO多路复用的基础类
- SocketOpt----------封装网络基本操作函数
- Taskthreadpool-----封装了任务线程池
- TcpClient----------封装了客户端行为
- Tcpconn------------对连接的封装
- TcpServer----------封装了服务端行为
- TimeEvent----------封装了定时器事件
- TimeEvent----------封装了定时器事件链表

#### 3.6.2 example
该目录存放的是一些简单的例子，文件名称与其功能如下：

- DaytimeServer------daytime服务端
- DiscardServer------discard服务端
- DiscardClietn------discard客户端
- Echoserver---------echo协议服务端
- EchoClient---------echo协议客户端



### 3.7 下载安装
#### 3.7.1 安装条件
- cmake version 3.13
- gcc version 7.4.0及其以上版本

```
mkdir build
cd build
cmake ..
su
make & make install
```
#### 3.7.2 安装使用
- 简单的例子可执行文件在build目录下，在build目录下均可使用。例子中的端口一律为8000,example示例代码的端口号语句如下:

```
InetAddr  Addr(8000,"127.0.0.1");
```
- daytime-svr可使用如下命令作为客户端:
```
nc 127.0.0.1 8000
```


## <a name="4">四、性能测试

### 4.1 测试环境相关信息
- 系   统: MacBook Pro (Retina, 13-inch, Early 2015)
- CPU型号: Intel(R) Core(TM) i5-5257U CPU @ 2.70GHz
- CPU个数: 2

### 4.2 测试方案
利用该网络库编写的HTTP服务器。在本地环境下使用siege进行测试。测试命令如下：
```
siege -r 1000000 -c 1 http://localhost:8000/testmyco.html
```
### 4.3测试结果
![image](https://github.com/jialuhu/MyTinyCo/blob/master/doc/meitu.jpg)