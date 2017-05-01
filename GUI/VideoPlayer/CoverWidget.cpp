#include "GUI/VideoPlayer/CoverWidget.h"
#include <QFontMetrics>
#include <QPainter>
#include <QPen>


quint64 CoverWidget::minSelectionArea = 64;
QFont CoverWidget::font = QFont("Times", 8);
size_t CoverWidget::crossPointVerLen = 10;
size_t CoverWidget::crossPointHorLen = 10;
CoverWidget::CoverWidget(QWidget* parent) : QWidget(parent) {
    selectionDisabled = false;
	textDisabled = true;
	rectRegionsDisabled = false;
	crossPointsDisabled =  false;
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
				QPoint point = rectCaptionTextPoint(rectRegions[i].GetRectangle(), rectRegions[i].GetStdString());
				painter.drawText(point, QString(rectRegions[i].GetStdString().c_str()));
			}
		}
	}
	if(!crossPointsDisabled) {
		for(size_t i = 0; i < objPoints.size(); ++i) {
			if(i == 100) break;
			QPen pen(objPoints[i].GetRgbaColor().QtColor());
			pen.setWidth(1);
			painter.setPen(pen);
			painter.drawLine(crossPointVerLine(objPoints[i].GetPoint()));
			painter.drawLine(crossPointHorLine(objPoints[i].GetPoint()));
			if(!textDisabled) {
				QPoint point = rectCaptionTextPoint(crossPointBoundingRect(objPoints[i].GetPoint()), objPoints[i].GetStdString());
				painter.drawText(point, QString(objPoints[i].GetStdString().c_str()));
			}
		}
	}
}
void CoverWidget::SetAnnoRectRegions(const std::vector<AnnotatedRectangle>& rects) {
	rectRegions = rects;
	update();
}
const std::vector<AnnotatedRectangle>& CoverWidget::GetAnnoRectRegions() const {
	return rectRegions;
}
void CoverWidget::ResetRegions() {
	rectRegions.clear();
	update();
}
void CoverWidget::SetAnnoCrossPoints(const std::vector<AnnotatedPoint>& points) {
	objPoints = points;
	update();
}
const std::vector<AnnotatedPoint>& CoverWidget::GetAnnoCrossPoints() const {
	return objPoints;
}
void CoverWidget::ResetPoints() {
	objPoints.clear();
}
QPoint CoverWidget::rectCaptionTextPoint(const Rectangle& rect, const std::string& text) {
	const int marginV = 2;
	const int marginH = 2;
	int textWidth = QFontMetrics(font).width(QString(text.c_str()));
	int textHeight = QFontMetrics(font).height();
	size_t rectWidth = rect.width;
	size_t rectHeight = rect.height;
	int x = rect.lt_point_x;
	int y = rect.lt_point_y;

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
QLine CoverWidget::crossPointVerLine(const Point& point) {
	return QLine(point.x, point.y - crossPointVerLen/2, point.x, point.y + crossPointVerLen/2);
}
QLine CoverWidget::crossPointHorLine(const Point& point) {
	return QLine(point.x - crossPointHorLen/2, point.y, point.x + crossPointHorLen/2, point.y);
}
Rectangle CoverWidget::crossPointBoundingRect(const Point& point) {
	return Rectangle(point.x - crossPointHorLen/2, point.y - crossPointVerLen/2, crossPointVerLen, crossPointHorLen);
}

