/******************************************************************************
 * @file       logindialog.h
 * @brief      登录窗口头文件
 * @author     ggggtxgt
 * @date       2025/12/8 19:42
 * @history
*****************************************************************************/
#ifndef QTCHAT_LOGINDIALOG_H
#define QTCHAT_LOGINDIALOG_H

#include <QDialog>

#include "global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LoginDialog; }
QT_END_NAMESPACE

class LoginDialog : public QDialog {
Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);

    ~LoginDialog() override;

private:
    int _uid;
    QString _token;
    Ui::LoginDialog *ui;
    QMap<TipErr, QString> _tip_errs;
    QMap<RequestId, std::function<void(const QJsonObject &)>> _handlers;

    bool checkEmailValid();

    bool checkPwdValid();

    void addTipErr(TipErr err, QString tips);

    void delTipErr(TipErr err);

    void showTip(QString str, bool b_ok);

    bool enableBtn(bool enabled);

    void initHttpHandlers();

private:
    void initHead();

public slots:

    void slot_forget_pwd();

    void on_login_btn_clicked();

    void slot_login_finish(RequestId id, QString res, ErrorCodes err);

    void slot_tcp_con_finish(bool);
signals:

    void switchRegister();

    void switchReset();

    void signal_connect_tcp(ServerInfo si);

    void signal_login_finish();
};


#endif //QTCHAT_LOGINDIALOG_H
