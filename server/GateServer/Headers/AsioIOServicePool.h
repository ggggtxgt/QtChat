#ifndef GATESERVER_ASIOIOSERVICEPOOL_H
#define GATESERVER_ASIOIOSERVICEPOOL_H

#include <vector>
#include <boost/asio.hpp>

#include "Singleton.h"

class AsioIOServicePool : public Singleton<AsioIOServicePool> {
    friend Singleton<AsioIOServicePool>;
public:
    // 将类型重命名
    using IOService = boost::asio::io_context;
    // 通过 work 绑定 io_context 可以使得 io_context 即使没有事件时，也不会退出
    using Work = boost::asio::io_context::work;
    using WorkPtr = std::unique_ptr<Work>;

    ~AsioIOServicePool();

    AsioIOServicePool(const AsioIOServicePool &) = delete;

    AsioIOServicePool &operator=(const AsioIOServicePool &) = delete;

    // 使用 round-robin 的方式返回一个 io_service
    boost::asio::io_context &GetIOService();

    // 停止服务池，同时回收资源，唤醒挂起的进程
    void Stop();

private:
    AsioIOServicePool(std::size_t size = 2);

    // 不支持拷贝的成员变量，都应放在初始化列表
    std::vector<IOService> _io_services;
    std::vector<WorkPtr> _works;
    std::vector<std::thread> _threads;
    std::size_t _next_IOService;            // 下一个 IOContext 的索引
};

#endif //GATESERVER_ASIOIOSERVICEPOOL_H
