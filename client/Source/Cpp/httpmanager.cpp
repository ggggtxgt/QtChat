/******************************************************************************
 * @file       httpmanager.cpp
 * @brief      Http管理者源文件
 * @author     ggggtxgt
 * @date       2025/12/9 08:52
 * @history    
*****************************************************************************/
#include "../Headers/httpmanager.h"

HttpManager::~HttpManager() {

}

HttpManager::HttpManager() {
    // 连接http请求和完成信号，信号槽机制保证队列消费
    connect(this, &HttpManager::signal_http_finish, this, &HttpManager::slot_http_finish);
}

void HttpManager::PostHttpRequest(QUrl url, QJsonObject json, RequestId id, Modules module) {
    QByteArray data = QJsonDocument(json).toJson();
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.length()));

    // 为何使用 shared_from_this？？？
    // 由于下方 Lambda 表达式中使用回调，捕获 this，必须保证在回调之前 HttpManager 不被删除
    // 因此使用 shared_from_this （引用计数 shared_ptr）保证不被删除
    auto self = shared_from_this();
    QNetworkReply *reply = _manager.post(request, data);
    QObject::connect(reply, &QNetworkReply::finished, [self, reply, id, module]() {
        // 处理错误情况
        if (QNetworkReply::NoError != reply->error()) {
            qDebug() << reply->errorString();

            // 发送信号通知完成
            emit self->signal_http_finish(id, "", ErrorCodes::ERROR_NETWORK, module);
            reply->deleteLater();
            return;
        }

        // 没有错误
        QString res = reply->readAll();

        // 发送信号通知完成
        emit self->signal_http_finish(id, res, ErrorCodes::SUCCESS, module);
        return;
    });
}

void HttpManager::slot_http_finish(RequestId id, QString str, ErrorCodes code, Modules module) {
    if (Modules::REGISTERMOD == module) {
        // 发送信号通知指定模块 http 的响应已经结束
        emit signal_register_finish(id, str, code);
    }

    if (Modules::RESETMOD == module) {
        // 发送信号通知指定模块，http 响应结束
        emit signal_reset_finish(id, str, code);
    }
}