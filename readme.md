# 简介

本项目为C++全栈聊天项目实战，包括PC端QT界面编程，asio异步服务器设计，beast网络库搭建http网关，node.js搭建验证服务，各服务间通过grpc通信，server和client使用asio通信等。实现跨平台设计，先设计windows的server，之后再考虑移植到windows中，较为全面的展示C++ 在实际项目中的应用。

# 架构设计

![架构设计](./client/res/架构设计.jpg)

1. GateServer：网关服务，主要应对客户端的连接和注册请求，由于服务器为分布式，因此GateServer收到用户连接请求之后将会查询状态服务选择负载较小的Server地址传给客户端，客户端使用该地址直接和Server通信建立长连接。
2. 当用户注册时发送到GateServer，GateServer调用VarifyServer验证注册的合理性并发送验证码给客户端，客户端使用该验证码在GateServer注册即可。
3. StatusServer，ServerA，ServerB都可以直接访问Redis和Mysql服务。

# 其他

- 项目原作者：[恋恋风辰zack](https://space.bilibili.com/271469206/?spm_id_from=333.788.upinfo.detail.click)
- 本人的笔记地址：[飞书云文档](https://ai.feishu.cn/drive/folder/UHd8fVG9mlsybwd21FXcEo0unUh?from=space_personal_filelist)