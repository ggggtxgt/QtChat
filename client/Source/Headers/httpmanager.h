/******************************************************************************
 * @file       httpmanager.h
 * @brief      Http管理者头文件
 * @author     ggggtxgt
 * @date       2025/12/9 08:51
 * @history
*****************************************************************************/
#ifndef QTCHAT_HTTPMANAGER_H
#define QTCHAT_HTTPMANAGER_H

#include <QUrl>
#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

#include "global.h"
#include "singleton.h"

// CRTP 奇异递归模板
class HttpManager : public QObject, public Singleton<HttpManager>, public std::enable_shared_from_this<HttpManager> {
Q_OBJECT
    // 为何构造函数、析构函数需要分别指定为 public private？？？
    // 构造函数指定为 private：单例构造函数必须指定为私有
    // 析构函数指定为 public：单例类的唯一实例为 _instance，其为 shared_ptr<T> 类型
    // 最终需要调用的构造函数为 T 类型的析构函数，即：需要调用 HttpManager 的析构函数
public:
    ~HttpManager();

    void PostHttpRequest(QUrl url, QJsonObject json, RequestId id, Modules module);

private:
    // 为何需要使用友元？？？
    // 在 Singleton 类中，_instance = std::shared_ptr<T>(new T);
    // 需要调用 T 类型的构造函数，即：需要调用 HttpManager 的构造函数
    friend class Singleton<HttpManager>;

    HttpManager();

private:
    QNetworkAccessManager _manager;

private slots:

    void slot_http_finish(RequestId id, QString str, ErrorCodes code, Modules module);

signals:

    void signal_http_finish(RequestId id, QString str, ErrorCodes code, Modules module);

    void signal_register_finish(RequestId id, QString str, ErrorCodes code);

    void signal_reset_finish(RequestId id, QString str, ErrorCodes code);
};


#endif //QTCHAT_HTTPMANAGER_H
