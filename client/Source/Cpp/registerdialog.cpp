/******************************************************************************
 * @file       registerdialog.cpp
 * @brief      注册窗口源文件
 * @author     ggggtxgt
 * @date       2025/12/8 20:15
 * @history
*****************************************************************************/
#include "../Headers/global.h"
#include "../Headers/registerdialog.h"
#include "../Forms/ui_RegisterDialog.h"

RegisterDialog::RegisterDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::RegisterDialog) {
    ui->setupUi(this);

    // 将注册窗口中的 lineEdit 输入输入模式设置为密码模式
    ui->password_edit->setEchoMode(QLineEdit::Password);
    ui->confirm_edit->setEchoMode(QLineEdit::Password);

    // 提前为注册窗口的 error_tip 标签设置属性
    ui->error_tip->setProperty("state", "normal");
    // 设置属性之后立刻刷新
    repolish(ui->error_tip);
}

RegisterDialog::~RegisterDialog() {
    delete ui;
}

void RegisterDialog::on_get_code_clicked() {
    auto email = ui->email_edit->text();
    // 定义正则表达式规则
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if (match) {
        // 发送Http验证码
    } else {
        showTip(tr("邮箱地址错误"), false);
    }
}

void RegisterDialog::showTip(QString string, bool ok) {
    if (ok) {
        ui->error_tip->setProperty("state", "normal");
    } else {
        ui->error_tip->setProperty("state", "error");
    }
    ui->error_tip->setText(string);
    repolish(ui->error_tip);
}