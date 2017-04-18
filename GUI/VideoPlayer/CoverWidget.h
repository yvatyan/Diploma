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
    protected:
        void mousePressEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void paintEvent(QPaintEvent* event);
    private:
        static quint64 minSelectionArea;
		static QFont font;
        QRect selectedRect;
        QPoint startedPoint;
        bool selectionDisabled;
		bool textDisabled;
		bool rectRegionsDisabled;
		std::vector<AnnotatedRectangle> rectRegions;
	
		QPoint rectCaptionTextPoint(AnnotatedRectangle annoRect);
    signals:
        void selectionComplete();
};

#endif // COVERWIDGET_H

