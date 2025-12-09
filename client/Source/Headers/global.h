/******************************************************************************
 * @file       global.h
 * @brief      全局使用的头文件、函数……
 * @author     ggggtxgt
 * @date       2025/12/8 22:46
 * @history
*****************************************************************************/
#ifndef QTCHAT_GLOBAL_H
#define QTCHAT_GLOBAL_H

#include <mutex>
#include <memory>
#include <QStyle>
#include <QWidget>
#include <iostream>
#include <QByteArray>
#include <functional>
#include <QRegularExpression>

// repolish 用于刷新 qss 样式
extern std::function<void(QWidget *)> repolish;

enum RequestId {
    ID_GET_VARIFY_CODE = 1001,      // 获取验证码
    ID_REGISTER_USER = 1002,        // 注册用户
};

enum Modules {
    REGISTERMOD = 0,                // 注册模块
};

enum ErrorCodes {
    SUCCESS = 0,                    // 成功
    ERROR_JSON = 1,                 // json 解析失败
    ERROR_NETWORK = 2,              // 网络错误
};

#endif //QTCHAT_GLOBAL_H
