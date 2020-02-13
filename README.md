# MyTinyCo
* <a href="#1">一、简介 </a>
* <a href="#2">二、背景 </a>
* <a href="#3">三、使用概述 </a>


## <a name="1">一、简介</a>
该网络库是一个小型简易的TCP网络库，支持MacOS和Linux平台使用。
MacOS:Apple LLVM version 9.0.0 (clang-900.0.39.2)测试通过。
Linux:目前正在开发测试中

## <a name="2">二、背景
本项目的初衷最初是设计一个能够运行在MacOS上的网络库，以供方便学习。在初学muduo后，采用muduo的接口设计进行了该项目的开发。在muduo
的接口设计上，采用c++11新特性进行编写。

## <a name="3">三、使用概述
### 3.1 多平台支持
在使用前，请确保gcc的版本。如若版本过低无法通过编译。
支持MacOS和Linux。
- MacOS：Apple LLVM version 9.0.0 (clang-900.0.39.2)测试通过。
- Linux：目前正在开发测试中

### 3.2 支持如下功能模式
- 支持定时器功能
- 支持Reactor模型
- 支持长连接、非阻塞连接
- 支持带有双缓冲异步日志
- 支持前后台分离的IO读写操作
- 支持one loop per thread + 单线程模式
- 支持one loop per thread + IO线程模式
- 支持one loop per thread + IO线程池模式
- 支持one loop per thread + 任务线程池模式

### 3.3 采用C++11新特性减少Boost库的依赖
- 智能指针
- Lambada
- RAII模式管理资源

### 3.4 使用简洁

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
### 3.5 目录结构
#### 3.5.1 MyTinyCo
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

#### 3.5.2 example
该目录存放的是一些简单的例子，文件名称与其功能如下：

- DaytimeServer------daytime服务端
- DiscardServer------discard服务端
- DiscardClietn------discard客户端
- Echoserver---------echo协议服务端
- EchoClient---------echo协议客户端



### 3.6 下载安装
下载前请下载或确保已经安装cmake

```
mkdir build
cd build
cmake ..
make & make install
```

