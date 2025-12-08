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


QT_BEGIN_NAMESPACE
namespace Ui { class LoginDialog; }
QT_END_NAMESPACE

class LoginDialog : public QDialog {
Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);

    ~LoginDialog() override;

private:
    Ui::LoginDialog *ui;

signals:

    void switchRegister();
};


#endif //QTCHAT_LOGINDIALOG_H
