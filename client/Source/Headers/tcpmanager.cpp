#include <QJsonDocument>
#include "tcpmanager.h"


TcpManager::TcpManager() : _host(""), _port(0), _b_recv_pending(false),
                           _message_id(0), _message_len(0) {
    QObject::connect(&_socket, &QTcpSocket::connected, [&]() {
        qDebug() << "Connected to server!";
        // 连接建立之后发送消息
        emit signal_con_success(true);
    });

    QObject::connect(&_socket, &QTcpSocket::readyRead, [&]() {
        // 有数据可读时，读取所有数据，并追加到缓冲区
        _buffer.append(_socket.readAll());

        QDataStream stream(&_buffer, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_5_0);

        forever {
            // 先解析头部
            if (!_b_recv_pending) {
                // 检查缓冲区中的数据是否足够解析得出一个消息头(消息ID + 消息长度)
                if (_buffer.size() < static_cast<int>(sizeof(quint16) * 2)) {
                    // 数据不够，等待更多数据
                    return;
                }
                // 预先读取消息ID和消息长度，但不从缓冲区中移除
                stream >> _message_id >> _message_len;

                // 将buffer中的前四个字节移除
                _buffer = _buffer.mid(sizeof(quint16) * 2);
                // 输出读取的数据
                qDebug() << "Message id is: " << _message_id << ", len is: " << _message_len;
            }
            // buffer剩余长度是否满足消息体长度，不满足则退出继续等待接收
            if (_buffer.size() < _message_len) {
                _b_recv_pending = true;
                return;
            }
            _b_recv_pending = false;
            // 读取消息体
            QByteArray body = _buffer.mid(0, _message_len);
            qDebug() << "receive body message is: " << body;
            _buffer = _buffer.mid(_message_len);
        }
    });

    // 处理错误，适用于 Qt5.15 之前的版本
    QObject::connect(&_socket, static_cast<void (QTcpSocket::*)(QTcpSocket::SocketError)>(&QTcpSocket::error),
                     [&](QTcpSocket::SocketError error) {
                         qDebug() << "Error is: " << _socket.errorString();
                         switch (error) {
                             case QTcpSocket::ConnectionRefusedError:
                                 qDebug() << "Connection Refused!";
                                 emit signal_con_success(false);
                                 break;
                             case QTcpSocket::RemoteHostClosedError:
                                 qDebug() << "Remote Host Closed Connection!";
                                 break;
                             case QTcpSocket::HostNotFoundError:
                                 qDebug() << "Host Not found!";
                                 emit signal_con_success(false);
                                 break;
                             case QTcpSocket::SocketTimeoutError:
                                 qDebug() << "Connection Timeout!";
                                 emit signal_con_success(false);
                                 break;
                             case QTcpSocket::NetworkError:
                                 qDebug() << "Network Error!";
                                 break;
                             default:
                                 qDebug() << "Other Error!";
                                 break;
                         }
                     });
    // 处理连接断开
    QObject::connect(&_socket, &QTcpSocket::disconnected, [&]() {
        qDebug() << "Disconnected from server.";
    });
    // 连接发送信号，用于发送数据
    QObject::connect(this, &TcpManager::signal_send_data, this, &TcpManager::slot_send_data);
    // 注册消息
    initHandlers();
}

void TcpManager::slot_tcp_connect(ServerInfo si) {
    qDebug() << "receive tcp connect signal";
    // 尝试连接到服务器
    qDebug() << "Connecting to server...";
    _host = si.Host;
    _port = static_cast<uint16_t>(si.Port.toUInt());
    _socket.connectToHost(si.Host, _port);
}

TcpManager::~TcpManager() {

}

void TcpManager::slot_send_data(RequestId rid, QString data) {
    uint16_t id = rid;
    // 将字符串转换为 UTF-8 编码的字节数组
    QByteArray byte = data.toUtf8();
    // 计算长度（使用网络字节序转换）
    quint16 len = static_cast<quint16>(data.size());
    // 创建一个 QByteArray 用于存储需要发送的所有数据
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    // 设置数据流使用网络字节序
    out.setByteOrder(QDataStream::BigEndian);
    // 写入ID和长度
    out << id << len;
    // 添加字符串数据
    block.append(data);
    // 发送数据
    _socket.write(block);
}

void TcpManager::handleMessage(RequestId id, int len, QByteArray data) {
    auto find_iter = _handlers.find(id);
    if (find_iter == _handlers.end()) {
        qDebug() << "not found id [" << id << "] to handle";
        return;
    }
    // 调用 _handlers 中的 lambda 表达式
    find_iter.value()(id, len, data);
}

void TcpManager::initHandlers() {
    _handlers.insert(ID_CHAT_LOGIN_RSP, [this](RequestId id, int len, QByteArray data) {
        Q_UNUSED(len)
        qDebug() << "handle id is: " << id << " , data is: " << data;
        // 将 QByteArray 转换为 QJsonDocument
        QJsonDocument json = QJsonDocument::fromJson(data);

        // 检查转换是否成功
        if (json.isNull()) {
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }
        QJsonObject obj = json.object();
        if (!obj.contains("error")) {
            int err = ErrorCodes::ERROR_JSON;
            qDebug() << "Login Failed, err is Json parse Err: " << err;
            emit signal_login_failed(err);
            return;
        }
        int err = obj["error"].toInt();
        if (err != ErrorCodes::SUCCESS) {
            qDebug() << "Login Failed, err is: " << err;
            emit signal_login_failed(err);
            return;
        }

//        UserManager::GetInstance()->SetUid(obj["uid"].toInt());
//        UserManager::GetInstance()->SetName(obj["name"].toInt());
//        UserManager::GetInstance()->SetToken(obj["token"].toInt());
        emit signal_switch_chatdlg();
    });
}
