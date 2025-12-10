#ifndef GATESERVER_SINGLETON_H
#define GATESERVER_SINGLETON_H

#include <mutex>
#include <memory>
#include <iostream>

template<typename T>
class Singleton {
public:
    static std::shared_ptr<T> GetInstance() {
        static std::once_flag s_flag;
        std::call_once(s_flag, [&]() {

            _instance = std::shared_ptr<T>(new T);
        });
        return _instance;
    }

    void PrintAddress() {
        std::cout << _instance.get() << std::endl;
    }

    ~Singleton() {
        std::cout << "this is singleton destruct" << std::endl;
    }

protected:
    // 使用默认构造函数
    Singleton() = default;

    // 删除拷贝构造函数
    Singleton(const Singleton<T> &) = delete;

    // 删除赋值构造函数
    Singleton &operator=(const Singleton<T> &) = delete;

    // 单例类的实例，使用静态成员变量确保整个程序生命周期内只有一个实例
    static std::shared_ptr<T> _instance;
};

template<typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

#endif //GATESERVER_SINGLETON_H
