#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

void *request_handler(void *arg);
void send_data(FILE *fp, char *ct, char *file_name);
char *content_type(char *file);
void send_error(FILE *fp);
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    int clnt_adr_size;
    char buf[BUF_SIZE];
    pthread_t t_id;
    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));
    if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    if (listen(serv_sock, 20) == -1)
        error_handling("listen() error");

    while (1)
    {
        clnt_adr_size = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_size);
        //输出连接的IP地址和端口号 
        //inet_ntoa:将32位网络字节序转化为字符串形式的IP地址
        //ntohs：把short型数据从网络字节序转化为主机字节序
        printf("Connection Request : %s:%d\n",
               inet_ntoa(clnt_adr.sin_addr), ntohs(clnt_adr.sin_port));
        //线程创建， 参数1--线程ID 参数2--NULL代表默认类型的线程
        //参数3--线程函数 参数4--线程函数传递的参数
        pthread_create(&t_id, NULL, request_handler, &clnt_sock);
        //删除线程
        pthread_detach(t_id);
    }
    close(serv_sock);
    return 0;
}

//请求处理函数
void *request_handler(void *arg)
{
    int clnt_sock = *((int *)arg);
    char req_line[SMALL_BUF];
    FILE *clnt_read;
    FILE *clnt_write;

    char method[10];
    char ct[15];
    char file_name[30];

    //标准I/O函数fdopen 不需要在接收的数据尾部插入0，因为可以按字符串单位进行数据交换  
    //dup函数复制文件描述符
    clnt_read = fdopen(clnt_sock, "r");
    clnt_write = fdopen(dup(clnt_sock), "w");
    fgets(req_line, SMALL_BUF, clnt_read);//读取请求行
    //strstr函数：返回字符串中首次出现子串的地址。
    if (strstr(req_line, "HTTP/") == NULL)
    {
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return;
    }
    //将读取的请求行复制到该线程函数分别定义的三个数组中
    //strtok函数：根据参数2对参数1的字符串进行分割
    strcpy(method, strtok(req_line, " /"));
    strcpy(file_name, strtok(NULL, " /"));
    strcpy(ct, content_type(file_name));
    //方法错误，提示错误
    if (strcmp(method, "GET") != 0)
    {
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return;
    }
    fclose(clnt_read);
    //向网页客户端发送网页消息 
    send_data(clnt_write, ct, file_name);
}

//发送网页客户端发送文件消息
void send_data(FILE *fp, char *ct, char *file_name)
{
    char protocol[] = "HTTP/1.0 200 OK\r\n";//成功处理了请求
    char server[] = "Server:Linux Web Server \r\n";//服务器端名为Linux Web Server
    char cnt_len[] = "Content-length:2048\r\n";//数据长度为2048字节
    char cnt_type[SMALL_BUF];
    char buf[BUF_SIZE];
    FILE *send_file;//定义发送文件的指针

    //cnt_type的内容为 "Content-type:%s\r\n\r\n"中的%s用ct代入
    sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
    send_file = fopen(file_name, "r");//只写打开
    if (send_file == NULL)
    {
        send_error(fp);
        return;
    }

    //传输头信息
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);

    //传输请求数据
    while (fgets(buf, BUF_SIZE, send_file) != NULL)
    {
        fputs(buf, fp);
        fflush(fp);
    }
    fflush(fp);
    fclose(fp);
}

char *content_type(char *file)
{
    char extension[SMALL_BUF];
    char file_name[SMALL_BUF];
    strcpy(file_name, file);
    strtok(file_name, ".");//得到文件名的"."前的字符
    strcpy(extension, strtok(NULL, "."));//继续获取由"."分割而来的字符串

    if (!strcmp(extension, "html") || !strcmp(extension, "htm"))
        return "text/html";
    else
        return "text/plain";
}

//向网页输出错误，并提示检查格式
void send_error(FILE *fp)
{
    char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cnt_len[] = "Content-length:2048\r\n";
    char cnt_type[] = "Content-type:text/html\r\n\r\n";
    char content[] = "<html><head><title>NETWORK</title></head>"
                     "<body><font size=+5><br>发生错误！ 查看请求文件名和请求方式!"
                     "</font></body></html>";
    /*将上述内容输出到fp指针所指的对象中去*/
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    //刷新缓冲区（标准I/O提供额外的缓冲）
    fflush(fp);
}
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}