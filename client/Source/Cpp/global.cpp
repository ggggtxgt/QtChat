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

extern QString md5Encrypt(const QString &input) {
    // 将输入字符转换为字节数组
    QByteArray array = input.toUtf8();
    // 使用md5进行加密
    QByteArray hash = QCryptographicHash::hash(array, QCryptographicHash::Md5);
    // 返回十六进制的加密结果
    return QString(hash.toHex());
}