/******************************************************************************
 * @file       main.cpp
 * @brief      主函数
 * @author     ggggtxgt
 * @date       2025/12/8 19:04
 * @history
*****************************************************************************/
#include <QFile>
#include <QApplication>

#include "Headers/mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 读取 qss 文件
    QFile qss("../res/style/stylesheet.qss");
    if (qss.open(QFile::ReadOnly)) {
        qDebug("open success");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    } else {
        qDebug("Open failed");
    }

    // 读取 config.ini 文件
    QString fileName = "config.ini";
    QString app_path = QCoreApplication::applicationDirPath();
    QString config_path = QDir::toNativeSeparators(app_path + QDir::separator() + fileName);
    QSettings settings(config_path, QSettings::IniFormat);
    QString gate_host = settings.value("GateServer/host").toString();
    QString gate_port = settings.value("GateServer/port").toString();
    gate_url_prefix = "http://" + gate_host + ":" + gate_port;
    qDebug() << "config_path: " << config_path
             << "\ngate_host: " << gate_host
             << "\ngate_port: " << gate_port;

    MainWindow w;
    w.show();
    return a.exec();
}