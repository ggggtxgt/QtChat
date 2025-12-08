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
    _login_dialog = new LoginDialog(this);

    // 将登录窗口设置为主窗口的核心窗口
    setCentralWidget(_login_dialog);
    // _login_dialog->show();

    // 创建[注册按钮]的连接
    connect(_login_dialog, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchRegister);
    _register_dialog = new RegisterDialog(this);

    // 将登录、注册窗口嵌入主窗口
    _login_dialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    _register_dialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    _register_dialog->hide();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::SlotSwitchRegister() {
    setCentralWidget(_register_dialog);
    _login_dialog->hide();
    _register_dialog->show();
}