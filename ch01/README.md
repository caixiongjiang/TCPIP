# 理解网络编程和套接字

系统环境：CentOS 7
宿主机：macOS Big Sur 

## 理解网络编程和套接字

### “打电话”过程
服务器端：
* 1.调用socket函数**创建套接字**
* 2.调用bind函数**分配IP地址和端口号**
* 3.调用listen函数**转为可接收请求状态**
* 4.调用accept函数**受理连接请求**

客户端：
* 1.调用socket函数**创建套接字**
* 2.调用connect函数**向服务器端发送连接请求**

### 实操

文件编写：
* [hello_server.c]()
* [hello_client.c]()

进入文件所在的根目录，编译并进行“打电话”：
* 服务器端：
```shell
gcc hello_server.c -o hserver  //编译生成hserver
./hserver 9190  //运行hserver生成9190端口等待客户端的连接请求
```
* 客户端(这里的ip地址为服务器端的ip地址)：
```shell
gcc hello_client.c -o hclient  //编译生成hserver
./hclient 172.16.127.128 9190   //运行hserver生成9190端口等待客户端的连接请求
```

注意：

1.服务端和客户端要分开：如果为同一台电脑，可以使用两个终端进行；如果不同的电脑需要搞清楚输入ip为**服务端的本地ip**。

2.通过终端ifcong，查看分配的本地ip