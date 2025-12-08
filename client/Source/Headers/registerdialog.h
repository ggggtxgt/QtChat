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

private:
    void showTip(QString string, bool ok);

private:
    Ui::RegisterDialog *ui;
};


#endif //QTCHAT_REGISTERDIALOG_H
