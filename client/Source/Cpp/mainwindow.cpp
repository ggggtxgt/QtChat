/******************************************************************************
 * @file       mainwindow.cpp
 * @brief      主窗口源文件
 * @author     ggggtxgt
 * @date       2025/12/8 19:05
 * @history
*****************************************************************************/
#include "../Headers/mainwindow.h"
#include "../Forms/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    _login_dialog = new LoginDialog();

    // 将登录窗口设置为主窗口的核心窗口
    setCentralWidget(_login_dialog);
    _login_dialog->show();

    // 创建[注册按钮]的连接
    connect(_login_dialog, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchRegister);
    _register_dialog = new RegisterDialog();
}

MainWindow::~MainWindow() {
    delete ui;

    // 未指定父对象，自行回收申请的内存空间
    if (_login_dialog) {
        delete _login_dialog;
        _login_dialog = nullptr;
    }
    if (_register_dialog) {
        delete _register_dialog;
        _register_dialog = nullptr;
    }
}

void MainWindow::SlotSwitchRegister() {
    setCentralWidget(_register_dialog);
    _login_dialog->hide();
    _register_dialog->show();
}