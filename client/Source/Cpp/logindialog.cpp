/******************************************************************************
 * @file       logindialog.cpp
 * @brief      登录窗口源文件
 * @author     ggggtxgt
 * @date       2025/12/8 19:43
 * @history
*****************************************************************************/
#include "../Headers/logindialog.h"
#include "../Forms/ui_LoginDialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::LoginDialog) {
    ui->setupUi(this);
    connect(ui->register_btn, &QPushButton::clicked, this, &LoginDialog::switchRegister);
}

LoginDialog::~LoginDialog() {
    delete ui;
}
