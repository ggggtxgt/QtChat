#ifndef QTCLIENT_RESETDIALOG_H
#define QTCLIENT_RESETDIALOG_H

#include <QDebug>
#include <QDialog>
#include <QRegularExpression>

#include "global.h"
#include "httpmanager.h"
#include "registerdialog.h"

#include "../Forms/ui_resetdialog.h"

class ResetDialog : public QDialog {
Q_OBJECT

public:
    explicit ResetDialog(QWidget *parent = nullptr);

    ~ResetDialog() override;

private:
    bool checkUserValid();

    bool checkPassValid();

    void showTip(QString str, bool b_ok);

    bool checkEmailValid();

    bool checkVarifyValid();

    void addTipErr(TipErr te, QString tips);

    void delTipErr(TipErr te);

    void initHandlers();

private:
    Ui::ResetDialog *ui;
    QMap<TipErr, QString> _tip_errs;
    QMap<RequestId, std::function<void(const QJsonObject &)>> _handlers;

private slots:

    void on_return_btn_clicked();

    void on_varify_btn_clicked();

    void slot_reset_finish(RequestId id, QString str, ErrorCodes code);

    void on_sure_btn_clicked();

    void on_cancel_btn_clicked();

signals:

    void switchLogin();
};

#endif //QTCLIENT_RESETDIALOG_H
