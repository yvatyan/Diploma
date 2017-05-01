#ifndef COVERWIDGET_H
#define COVERWIDGET_H

#include <QWidget>
#include <QFont>
#include <QMouseEvent>
#include "Core/Misc.h"

class CoverWidget : public QWidget {
    Q_OBJECT
    public:
        CoverWidget(QWidget* parent = Q_NULLPTR);

		void SetAnnoRectRegions(const std::vector<AnnotatedRectangle>& rects);
		const std::vector<AnnotatedRectangle>& GetAnnoRectRegions() const;
		void ResetRegions();
		void SetAnnoCrossPoints(const std::vector<AnnotatedPoint>& points);
		const std::vector<AnnotatedPoint>& GetAnnoCrossPoints() const;
		void ResetPoints();
    protected:
        void mousePressEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void paintEvent(QPaintEvent* event);
    private:
        static quint64 minSelectionArea;
		static QFont font;
		static size_t crossPointVerLen;
		static size_t crossPointHorLen;
        QRect selectedRect;
        QPoint startedPoint;
        bool selectionDisabled;
		bool textDisabled;
		bool rectRegionsDisabled;
		std::vector<AnnotatedRectangle> rectRegions;
		bool crossPointsDisabled;
		std::vector<AnnotatedPoint> objPoints;
	
		QPoint rectCaptionTextPoint(const Rectangle& rect, const std::string& text);
		QLine crossPointVerLine(const Point& point);
		QLine crossPointHorLine(const Point& point);
		Rectangle crossPointBoundingRect(const Point& point);
    signals:
        void selectionComplete();
};

#endif // COVERWIDGET_H

