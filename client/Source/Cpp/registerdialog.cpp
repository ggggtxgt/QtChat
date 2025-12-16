/******************************************************************************
 * @file       registerdialog.cpp
 * @brief      注册窗口源文件
 * @author     ggggtxgt
 * @date       2025/12/8 20:15
 * @history
*****************************************************************************/
#include "../Headers/global.h"
#include "../Headers/httpmanager.h"
#include "../Headers/registerdialog.h"
#include "../Forms/ui_RegisterDialog.h"

RegisterDialog::RegisterDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::RegisterDialog), _countdown(5) {
    ui->setupUi(this);

    // 将注册窗口中的 lineEdit 输入输入模式设置为密码模式
    ui->password_edit->setEchoMode(QLineEdit::Password);
    ui->confirm_edit->setEchoMode(QLineEdit::Password);

    // 提前为注册窗口的 error_tip 标签设置属性
    ui->error_tip->setProperty("state", "normal");
    // 设置属性之后立刻刷新
    repolish(ui->error_tip);

    connect(HttpManager::GetInstance().get(), &HttpManager::signal_register_finish,
            this, &RegisterDialog::slot_register_finish);

    initHttpHandlers();

    // 清空初始时的错误提示
    ui->error_tip->clear();

    connect(ui->user_edit, &QLineEdit::editingFinished, this, [this]() {
        checkUserValid();
    });

    connect(ui->email_edit, &QLineEdit::editingFinished, this, [this]() {
        checkEmailValid();
    });

    connect(ui->password_edit, &QLineEdit::editingFinished, this, [this]() {
        checkPassValid();
    });

    connect(ui->confirm_edit, &QLineEdit::editingFinished, this, [this]() {
        checkConfirmValid();
    });

    connect(ui->varify_edit, &QLineEdit::editingFinished, this, [this]() {
        checkVarifyValid();
    });

    // 鼠标移到 [眼睛]位置，将会变成 [小手]
    ui->password_visible->setCursor(Qt::PointingHandCursor);
    ui->confirm_visible->setCursor(Qt::PointingHandCursor);

    // 设置多种状态样式
    ui->password_visible->SetState("unvisible", "unvisible_hover", "", "visible",
                                   "visible_hover", "");

    ui->confirm_visible->SetState("unvisible", "unvisible_hover", "", "visible",
                                  "visible_hover", "");

    // 连接点击事件
    connect(ui->password_visible, &ClickedLabel::clicked, this, [this]() {
        auto state = ui->password_visible->GetCurState();
        if (state == ClickLbState::Normal) {
            ui->password_edit->setEchoMode(QLineEdit::Password);
        } else {
            ui->password_edit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "Label was clicked!";
    });

    connect(ui->confirm_visible, &ClickedLabel::clicked, this, [this]() {
        auto state = ui->confirm_visible->GetCurState();
        if (state == ClickLbState::Normal) {
            ui->confirm_edit->setEchoMode(QLineEdit::Password);
        } else {
            ui->confirm_edit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "Label was clicked!";
    });

    // 创建定时器
    _countdown_timer = new QTimer(this);
    // 连接定时器的信号和槽
    connect(_countdown_timer, &QTimer::timeout, [this]() {
        if (0 == _countdown) {
            _countdown_timer->stop();
            emit sigSwitchLogin();
            return;
        }
        _countdown--;
        auto str = QString("注册成功，%1 秒之后返回登录").arg(_countdown);
        ui->tip01_lb->setText(str);
    });
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
        QJsonObject json;
        json["email"] = email;
        HttpManager::GetInstance()->PostHttpRequest(QUrl(gate_url_prefix + "/get_varify_code"), json,
                                                    RequestId::ID_GET_VARIFY_CODE, Modules::REGISTERMOD);
    } else {
        showTip(tr("邮箱地址错误"), false);
    }
}

void RegisterDialog::slot_register_finish(RequestId id, QString str, ErrorCodes code) {
    if (ErrorCodes::SUCCESS != code) {
        showTip(tr("网络请求错误"), false);
        return;
    }

    // 解析 json 字符串，将 str 转换为 QByteArray
    QJsonDocument json = QJsonDocument::fromJson(str.toUtf8());
    if (json.isNull()) {
        showTip(tr("json解析失败"), false);
        return;
    }
    if (!json.isObject()) {
        showTip(tr("json解析失败"), false);
        return;
    }
    _handlers[id](json.object());
    return;
}

void RegisterDialog::on_sure_btn_clicked() {
    // 所有条件都正确，才能发送请求：
    bool valid = checkUserValid();
    if (!valid) {
        return;
    }

    valid = checkEmailValid();
    if (!valid) {
        return;
    }

    valid = checkPassValid();
    if (!valid) {
        return;
    }

    valid = checkVarifyValid();
    if (!valid) {
        return;
    }

    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = md5Encrypt(ui->password_edit->text());
    json_obj["confirm"] = md5Encrypt(ui->confirm_edit->text());
    json_obj["varifycode"] = ui->varify_edit->text();
    HttpManager::GetInstance()->PostHttpRequest(QUrl(gate_url_prefix + "/user_register"),
                                                json_obj, RequestId::ID_REGISTER_USER, Modules::REGISTERMOD);
}


void RegisterDialog::initHttpHandlers() {
    // 获取验证码回包
    _handlers.insert(RequestId::ID_GET_VARIFY_CODE, [this](const QJsonObject &json) {
        int error = json["error"].toInt();
        if (ErrorCodes::SUCCESS != error) {
            showTip(tr("参数错误"), false);
            return;
        }

        auto email = json["email"].toString();
        showTip(tr("验证码已发送至邮箱，注意查收"), true);
        qDebug() << "email is " << email;
    });

    // 注册用户回包逻辑
    _handlers.insert(RequestId::ID_REGISTER_USER, [this](QJsonObject jsonObj) {
        int error = jsonObj["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            showTip(tr("参数错误"), false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("用户注册成功"), true);
        qDebug() << "user uid is: " << jsonObj["uid"].toInt();
        qDebug() << "email is " << email;
        // 注册成功之后，切换回到登录界面
        changeTipPage();
    });
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

void RegisterDialog::addTipErr(TipErr te, QString tips) {
    _tip_errs[te] = tips;
    showTip(tips, false);
}

void RegisterDialog::delTipErr(TipErr te) {
    _tip_errs.remove(te);
    if (_tip_errs.empty()) {
        ui->error_tip->clear();
        return;
    }
    showTip(_tip_errs.first(), false);
}

bool RegisterDialog::checkUserValid() {
    if (ui->user_edit->text() == "") {
        addTipErr(TipErr::TIP_USER_ERR, tr("用户名称不能为空"));
        return false;
    }
    delTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool RegisterDialog::checkEmailValid() {
    auto email = ui->email_edit->text();
    // 通过正则表达式判断邮箱的样式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if (!match) {
        addTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱地址错误"));
        return false;
    }
    delTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool RegisterDialog::checkPassValid() {
    auto pwd = ui->password_edit->text();
    // 限制密码长度
    if (pwd.length() < 6 || pwd.length() > 15) {
        addTipErr(TipErr::TIP_PWD_ERR, tr("密码长度就为6~15"));
        return false;
    }
    // 使用正则表达式判断是否存在非法字符
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pwd).hasMatch();
    // 存在非法字符
    if (!match) {
        addTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;
    }
    delTipErr(TipErr::TIP_PWD_ERR);
    return true;
}

bool RegisterDialog::checkConfirmValid() {
    // 判断确定密码：只需判断与原密码是否相同即可
    auto pwd = ui->password_edit->text();
    auto confirm = ui->confirm_edit->text();
    if (pwd != confirm) {
        addTipErr(TipErr::TIP_PWD_CONFIRM, tr("再次密码不一致"));
        return false;
    }
    delTipErr(TipErr::TIP_PWD_CONFIRM);
    return true;
}

bool RegisterDialog::checkVarifyValid() {
    auto code = ui->varify_edit->text();
    if (code.isEmpty()) {
        addTipErr(TipErr::TIP_VARIFY_ERR, tr("验证码错误"));
        return false;
    }
    delTipErr(TipErr::TIP_VARIFY_ERR);
    return true;
}

void RegisterDialog::changeTipPage() {
    _countdown_timer->stop();
    ui->stackedWidget->setCurrentWidget(ui->page_2);

    // 启动定时器，设置间隔为1秒
    _countdown_timer->start(1000);
}

void RegisterDialog::on_return_btn_clicked() {
    _countdown_timer->stop();
    emit sigSwitchLogin();
}

void RegisterDialog::on_cancel_btn_clicked() {
    _countdown_timer->stop();
    emit sigSwitchLogin();
}