/******************************************************************************
 * @file       global.h
 * @brief      全局使用的头文件、函数……
 * @author     ggggtxgt
 * @date       2025/12/8 22:46
 * @history
*****************************************************************************/
#ifndef QTCHAT_GLOBAL_H
#define QTCHAT_GLOBAL_H

#include <QDir>
#include <mutex>
#include <memory>
#include <QDebug>
#include <QStyle>
#include <QWidget>
#include <iostream>
#include <QSettings>
#include <QByteArray>
#include <functional>
#include <QJsonObject>
#include <QCryptographicHash>
#include <QRegularExpression>

// repolish 用于刷新 qss 样式
extern std::function<void(QWidget *)> repolish;

// 发送的网络前缀
extern QString gate_url_prefix;

// 通过md5进行加密
extern QString md5Encrypt(const QString &input);

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

// 错误提示的枚举类型
enum TipErr{
    TIP_SUCCESS = 0,
    TIP_EMAIL_ERR = 1,
    TIP_PWD_ERR = 2,
    TIP_CONFIRM_ERR = 3,
    TIP_PWD_CONFIRM = 4,
    TIP_VARIFY_ERR = 5,
    TIP_USER_ERR = 6
};

// label 的两种基本状态
enum ClickLbState{
    Normal = 0,
    Selected = 1
};

#endif //QTCHAT_GLOBAL_H
