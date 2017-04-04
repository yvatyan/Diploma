#include "FileLineEdit.h"
#include <QDebug>
#include <QApplication>
#include <QPalette>

const QMap<FileLineEdit::PathStatus, QColor>& FileLineEdit::DefaultStatusColors() {
    static QMap<PathStatus, QColor> defaultStatusColors;
    if(!defaultStatusColors.size()) {
        defaultStatusColors.insert(InvalidPath, QColor(220, 0, 0));
        defaultStatusColors.insert(ValidFile, QColor(220, 200, 0));
        defaultStatusColors.insert(ValidDir, QColor(220, 100, 0));
        defaultStatusColors.insert(ValidExtension, QColor(0, 220, 0));
    }
    return defaultStatusColors;
}
FileLineEdit::FileLineEdit(QString filter,
             ColorMode statusColorMode,
             QWidget* parent,
             QPushButton* button,
             QMap<PathStatus, QColor> statusColors,
             QString caption)
   :    dialogFilter(filter)
   ,    fileLineColorMode(statusColorMode)
   ,    QLineEdit(parent)
   ,    pathStatusColors(statusColors)
   ,    dialogCaption(caption)
   ,    dialogStartingDir("C:")
{
    if(button) {
        QObject::connect(button, SIGNAL(clicked(bool)), this, SLOT(OpenFileDialog(bool)));
    }
    QObject::connect(this, SIGNAL(returnPressed()), this, SLOT(InputIsComplete()));
    QObject::connect(this, SIGNAL(textChanged(QString)), this, SLOT(InputIsInProgress(QString)));
    extractFileExtensions();
}
void FileLineEdit::SetFile(QString path) {
    setText(path);
    InputIsComplete();
}
void FileLineEdit::OpenFileDialog(bool buttonClicked) {
    Q_UNUSED(buttonClicked);
    QString fileName = QFileDialog::getOpenFileName(
                            this,
                            dialogCaption,
                            dialogStartingDir,
                            dialogFilter
                       );
    if(fileName.size()) {
        setText(fileName);
        applyColorStatus(ValidExtension, false);
        emit fileSelected(fileName);
    }
}
void FileLineEdit::InputIsComplete() {
    PathStatus status = retrievePathStatus(text());
    applyColorStatus(status, false);
    if(status == ValidExtension) {
        emit fileSelected(text());
    }
}
void FileLineEdit::InputIsInProgress(QString path) {
    applyColorStatus(retrievePathStatus(path));
}
void FileLineEdit::extractFileExtensions() {
    QStringList differentTypes = dialogFilter.split(";;", QString::SkipEmptyParts);
    QRegExp pattern("\\((.*)\\)");
    for(size_t i = 0; i < differentTypes.size(); ++i) {
        pattern.indexIn(differentTypes[i]);
        if(pattern.cap(1) == "*") {
            validFileExtensions.clear();
            validFileExtensions.append("all");
            return;
        }
        QStringList dirtyExt = pattern.cap(1).split("*.", QString::SkipEmptyParts);
        for(size_t j = 0; j < dirtyExt.size(); ++j) {
            validFileExtensions.append(dirtyExt[j].trimmed());
        }
    }
}
FileLineEdit::PathStatus FileLineEdit::retrievePathStatus(QString path) {
    QFileInfo fileInfo(path);
    if(fileInfo.isDir()) {
        return ValidDir;
    } else if(fileInfo.isFile()) {
        if(validFileExtensions.first() == "all") {
            return ValidExtension;
        }
        QRegExp sep("\\\\|/");
        QString fileExtension = path
                                .split(sep, QString::SkipEmptyParts)
                                .last()
                                .split(".", QString::SkipEmptyParts)
                                .last();
        if(validFileExtensions.contains(fileExtension)){
            return ValidExtension;
        } else {
            return ValidFile;
        }
    } else {
        return InvalidPath;
    }
}
void FileLineEdit::applyColorStatus(FileLineEdit::PathStatus status, bool inputInProgress) {
    QPalette palette;
    palette.setColor(QPalette::Text, pathStatusColors[status]);
    if((fileLineColorMode == ColorfulWhileEditing && inputInProgress)
            || (fileLineColorMode == ColorlessWhileEditing && !inputInProgress)
            || (fileLineColorMode == ColorfulAlways)) {
        setPalette(palette);
    } else if(fileLineColorMode == ColorfulWhileEditing && !inputInProgress
            || (fileLineColorMode == ColorlessWhileEditing && inputInProgress)
            || (fileLineColorMode == ColorlessAlways)) {
        palette.setColor(QPalette::Text, qApp->palette().color(QPalette::Text));
    }
}
void FileLineEdit::setEnabled(bool enable) {
    int darkerBy = 64;
    if(enable && !isEnabled()) {
        applyColorStatus(retrievePathStatus(text()), false);
    } else if(!enable && isEnabled()) {
        QPalette pal(palette());
        QColor color = pal.color(QPalette::Text);
        pal.setColor(QPalette::Text, QColor(color.red() >= darkerBy ? color.red()-darkerBy : 0,
                                            color.green() >= darkerBy ? color.green()-darkerBy : 0,
                                            color.blue() >= darkerBy ? color.blue()-darkerBy : 0));
        setPalette(pal);
    }
    QLineEdit::setEnabled(enable);
}
void FileLineEdit::setDisabled(bool disable) {
    setEnabled(!disable);
}
