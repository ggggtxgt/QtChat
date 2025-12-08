/******************************************************************************
 * @file       registerdialog.cpp
 * @brief      注册窗口源文件
 * @author     ggggtxgt
 * @date       2025/12/8 20:15
 * @history
*****************************************************************************/
#include "../Headers/registerdialog.h"
#include "../Forms/ui_RegisterDialog.h"

RegisterDialog::RegisterDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::RegisterDialog) {
    ui->setupUi(this);
}

RegisterDialog::~RegisterDialog() {
    delete ui;
}
