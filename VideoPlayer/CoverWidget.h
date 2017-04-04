#ifndef COVERWIDGET_H
#define COVERWIDGET_H

#include <QWidget>
#include <QMouseEvent>

class CoverWidget : public QWidget {
    Q_OBJECT
    public:
        CoverWidget(QWidget* parent = Q_NULLPTR);
    protected:
        void mousePressEvent(QMouseEvent * event);
        void mouseReleaseEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent * event);
        void paintEvent(QPaintEvent *event);
    private:
        static quint64 minSelectionArea;

        QRect selectedRect;
        QPoint startedPoint;
        bool selectionDisabled;
    signals:
        void selectionComplete();
};

#endif // COVERWIDGET_H
