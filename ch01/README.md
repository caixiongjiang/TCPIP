# 第一章.理解网络编程和套接字

系统环境：CentOS 7
宿主机：macOS Big Sur 

## 1.1 理解网络编程和套接字

### 1.1.1 “打电话”过程
服务器端：
* 1.调用socket函数**创建套接字**
* 2.调用bind函数**分配IP地址和端口号**
* 3.调用listen函数**转为可接收请求状态**
* 4.调用accept函数**受理连接请求**

客户端：
* 1.调用socket函数**创建套接字**
* 2.调用connect函数**向服务器端发送连接请求**

### 1.1.2 实操

文件编写：
* [hello_server.c](https://github.com/caixiongjiang/TCPIP/blob/master/ch01/hello_server.c)
* [hello_client.c](https://github.com/caixiongjiang/TCPIP/blob/master/ch01/hello_client.c)

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

结果：服务器端输出“Hello World!”

注意：

1.服务端和客户端要分开：如果为同一台电脑，可以使用两个终端进行；如果不同的电脑需要搞清楚输入ip为**服务端的本地ip**。

2.通过终端ifcong，查看分配的本地ip

3.顺序一定是先服务器端再客户端

4.运行一次过后就需要更改服务器的端口号

## 1.2 基于Linux的文件操作

在Linux中，socket也被认为是一种文件，因此在网络数据传输时可以使用文件I/O的相关函数，而Windows需要调用特殊的数据传输相关的函数。

### 1.2.1 文件描述符

文件描述符是指为了方便称呼操作系统创建的文件或套接字而赋予的数。

### 1.2.2 打开/关闭/读/写文件/文件描述符和套接字

* 打开文件用open函数

打开模式有5种：
|打开模式|含义|
|:--|:--|
|O_CREAT|必要时创建文件|
|O_TRUNC|删除全部现有数据|
|O_APPEND|维持现有数据，保存到其后面|
|O_RDONLY|只读打开|
|O_WRONLY|只写打开|
|O_RDWR|读写打开|

* 关闭文件使用close函数
* 数据写入使用write函数
* 数据读取使用read函数
* 创建文件和套接字，并用整数形态比较返回的文件描述符值

### 1.2.3 实操

文件编写：
* [low_open.c](https://github.com/caixiongjiang/TCPIP/blob/master/ch01/low_open.c)
* [low_read.c](https://github.com/caixiongjiang/TCPIP/blob/master/ch01/low_read.c)
* [fd_seri.c](https://github.com/caixiongjiang/TCPIP/blob/master/ch01/fd_seri.c)

进入文件所在的根目录，编译并进行“读/写”：

* 写：
```shell
gcc low_open.c -o lopen
./lopen
cat data.txt  //输出文件的内容
```

结果：
```
file description: 3
Let's go!
```

* 读：
```shell
gcc low_read.c -o lread
./lread
```
结果：
```
file description: 3
Let's go!
```

* 同时创建文件和套接字，并用整数形态比较返回的文件描述符
```shell
gcc fd_seri.c -o fds
./fds
```

结果：
```
file descriptor 1: 3
file descriptor 2: 4
file descriptor 3: 5
```

## 1.3 基于Windows平台的实现

暂时先不学，需要用了再学！

## 课后练习7

使用底层I/O编写文件复制程序,这里复制的是之前的data.txt。

编写文件：
* [low_copy.c]()

进入文件路径下，编译并运行：
```shell
gcc low_copy.c -o lcopy
./lcopy

cat copy.txt
```

结果：
```
file descriptor: 3 
file descriptor: 4 

Let's go!
���@P^���@�@0_��
```

前面的复制对了，不知道后面的没有文字怎么出现的乱码，奇怪，难道我写的[low_copy.c]()有问题，如果有大神能解决可以帮我看一下，十分感谢！