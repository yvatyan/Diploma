#include "GUI/VideoPlayer/CoverWidget.h"
#include <QPainter>
#include <QPen>

quint64 CoverWidget::minSelectionArea = 64;
CoverWidget::CoverWidget(QWidget* parent) : QWidget(parent) {
    selectionDisabled = false;
    setMouseTracking(false);
}
void CoverWidget::mousePressEvent(QMouseEvent * event) {
    selectionDisabled = false;
    selectedRect = QRect(startedPoint, QSize(0, 0));
    startedPoint = event->pos();
}
void CoverWidget::mouseReleaseEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    quint64 rectArea = selectedRect.width() * selectedRect.height();
    if(rectArea < minSelectionArea) {
        selectionDisabled = true;
    } else {
        emit selectionComplete();
    }
    update();
}
void CoverWidget::mouseMoveEvent(QMouseEvent* event) {
    selectedRect = QRect(startedPoint, event->pos());
    update();
}
void CoverWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    if(!selectionDisabled) {
        QPen pen(Qt::red);
        pen.setWidth(3);
        painter.setPen(pen);
        painter.drawRect(selectedRect);
    }
}
