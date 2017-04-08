#include "GUI/VideoPlayer/Background.h"
#include <QPainter>

Background::Background(QWidget* parent) : QWidget(parent) {
}
void Background::SwitchBgrMode(BackgroundMode mode) {
    bgrMode = mode;
    update();
}
void Background::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    switch(bgrMode) {
        case NoSignal   : {
            int step = width() / 7;
            int eps = width() - step*7;

            painter.fillRect(QRect(0*step, 0, step, height()), QColor(192, 192, 192));
            painter.fillRect(QRect(1*step, 0, step, height()), QColor(192, 192, 0));
            painter.fillRect(QRect(2*step, 0, step, height()), QColor(0, 192, 192));
            painter.fillRect(QRect(3*step, 0, step, height()), QColor(0, 192, 0));
            painter.fillRect(QRect(4*step, 0, step, height()), QColor(192, 0, 192));
            painter.fillRect(QRect(5*step, 0, step, height()), QColor(192, 0, 0));
            painter.fillRect(QRect(6*step, 0, step+eps, height()), QColor(0, 0, 192));
            break;
        }
        case Black  : {
            painter.fillRect(QRect(0, 0, width(), height()), Qt::black);
            break;
        }
    }
}
