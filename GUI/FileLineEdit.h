#ifndef FILELINEEDIT_H
#define FILELINEEDIT_H

#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QColor>

class FileLineEdit : public QLineEdit {
    Q_OBJECT
    public:
        enum PathStatus {
            InvalidPath,
            ValidFile,
            ValidDir,
            ValidExtension
        };
        enum ColorMode {
            ColorfulWhileEditing,
            ColorfulAlways,
            ColorlessAlways,
            ColorlessWhileEditing
        };
        static const QMap<PathStatus, QColor>& DefaultStatusColors();
        FileLineEdit(QString filter,
                     ColorMode statusColorMode = ColorfulAlways,
                     QWidget* parent = Q_NULLPTR,
                     QPushButton* button = Q_NULLPTR,
                     QMap<PathStatus, QColor> statusColors = FileLineEdit::DefaultStatusColors(),
                     QString caption = "Select file");
        void SetFile(QString path);
    public slots:
        void OpenFileDialog(bool buttonClicked);
        void InputIsComplete();
        void InputIsInProgress(QString path);
        void setEnabled(bool enable);
        void setDisabled(bool disable);
    signals:
        void fileSelected(QString fileName);
    private:
        const QString dialogFilter;
        const QString dialogCaption;
        const QString dialogStartingDir;
        QStringList validFileExtensions;
        QMap<PathStatus, QColor> pathStatusColors;
        ColorMode fileLineColorMode;

        void extractFileExtensions();
        PathStatus retrievePathStatus(QString path);
        void applyColorStatus(PathStatus status, bool inputInProgress = true);
};

#endif // FILELINEEDIT_H
