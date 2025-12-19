#ifndef QTCLIENT_TCPMANAGER_H
#define QTCLIENT_TCPMANAGER_H

#include "global.h"
#include "singleton.h"

#include <QTcpSocket>

class TcpManager : public QObject, public Singleton<TcpManager>,
                   public std::enable_shared_from_this<TcpManager> {
Q_OBJECT

    friend class Singleton<TcpManager>;

public:
    ~TcpManager();

private:
    TcpManager();

    void initHandlers();

    void handleMessage(RequestId id, int len, QByteArray data);

private:
    QString _host;
    uint16_t _port;
    QTcpSocket _socket;
    QByteArray _buffer;
    quint16 _message_id;
    quint16 _message_len;
    bool _b_recv_pending;
    QMap<RequestId, std::function<void(RequestId id, int len, QByteArray data)>> _handlers;
public slots:

    void slot_tcp_connect(ServerInfo);

    void slot_send_data(RequestId id, QString data);

signals:

    void signal_con_success(bool success);

    void signal_send_data(RequestId id, QString data);

    void signal_switch_chatdlg();

    void signal_login_failed(int);

};

#endif //QTCLIENT_TCPMANAGER_H
