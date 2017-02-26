#ifndef APPLICATIONSTYLES_H
#define APPLICATIONSTYLES_H

#include <QStyleFactory>
#include <QApplication>
#include <QString>
#include <QPalette>
#include <QStyle>

class ApplicationStyles : public QObject {
    Q_OBJECT
    public:
        ApplicationStyles();
        ApplicationStyles(const QString& styleName);

        QStringList StyleNames() const;
        QStringList ThemeNames() const;
    public slots:
        void SetStyle(const QString& styleName);
        void SetStyle(const int styleIndex);
    private:
        QStringList supportedStyles;

        void init();
        QPalette getDarkFusionPallete() const;
};

#endif // APPLICATIONSTYLES_H
