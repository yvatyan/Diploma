#include "ApplicationStyles.h"

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
ApplicationStyles::ApplicationStyles(const QString& styleName) {
    init();
    SetStyle(styleName);
}
void ApplicationStyles::SetStyle(const QString& styleName) {
    if(styleName == "Dark Fusion") {
        qApp->setStyle(QStyleFactory::create("Fusion"));
        qApp->setPalette(getDarkFusionPallete());
    } else {
        qApp->setStyle(QStyleFactory::create(styleName));
        qApp->setPalette(qApp->style()->standardPalette());
    }
}
void ApplicationStyles::SetStyle(const int styleIndex) {
    SetStyle(supportedStyles[styleIndex]);
}
QStringList ApplicationStyles::StyleNames() const {
    return supportedStyles;
}
QStringList ApplicationStyles::ThemeNames() const {
    QStringList themes;
    for(int i = 0; i < supportedStyles.count(); ++i) {
        themes << supportedStyles[i] + " Theme";
    }
    return themes;
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
