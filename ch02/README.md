# 第二章.套接字类型和协议设置

## 2.1 套接字类型和数据传输特性

如果两个端口需要通信，必须使用相同的协议。协议就是为了完成数据交换定好的约定。

### 2.1.1 创建套接字

```c
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
/*
成功时返回文件描述符，失败时返回-1
domain: 套接字中使用的协议族（Protocol Family）
type: 套接字数据传输的类型信息
protocol: 计算机间通信中使用的协议信息
*/
```

### 2.1.2 协议族

协议族其实就是协议分类信息。

头文件`sys/socket.h`中声明的协议族

|名称	|协议族|
|:--|:--|
|PF_INET|	IPV4互联网协议族|
|PF_INET6|	IPV6互联网协议族|
|PF_LOCAL|	本地通信Unix协议族|
|PF_PACKET|	底层套接字的协议族|
|PF_IPX|	IPX Novel协议族|

本书着重讲 PF_INET 对应的 IPV4 互联网协议族。其他协议并不常用，或并未普及。**另外，套接字中采用的最终的协议信息是通过 socket 函数的第三个参数传递的。在指定的协议族范围内通过第一个参数决定第三个参数。**

### 2.1.3 面向连接的套接字（SOCK_DGRAM）

首先说明，socket函数的第一个参数决定使用的协议族，第二个参数指定数据传输类型。**也就是说一个协议族中有多种传输方式。**

面向连接的数据传输方式特征（有连接）：
* 传输过程中数据不会消失
* 按序传输数据
* 传输的数据不存在数据边界（Boundary）

**这里的不存在数据边界是指接收方会制定数据的大小，但发送方不需要一次性发送这么大的消息，可以分多次传输。接收方可以在没有达到指定大小就接受数据，也可以收集够指定大小的数据才会打包接收数据！**

收发数据的套接字内部有缓冲（buffer），简言之就是字节数组。只要不超过数组容量，那么数据填满缓冲后过 1 次 read 函数的调用就可以读取全部，也有可能调用多次来完成读取。

套接字缓冲已满是否意味着数据丢失？
```
缓冲区满了，套接字无法再接收数据，但此时传输端套接字也将停止传输数据！因此不会发生数据丢失
```

**套接字必须一一对应，也就是说发送端和接收端的套接字类型和特性必须一致！**

### 2.1.4 面向消息的套接字

* 强调快速传输而非传输有序
* 传输的数据可能丢失也可能损毁
* 传输的数据有边界
* 限制每次传输数据的大小

这是一种不可靠的连接（其实是无连接），也就是不能保证数据丢失或者损坏。每次传输的大小有一定的限制（因为本身是不可靠传输，减小每次传输数据大小有利于减少数据丢失），最重要的是发送方发送几次数据，接收方也要接受几次数据（有数据边界）。

### 2.1.5 协议的最终选择

这里指socket函数的第三个参数，决定使用最终采用的协议。**也就是说其实同一协议族种存在传输方式相同的不同的协议。**

* 如果协议相同，或者只有一个协议：参数可以为0
* 如果有不同的传输方式相同的协议：需要指定不同的参数
    * IPPROTO_TCP:
    ```c
    //ipv4中面向连接的套接字
    int tcp_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_TCP);
    ```
    * IPPROTO_UDP:
    ```c
    //ipv4中面向消息的套接字
    int udp_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    ```

### 2.1.6 面向连接的套接字：TCP套接字示例

在第一章的代码做出修改，
* 服务端没有变化：[tcp_server.c](https://github.com/caixiongjiang/TCPIP/blob/master/ch02/tcp_server.c)
* 客户端更改read函数调用的方式（分多次调用read函数）：[tcp_client](https://github.com/caixiongjiang/TCPIP/blob/master/ch02/tcp_client.c)

实操：
* 服务器端：
```shell
gcc tcp_server.c -o tserver
./tserver 9190
```

* 客户端(这里的ip指服务端的本地ip地址)：
```shell
gcc tcp_client.c -o tclient
./tclient 172.16.127.128 9190
```

结果：
服务端出现：
```shell
Message from server : Hello World! 
Function read call count: 13 #13个字节
```

## 2.2 Windows平台下的实现

略，用到时再学！

## 2.3 课后作业第六题

修改前面的tcp_server.c和tcp_client.c，使服务端分多次调用write函数传输数据，客户端只调用一次读取。

编写文件：
* [tcp_server_kehou.c](https://github.com/caixiongjiang/TCPIP/blob/master/ch02/tcp_server_kehou.c)
* [tcp_client_kehou.c](https://github.com/caixiongjiang/TCPIP/blob/master/ch02/tcp_client_kehou.c)

实操：
* 服务端：
```shell
gcc tcp_server_kehou.c -o tserverkh
./tserverkh 9090
```

* 客户端
```shell
gcc tcp_client_kehou.c -o tclientkh
./tclientkh 172.16.127.128 9090
```

结果：
客户端出现：
```
# 等待3000ms
wait time 0
wait time 1
···
wait time 2999
Message from server : Hello World!
```