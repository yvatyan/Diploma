#include "GUI/ApplicationStyles.h"

void ApplicationStyles::init() {
    supportedStyles = QStyleFactory::keys();
    if(supportedStyles.contains("Fusion")) {
        supportedStyles.append("Dark Fusion");
    }
    qSort(supportedStyles.begin(), supportedStyles.end());
}
ApplicationStyles::ApplicationStyles() {
    init();
}
int ApplicationStyles::SetStyle(const QString& styleName) {
    if(styleName == "Dark Fusion") {
        qApp->setStyle(QStyleFactory::create("Fusion"));
        qApp->setPalette(getDarkFusionPallete());
        return StyleIndex("Dark Fusion");
    } else {
        qApp->setStyle(QStyleFactory::create(styleName));
        qApp->setPalette(qApp->style()->standardPalette());
        return StyleIndex(qApp->style()->objectName());
    }
}
int ApplicationStyles::SetStyle(const int styleIndex) {
    SetStyle(supportedStyles[styleIndex]);
    return styleIndex;
}
QStringList ApplicationStyles::StyleNames() const {
    return supportedStyles;
}
QStringList ApplicationStyles::ThemeNames() const {
    QStringList themes;
    for(int i = 0; i < supportedStyles.count(); ++i) {
        themes << supportedStyles[i]/* + " Theme"*/;
    }
    return themes;
}
int ApplicationStyles::StyleIndex(QString styleName) const {
    for(int i = 0; i < supportedStyles.count(); ++i) {
        if(styleName.toLower() == supportedStyles[i].toLower()) {
            return i;
        }
    }
    return -1;
}
QString ApplicationStyles::StyleName(int styleIndex) const {
    return supportedStyles[styleIndex];
}
QPalette ApplicationStyles::getDarkFusionPallete() const {
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(53,53,53));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Base, QColor(40,40,40));
    palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Button, QColor(53,53,53));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Highlight, QColor(220,53,0).lighter());
    palette.setColor(QPalette::HighlightedText, Qt::black);
    palette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);
    return palette;
}
