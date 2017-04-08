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

        QStringList StyleNames() const;
        QStringList ThemeNames() const;
        int StyleIndex(QString styleName) const;
        QString StyleName(int styleIndex) const;
    public slots:
        int SetStyle(const QString& styleName);
        int SetStyle(const int styleIndex);
    private:
        QStringList supportedStyles;

        void init();
        QPalette getDarkFusionPallete() const;
};

#endif // APPLICATIONSTYLES_H
