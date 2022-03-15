# 第三章.地址族与数据序列

## 3.1 分配给套接字的ip地址与端口号

IP是为了收发网络数据而分配给计算机的值；端口号是为了区分程序中创建的套接字的序号。

### 3.1.1 网络地址

IP地址分为两类：
* Ipv4：4字节地址
* IPv6：16字节地址

地址分为网络号和主机号：
![Ipv4地址族](https://camo.githubusercontent.com/11262bf4c83f71bf98df9b2dfae6e006335a0d706f5434383919bf4c88ff9ac0/68747470733a2f2f692e6c6f6c692e6e65742f323031392f30312f31332f356333616230656231376262652e706e67)


### 3.1.2 网络地址分类与主机地址边界

只需通过IP地址的第一个字节即可判断网络地址占用的总字节数，因为我们根据IP地址的边界区分网络地址，如下所示：

* A 类地址的首字节范围为：0~127
* B 类地址的首字节范围为：128~191
* C 类地址的首字节范围为：192~223
还有如下这种表示方式：

* A 类地址的首位以 0 开始
* B 类地址的前2位以 10 开始
* C 类地址的前3位以 110 开始

### 3.1.3 用于区分套接字的端口号

端口号用于同一操作系统内区分不同的套接字而设立的。端口号由16位构成，范围为0～65536，但0～1023是知名端口，一般分配给特定应用程序。

*虽然端口号不能重复，但是 TCP 套接字和 UDP 套接字不会共用端接口号，所以允许重复。*

## 3.2 地址信息的表示

### 3.2.1 IPv4地址结构体

结构体的定义:
```c++
struct sockaddr_in
{
    sa_family_t sin_family;  //地址族（Address Family）
    uint16_t sin_port;       //16 位 TCP/UDP 端口号
    struct in_addr sin_addr; //32位 IP 地址
    char sin_zero[8];        //不使用
};
```
该结构体中提到的另一个结构体 in_addr 定义如下，它用来存放 32 位IP地址:
```c++
struct in_addr
{
    in_addr_t s_addr; //32位IPV4地址
}
```

关于以上两个结构体的一些数据类型。

|数据类型名称|	数据类型说明|	声明的头文件|
|:--|:--|:--|
|int 8_t|	signed 8-bit int|	sys/types.h|
|uint8_t|	unsigned 8-bit int (unsigned char)|	sys/types.h|
|int16_t|	signed 16-bit int|	sys/types.h|
|uint16_t|	unsigned 16-bit int (unsigned short)|	sys/|types.h|
|int32_t|	signed 32-bit int|	sys/types.h|
|uint32_t|	unsigned 32-bit int (unsigned long)|	sys/types.h|
|sa_family_t|	地址族（address family）|	sys/socket.h|
|socklen_t|	长度（length of struct）|	sys/socket.h|
|in_addr_t|	IP地址，声明为 uint_32_t|	netinet/in.h|
|in_port_t|	端口号，声明为 uint_16_t|	netinet/in.h|

### 3.2.2 结构体 sockaddr_in 的成员分析
* 成员 sin_family
每种协议适用的地址族不同，比如，IPV4 使用 4 字节的地址族，IPV6 使用 16 字节的地址族。

|地址族（Address Family）|	含义|
|:--|:--|
|AF_INET|	IPV4用的地址族|
|AF_INET6|	IPV6用的地址族|
|AF_LOCAL|	本地通信中采用的 Unix 协议的地址族|

* 成员 sin_port

该成员保存 16 位端口号，重点在于，它以网络字节序保存。

* 成员 sin_addr

该成员保存 32 为IP地址信息，且也以网络字节序保存

* 成员 sin_zero

无特殊含义。只是为结构体 sockaddr_in 结构体变量地址值将以如下方式传递给 bind 函数。

在之前的代码中
```c++
if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    error_handling("bind() error");
```
此处 bind 第二个参数期望得到的是 sockaddr 结构体变量的地址值，包括地址族、端口号、IP地址等。
```c++
struct sockaddr
{
    sa_family_t sin_family; //地址族
    char sa_data[14];       //地址信息
}
```

此结构体 sa_data 保存的地址信息中需要包含IP地址和端口号，剩余部分应该填充 0 ，但是这样对于包含地址的信息非常麻烦，所以出现了 sockaddr_in 结构体，然后强制转换成 sockaddr 类型，则生成符合 bind 条件的参数。

## 3.3 网络字节序和地址变换

### 3.3.1 字节序与网络字节序
CPU 保存数据的方式有两种，这意味着 CPU 解析数据的方式也有 2 种：
* 大端序（Big Endian）：高位字节存放到低位地址
* 小端序（Little Endian）：高位字节存放到高位地址

0x12345678：
* 大端序：0x12 0x34 0x56 0x78（按照计算机读取的顺序）
* 小端序：0x78 0x56 0x34 0x12（按照计算机的顺序进行读取）

网络传输数据时统一约定方式：大端序

### 3.3.2 字节序转换

转换字节序的函数：
```c++
unsigned short htons(unsigned short);
unsigned short ntohs(unsigned short);
unsigned long htonl(unsigned long);
unsigned long ntohl(unsigned long);
```

通过函数名称掌握其功能，只需要了解：

htons 的 h 代表主机（host）字节序。
htons 的 n 代表网络（network）字节序。
s 代表 short
l 代表 long


编写代码：
* [endian_conv.c](https://github.com/caixiongjiang/TCPIP/blob/master/ch03/endian_conv.c)


编译运行
```shell
gcc endian_conv.c -o conv
./conv
```

结果：
```
Host ordered port: 0x1234
Network ordered port: 0x3412
Host ordered address: 0x12345678
Network ordered address: 0x78563412
```

**这是在小端 CPU 的运行结果。大部分人会得到相同的结果，因为 Intel 和 AMD 的 CPU 都是小端序为标准。**

## 3.4 网络地址的初始化和分配

### 3.4.1 将字符串信息转换为网络字节序的整数型

如何把类似于 201.211.214.36 转换为 4 字节的整数类型数据 ?

* 1.inet_addr函数

```c++
#include <arpa/inet.h>
in_addr_t inet_addr(const char* string);
```

如果向该函数传递类似“211.214.107.99”的点分十进制格式的字符串，它会将其转换为32位整数型数据并返回。

具体示例：
[inet_addr.c](https://github.com/caixiongjiang/TCPIP/blob/master/ch03/inet_addr.c)

编译运行：
```shell
gcc inet_addr.c -o addr
./addr
```

结果：
```
Network ordered integer addr: 0x4030201
Error occured!
```


* 2.inet_aton函数

inet_aton 函数与 inet_addr 函数在功能上完全相同，也是将字符串形式的IP地址转换成整数型的IP地址。**只不过该函数用了 in_addr 结构体，如果传递in_addr结构体的变量地址值，函数会自动把结果填入该结构体变量。**

```c++
#include <arpa/inet.h>
int inet_aton(const char *string, struct in_addr *addr);
/*
成功时返回 1 ，失败时返回 0
string: 含有需要转换的IP地址信息的字符串地址值
addr: 将保存转换结果的 in_addr 结构体变量的地址值
*/
```


具体示例：
[inet_aton.c](https://github.com/caixiongjiang/TCPIP/blob/master/ch03/inet_aton.c)

编译运行：
```shell
gcc inet_aton.c -o aton
./aton
```

结果：
```
Network ordered integer addr: 0x4f7ce87f
```

* 还有一个函数，与 inet_aton() 正好相反，它可以把网络字节序整数型IP地址转换成我们熟悉的字符串形式，函数原型如下：

```c++
#include <arpa/inet.h>
char *inet_ntoa(struct in_addr adr);
```

该函数将通过参数传入的整数型IP地址转换为字符串格式并返回。**但要小心，返回值为 char 指针，返回字符串地址意味着字符串已经保存在内存空间，但是该函数未向程序员要求分配内存，而是再内部申请了内存保存了字符串。**也就是说调用了该函数候要立即把信息复制到其他内存空间。因此，若再次调用 inet_ntoa 函数，则有可能覆盖之前保存的字符串信息。

示例：
[inet_ntoa.c](https://github.com/caixiongjiang/TCPIP/blob/master/ch03/inet_ntoa.c)

编译运行：
```shell
gcc inet_ntoa.c -o ntoa
./ntoa
```

结果：
```
Dotted-Decimal notation1: 1.2.3.4
Dotted-Decimal notation2: 1.1.1.1
Dotted-Decimal notation3: 1.2.3.4
```

### 3.4.2 网络地址初始化

常见的套接字创建的过程：
```c++
struct sockaddr_in addr;
char *serv_ip = "211.217,168.13";          //声明IP地址族
char *serv_port = "9190";                  //声明端口号字符串
memset(&addr, 0, sizeof(addr));            //结构体变量 addr 的所有成员初始化为0
addr.sin_family = AF_INET;                 //制定地址族
addr.sin_addr.s_addr = inet_addr(serv_ip); //基于字符串的IP地址初始化
addr.sin_port = htons(atoi(serv_port));    //基于字符串的IP地址端口号初始化
```