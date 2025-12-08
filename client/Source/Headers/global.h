/******************************************************************************
 * @file       global.h
 * @brief      全局使用的头文件、函数……
 * @author     ggggtxgt
 * @date       2025/12/8 22:46
 * @history
*****************************************************************************/
#ifndef QTCHAT_GLOBAL_H
#define QTCHAT_GLOBAL_H

#include <QStyle>
#include <QWidget>
#include <functional>
#include <QRegularExpression>

// repolish 用于刷新 qss 样式
extern std::function<void(QWidget *)> repolish;

#endif //QTCHAT_GLOBAL_H
