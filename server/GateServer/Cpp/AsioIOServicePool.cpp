#include <iostream>

#include "../Headers/AsioIOServicePool.h"

AsioIOServicePool::AsioIOServicePool(std::size_t size) : _io_services(size), _works(size), _next_IOService(0) {
    // 绑定 io_context 和 work，使得即使没有事件，io_service 也不会退出
    // 轮循 io_context 下标
    for (std::size_t i = 0; i < size; i++) {
        _works[i] = std::unique_ptr<Work>(new Work(_io_services[i]));
    }

    // 遍历多个 io_service，创建多个线程，每个线程内部启动 io_service
    for (std::size_t i = 0; i < _io_services.size(); i++) {
        _threads.emplace_back([this, i]() {
            _io_services[i].run();
        });
    }
}

AsioIOServicePool::~AsioIOServicePool() {
    std::cout << "AsioIoServicePool destruct" << std::endl;
}

boost::asio::io_context &AsioIOServicePool::GetIOService() {
    // 较为平均地使用每个 io_service
    auto &service = _io_services[_next_IOService++];
    if (_next_IOService == _io_services.size()) {
        _next_IOService = 0;
    }
    return service;
}

void AsioIOServicePool::Stop() {
    // 由于只执行 work.reset() 不能使得 io_context 从 run 的状态中退出
    // 当 io_context 已经绑定读或写的监听事件之后，需要手动关闭该服务
    for (auto &work: _works) {
        // 关闭该服务
        work->get_io_context().stop();
        work.reset();
    }

    // 等待线程退出
    for (auto &t: _threads) {
        t.join();
    }
}