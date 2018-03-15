# Micro-Server

---

简易 Linux C++ Web Server。 4 个工作进程监听 TCP 连接；使用 epoll 实现 I/O 多路复用。

Http 协议解析和响应功能还不完善。

---


```
g++ -std=c++11 server_run.cpp Server.cpp HttpRes.cpp HttpReq.cpp -o server.out
./server.out 8080
```

浏览器：http://127.0.0.1:8080

