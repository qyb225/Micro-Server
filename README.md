# Micro-Server

---

简易 Linux C++ Web Server。 ~~4 个工作进程监听 TCP 连接；~~使用 epoll 实现 I/O 多路复用。

Http 协议解析和响应功能还不完善。

---


```
g++ -std=c++11 server_run.cpp Server.cpp HttpRes.cpp HttpReq.cpp -o server.out
./server.out 8080
```

浏览器：http://127.0.0.1:8080

---

多进程出现**群惊问题**：一个请求发过来，操作系统又无从判断由谁来负责accept，因此会唤醒所有子进程，但最终只会有一个进程成功accept，其他进程accept失败。但是这样会使得其它子进程没必要的浪费。