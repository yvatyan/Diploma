#include "GUI/VideoPlayer/CoverWidget.h"
#include <QFontMetrics>
#include <QPainter>
#include <QPen>

quint64 CoverWidget::minSelectionArea = 64;
QFont CoverWidget::font = QFont("Times", 8);
CoverWidget::CoverWidget(QWidget* parent) : QWidget(parent) {
    selectionDisabled = false;
	textDisabled = false;
	rectRegionsDisabled = false;
    setMouseTracking(false);
}
void CoverWidget::mousePressEvent(QMouseEvent* event) {
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
	painter.setFont(font);
    if(!selectionDisabled) {
        QPen pen(Qt::red);
        pen.setWidth(3);
        painter.setPen(pen);
        painter.drawRect(selectedRect);
    }
	if(!rectRegionsDisabled) {
		for(size_t i = 0; i < rectRegions.size(); ++i) {
			QPen pen(rectRegions[i].GetRgbaColor().QtColor());
			pen.setWidth(1);
			painter.setPen(pen);
			painter.drawRect(rectRegions[i].GetRectangle().QtRect());
			if(!textDisabled) {
				QPoint point = rectCaptionTextPoint(rectRegions[i]);
				painter.drawText(point, QString(rectRegions[i].GetStdString().c_str()));
			}
		}
	}
}
const std::vector<AnnotatedRectangle>& CoverWidget::GetAnnoRectRegions() const {
	return rectRegions;
}
void CoverWidget::SetAnnoRectRegions(const std::vector<AnnotatedRectangle>& rects) {
	rectRegions = rects;
	update();
}
void CoverWidget::ResetRegions() {
	rectRegions.clear();
	update();
}
QPoint CoverWidget::rectCaptionTextPoint(AnnotatedRectangle annoRect) {
	const int marginV = 2;
	const int marginH = 2;
	int textWidth = QFontMetrics(font).width(QString(annoRect.GetStdString().c_str()));
	int textHeight = QFontMetrics(font).height();
	size_t rectWidth = annoRect.GetRectangle().width;
	size_t rectHeight = annoRect.GetRectangle().height;
	int x = annoRect.GetRectangle().lt_point_x;
	int y = annoRect.GetRectangle().lt_point_y;

	if(x + rectWidth + marginH + textWidth < width() 
		&&
	   y - marginV - textHeight >= 0) {	// top-right
		return QPoint(x + rectWidth + marginH, y - marginV);
	}
	if(x + rectWidth + marginH + textWidth < width()
		&&
	   y + marginV + rectHeight <= height()) { // bottom-right
		return QPoint(x + rectWidth + marginH, y + marginV + textHeight + rectHeight);
	}
	if(x - marginH - textWidth >= 0
		&&
	   y + marginV + rectHeight <= height()) { // bottom-left
		return QPoint(x - marginH - textWidth, y + marginV + textHeight + rectHeight);
	}
	if(x - marginH - textWidth >= 0
		&&
	   y - marginV - textHeight >= 0) { // top-left
		return QPoint(x - marginH - textWidth, y - marginV);
	}
}
