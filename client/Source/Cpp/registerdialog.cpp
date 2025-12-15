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
        QDialog(parent), ui(new Ui::RegisterDialog) {
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
        // HttpManager::GetInstance()->PostHttpRequest(QUrl("http://localhost:8080/get_varify_code"), json,
        //                                             RequestId::ID_GET_VARIFY_CODE, Modules::REGISTERMOD);
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
    if (ui->user_edit->text() == "") {
        showTip(tr("用户名不能为空"), false);
        return;
    }

    if (ui->email_edit->text() == "") {
        showTip(tr("邮箱不能为空"), false);
        return;
    }

    if (ui->password_edit->text() == "") {
        showTip(tr("密码不能为空"), false);
        return;
    }

    if (ui->confirm_edit->text() == "") {
        showTip(tr("确认密码不能为空"), false);
        return;
    }

    if (ui->confirm_edit->text() != ui->password_edit->text()) {
        showTip(tr("密码和确认密码不匹配"), false);
        return;
    }

    if (ui->varify_edit->text() == "") {
        showTip(tr("验证码不能为空"), false);
        return;
    }

    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = ui->password_edit->text();
    json_obj["confirm"] = ui->confirm_edit->text();
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
        qDebug() << "user uid is: " << jsonObj["uid"].toString();
        qDebug() << "email is " << email;
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