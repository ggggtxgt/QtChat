#include "../Headers/timerbtn.h"

#include <QDebug>
#include <QMouseEvent>

TimerBtn::TimerBtn(QWidget *parent) : QPushButton(parent), _counter(10) {
    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, [this]() {
        _counter--;
        if (_counter <= 0) {
            _timer->stop();
            _counter = 10;
            this->setText("获取");
            this->setEnabled(true);
            return;
        }
        this->setText(QString::number(_counter));
    });
}

TimerBtn::~TimerBtn() {
    _timer->stop();
}

// @todo 即使未输入任何内容，也可点击[获取]按钮
void TimerBtn::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // 处理鼠标左键释放事件
        qDebug() << "MyButton was released!";
        this->setEnabled(false);
        this->setText(QString::number(_counter));
        _timer->start(1000);
        emit click();
    }
    // 调用基类 mouseReleaseEvent 确保正常的事件处理
    QPushButton::mouseReleaseEvent(event);
}