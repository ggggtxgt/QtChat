/******************************************************************************
 * @file       singleton.h
 * @brief      单例模式基类
 * @author     ggggtxgt
 * @date       2025/12/8 23:43
 * @history
*****************************************************************************/
#ifndef QTCHAT_SINGLETON_H
#define QTCHAT_SINGLETON_H

#include "global.h"

template<typename T>
class Singleton {
public:
    // 双重检测可能出现安全风险，此处不使用
    // 使用 C++ 最安全的方式实现（使用 once_flag）
    static std::shared_ptr<T> GetInstance() {
        static std::once_flag s_flag;
        std::call_once(s_flag, [&]() {
            // 为何使用 new，而不使用 make_shared？？？
            // make_shared 是一个独立函数，不是类的成员，其在函数内部调用构造函数时，需要构造函数是 public
            // 但是 new 表达式是在类的成员函数内执行，可以访问私有成员
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

// 类的静态成员变量必须被初始化
// 若为模板类，则在类外部初始化；否则在类内部初始化
template<typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

#endif //QTCHAT_SINGLETON_H
