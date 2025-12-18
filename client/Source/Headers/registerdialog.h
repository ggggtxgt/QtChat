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

public slots:

    void slot_register_finish(RequestId id, QString str, ErrorCodes code);

private slots:

    void on_get_code_clicked();


    void on_sure_btn_clicked();

private:
    void showTip(QString string, bool ok);

    void initHttpHandlers();

    void addTipErr(TipErr te, QString tips);

    void delTipErr(TipErr te);

    bool checkUserValid();

    bool checkEmailValid();

    bool checkPassValid();

    bool checkConfirmValid();

    bool checkVarifyValid();

    void changeTipPage();

private:
    Ui::RegisterDialog *ui;
    QMap<RequestId, std::function<void(const QJsonObject &)>> _handlers;

    QMap<TipErr, QString> _tip_errs;
    QTimer *_countdown_timer;
    int _countdown;

signals:

    void sigSwitchLogin();

public slots:

    void on_return_btn_clicked();

    void on_cancel_btn_clicked();
};


#endif //QTCHAT_REGISTERDIALOG_H
