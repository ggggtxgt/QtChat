/******************************************************************************
 * @file       registerdialog.h
 * @brief      注册窗口头文件
 * @author     ggggtxgt
 * @date       2025/12/8 20:14
 * @history
*****************************************************************************/
#ifndef QTCHAT_REGISTERDIALOG_H
#define QTCHAT_REGISTERDIALOG_H

#include <QDialog>

#include "global.h"


QT_BEGIN_NAMESPACE
namespace Ui { class RegisterDialog; }
QT_END_NAMESPACE

class RegisterDialog : public QDialog {
Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);

    ~RegisterDialog() override;

private slots:

    void on_get_code_clicked();

    void slot_register_finish(RequestId id, QString str, ErrorCodes code);

private:
    void showTip(QString string, bool ok);

    void initHttpHandlers();

private:
    Ui::RegisterDialog *ui;
    QMap<RequestId, std::function<void(const QJsonObject &)>> _handlers;
};


#endif //QTCHAT_REGISTERDIALOG_H
