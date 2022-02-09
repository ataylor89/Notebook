#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include "codeeditor.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QFontDialog>
#include <QColorDialog>
#include <QSettings>
#include <QKeyEvent>
#include <QKeySequence>

QT_BEGIN_NAMESPACE
namespace Ui { class Notebook; }
QT_END_NAMESPACE

class Notebook : public QMainWindow
{
    Q_OBJECT

public:
    Notebook(QWidget *parent = nullptr);
    ~Notebook();

protected:
    void keyPressEvent(QKeyEvent *keyEvent);

private slots:
    void loadSettings();
    void newDocument();
    void open();
    void save();
    void saveAs();
    void exit();
    void copy();
    void paste();
    void undo();
    void redo();
    void selectFont();
    void selectForegroundColor();
    void selectBackgroundColor();
    void selectHighlightColor();
    void toggleHighlight();

private:
    Ui::Notebook *ui;
    QSettings *settings;
    QString currentFile;
    QString homeDirectory;
    CodeEditor *codeEditor;
};

#endif // NOTEBOOK_H
