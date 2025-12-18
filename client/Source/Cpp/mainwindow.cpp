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
    _login_dialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_login_dialog);

    // 创建[注册按钮]的连接
    connect(_login_dialog, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchRegister);
    // 连接登录界面 [忘记密码] 信号
    connect(_login_dialog, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::SlotSwitchRegister() {
    _register_dialog = new RegisterDialog(this);
    _register_dialog->hide();

    // 设置注册窗口的属性标记
    _register_dialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    // 连接注册界面返回登录信号
    connect(_register_dialog, &RegisterDialog::sigSwitchLogin, this, &MainWindow::SlotSwitchLogin);

    setCentralWidget(_register_dialog);
    _login_dialog->hide();
    _register_dialog->show();
}

void MainWindow::SlotSwitchLogin() {
    // 创建 CentralWidget，并将其设置为 MainWindow 的中心部件
    _login_dialog = new LoginDialog(this);
    _register_dialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_login_dialog);

    _register_dialog->hide();
    _login_dialog->show();
    // 连接登录界面注册信号
    connect(_login_dialog, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchRegister);
    // 连接登录界面忘记密码信号
    connect(_login_dialog, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
}

void MainWindow::SlotSwitchReset() {
    // 创建 CentralWidget，并将其设置为 MainWindow 的中心部件
    _reset_dialog = new ResetDialog(this);
    _reset_dialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_reset_dialog);

    _login_dialog->hide();
    _reset_dialog->show();
    // 连接登录界面注册信号
    connect(_reset_dialog, &ResetDialog::switchLogin, this, &MainWindow::SlotSwitchLogin2);
}

void MainWindow::SlotSwitchLogin2() {
    // 创建 CentralWidget，并将其设置为 MainWindow 的中心部件
    _login_dialog = new LoginDialog(this);
    _login_dialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_login_dialog);

    _reset_dialog->hide();
    _login_dialog->show();
    // 连接登录界面 [忘记密码] 信号
    connect(_login_dialog, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);

    // 连接登录界面注册信号
    connect(_login_dialog, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchRegister);
}