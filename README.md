# connect4

## 依赖

Debian 或 Ubuntu 上需要下载：

```console
$ sudo apt install libjpeg-dev libpng-dev libfreetype-dev
```

## 编译

分为两个版本，SDL2 版本和 framebuffer 版本，SDL2 版本能在图形环境下运行。

编译 SDL2 版本：

```console
$ make simulate=on
```

编译 framebuffer 版本：

```console
$ make
```

## 运行

热座模式，既两位玩家轮流操控同一设备：

```console
$ ./connect4
```

联机模式，即两位玩家在不同的设备上对战。

作为服务器的玩家选择一个喜欢的端口：

```console
$ ./connect host {HOST_PORT}
```

作为客户端的玩家要知晓服务器的 IP 和端口：

```console
$ ./connect guest {HOST_IP} {HOST_PORT}
```

## 效果

![demo](./images/demo.gif)

