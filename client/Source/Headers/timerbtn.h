#ifndef QTCLIENT_TIMERBTN_H
#define QTCLIENT_TIMERBTN_H

#include <QTimer>
#include <QPushButton>

#include "global.h"

class TimerBtn : public QPushButton {
Q_OBJECT
public:
    TimerBtn(QWidget *parent = nullptr);

    ~TimerBtn();

    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QTimer *_timer;
    int _counter;
};

#endif //QTCLIENT_TIMERBTN_H
