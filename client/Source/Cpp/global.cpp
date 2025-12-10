/******************************************************************************
 * @file       global.cpp
 * @brief      全局通用函数的实现
 * @author     ggggtxgt
 * @date       2025/12/8 22:51
 * @history
*****************************************************************************/
#include "../Headers/global.h"

QString gate_url_prefix = "";

std::function<void(QWidget *)> repolish = [](QWidget *widget) {
    // 取消当前的样式，重新加载新样式
    widget->style()->unpolish(widget);
    widget->style()->polish(widget);
};