#ifndef COMM_H_
#define COMM_H_

/// 服务器连接客户端
/// - here_port: 绑定的端口
int host_get_sock_fd(char *here_port);

/// 客户端连接服务器
/// - there_ip: 对方的 ip
/// - there_port: 对方绑定的端口
int guest_get_sock_fd(char *there_ip, char *there_port);

/// 发送数据
/// - fd: 文件描述符
/// - buff: 待发送数据的地址
/// - size: 待发送数据的大小(byte)
int send_data(int fd, void *buff, int size);

/// 接收数据
/// - fd: 文件描述符
/// - buff: 待接收数据的地址
/// - size: 待接收数据的大小(byte)
int recv_data(int fd, void *buff, int size);

#endif
