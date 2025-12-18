#include "../Headers/resetdialog.h"

ResetDialog::ResetDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::ResetDialog) {
    ui->setupUi(this);

    connect(ui->user_edit, &QLineEdit::editingFinished, this, [this]() {
        checkUserValid();
    });

    connect(ui->email_edit, &QLineEdit::editingFinished, this, [this]() {
        checkEmailValid();
    });

    connect(ui->pwd_edit, &QLineEdit::editingFinished, this, [this]() {
        checkPassValid();
    });

    connect(ui->varify_edit, &QLineEdit::editingFinished, this, [this]() {
        checkVarifyValid();
    });

    // 连接reset相关信号和注册处理回调
    initHandlers();
    connect(HttpManager::GetInstance().get(), &HttpManager::signal_reset_finish, this,
            &ResetDialog::slot_reset_finish);

}

ResetDialog::~ResetDialog() {
    delete ui;
}

bool ResetDialog::checkUserValid() {
    if ("" == ui->user_edit->text()) {
        addTipErr(TipErr::TIP_USER_ERR, tr("用户名称不能为空"));
        return false;
    }
    delTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool ResetDialog::checkPassValid() {
    auto pwd = ui->pwd_edit->text();
    if (pwd.length() < 6 || pwd.length() > 15) {
        // 提示密码长度错误
        addTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }
    // 通过正则表达式判断密码是否包含非法字符
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pwd).hasMatch();
    if (!match) {
        // 提示密码存在非法字符
        addTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;
    }
    delTipErr(TipErr::TIP_PWD_ERR);
    return true;
}

bool ResetDialog::checkEmailValid() {
    auto email = ui->email_edit->text();
    // 通过正则表达式判断邮箱是否正确
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if (!match) {
        addTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱地址错误"));
        return false;
    }
    delTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool ResetDialog::checkVarifyValid() {
    auto pwd = ui->varify_edit->text();
    if (pwd.isEmpty()) {
        addTipErr(TipErr::TIP_VARIFY_ERR, tr("验证码错误"));
        return false;
    }
    delTipErr(TipErr::TIP_VARIFY_ERR);
    return true;
}

void ResetDialog::addTipErr(TipErr te, QString tips) {
    _tip_errs[te] = tips;
    showTip(tips, false);
}

void ResetDialog::delTipErr(TipErr te) {
    _tip_errs.remove(te);
    if (_tip_errs.empty()) {
        ui->error_tip->clear();
        return;
    }
    showTip(_tip_errs.first(), false);
}

void ResetDialog::showTip(QString str, bool b_ok) {
    if (b_ok) {
        ui->error_tip->setProperty("state", "normal");
    } else {
        ui->error_tip->setProperty("state", "err");
    }
    ui->error_tip->setText(str);
    repolish(ui->error_tip);
}

void ResetDialog::on_varify_btn_clicked() {
    qDebug() << "receive varify bin clicked";
    auto email = ui->email_edit->text();
    auto bcheck = checkEmailValid();
    if (!bcheck) {
        return;
    }
    // 发送 http 请求获取验证码
    QJsonObject json;
    json["email"] = email;
    HttpManager::GetInstance()->PostHttpRequest(QUrl(gate_url_prefix + "/get_varify_code"),
                                                json, RequestId::ID_GET_VARIFY_CODE,
                                                Modules::RESETMOD);
}

void ResetDialog::initHandlers() {
    // 注册获取验证码回包
    _handlers.insert(RequestId::ID_GET_VARIFY_CODE, [this](QJsonObject json) {
        int error = json["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            showTip(tr("参数错误"), false);
            return;
        }
        auto email = json["email"].toString();
        showTip(tr("验证码已经发送，注意查收"), true);
        qDebug() << "email is: " << email;
    });

    // 注册注册用户回包
    _handlers.insert(RequestId::ID_RESET_PWD, [this](QJsonObject json) {
        int error = json["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            showTip(tr("参数错误"), false);
            return;
        }
        auto email = json["email"].toString();
        showTip(tr("重置成功，点击返回登录"), true);
        qDebug() << "email is: " << email;
        qDebug() << "user uid is: " << json["uid"].toInt();
    });
}

void ResetDialog::slot_reset_finish(RequestId id, QString str, ErrorCodes code) {
    if (code != ErrorCodes::SUCCESS) {
        showTip(tr("网络请求错误"), false);
        return;
    }
    // 解析 JSON 字符串，转换为 QByteArray
    QJsonDocument json = QJsonDocument::fromJson(str.toUtf8());
    if (json.isNull()) {
        showTip(tr("json解析错误"), false);
        return;
    }
    if (!json.isObject()) {
        showTip(tr("json解析错误"), false);
        return;
    }

    // 调用对应逻辑，根据id回调
    _handlers[id](json.object());
    return;
}

void ResetDialog::on_sure_btn_clicked() {
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

    // 发送http重置用户请求
    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = md5Encrypt(ui->pwd_edit->text());
    json_obj["varifycode"] = ui->varify_edit->text();
    HttpManager::GetInstance()->PostHttpRequest(QUrl(gate_url_prefix + "/reset_pwd"),
                                        json_obj, RequestId::ID_RESET_PWD, Modules::RESETMOD);
}

void ResetDialog::on_return_btn_clicked() {
    qDebug() << "sure btn clicked: ";
    emit switchLogin();
}

void ResetDialog::on_cancel_btn_clicked() {

}