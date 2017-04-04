#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QWidget>

class Background : public QWidget {
    Q_OBJECT
    public:
        enum BackgroundMode {
            NoSignal,
            Black
        };
        Background(QWidget* parent = Q_NULLPTR);
    public slots:
        void SwitchBgrMode(BackgroundMode mode);
    protected:
        void paintEvent(QPaintEvent *event);
    private:
        BackgroundMode bgrMode;
};

#endif // BACKGROUND_H
