#include "notebook.h"
#include "ui_notebook.h"

Notebook::Notebook(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Notebook)
{
    ui->setupUi(this);
    codeEditor = new CodeEditor(this);
    this->setCentralWidget(codeEditor);

    struct passwd *pw = getpwuid(getuid());
    homeDirectory = QString(pw->pw_dir);
    this->loadSettings();

    connect(ui->actionNew, &QAction::triggered, this, &Notebook::newDocument);
    connect(ui->actionOpen, &QAction::triggered, this, &Notebook::open);
    connect(ui->actionSave, &QAction::triggered, this, &Notebook::save);
    connect(ui->actionSave_As, &QAction::triggered, this, &Notebook::saveAs);
    connect(ui->actionExit, &QAction::triggered, this, &Notebook::exit);
    connect(ui->actionCopy, &QAction::triggered, this, &Notebook::copy);
    connect(ui->actionPaste, &QAction::triggered, this, &Notebook::paste);
    connect(ui->actionUndo, &QAction::triggered, this, &Notebook::undo);
    connect(ui->actionRedo, &QAction::triggered, this, &Notebook::redo);
    connect(ui->actionSelect_font, &QAction::triggered, this, &Notebook::selectFont);
    connect(ui->actionSelect_fgcolor, &QAction::triggered, this, &Notebook::selectForegroundColor);
    connect(ui->actionSelect_bgcolor, &QAction::triggered, this, &Notebook::selectBackgroundColor);
    connect(ui->actionSelect_highlight_color, &QAction::triggered, this, &Notebook::selectHighlightColor);
    connect(ui->actionToggle_highlight, &QAction::triggered, this, &Notebook::toggleHighlight);
}

Notebook::~Notebook()
{
    delete ui;
}

void Notebook::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->matches(QKeySequence::Save))
    {
        save();
    }
    if (keyEvent->matches(QKeySequence::Open))
    {
        open();
    }
}

void Notebook::loadSettings() {
    settings = new QSettings(homeDirectory + "/.notebook", QSettings::Format::IniFormat, nullptr);
    QString fontFamily = settings->value("fontFamily", "Chalkboard").toString();
    int fontSize = settings->value("fontSize", "14").toInt();
    QColor fgcolor = QColor(settings->value("foregroundColor", "FFFFFF").toString().toInt(nullptr, 16));
    QColor bgcolor = QColor(settings->value("backgroundColor", "0099FF").toString().toInt(nullptr, 16));
    QFont font = QFont(fontFamily, fontSize, -1, false);
    codeEditor->setFont(font);
    QPalette p = codeEditor->palette();
    p.setColor(QPalette::Text, fgcolor);
    p.setColor(QPalette::Base, bgcolor);
    codeEditor->setPalette(p);
    QColor highlightColor = QColor(settings->value("highlightColor", "9900FF").toString().toInt(nullptr, 16));
    codeEditor->setHighlightColor(highlightColor);
    bool highlight = settings->value("highlight", "false").toBool();
    codeEditor->setHighlight(highlight);
}

void Notebook::newDocument() {
    currentFile.clear();
    codeEditor->setPlainText(QString());
}

void Notebook::open() {
    QString directory = homeDirectory + "/Documents";
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file", directory, nullptr);
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
       // QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
       return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    codeEditor->setPlainText(text);
    file.close();
}

void Notebook::save()
{
    QString directory = homeDirectory + "/Documents";
    QString fileName;
    // If we don't have a filename from before, get one.
    if (currentFile.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, "Save", directory, nullptr);
        currentFile = fileName;
    } else {
        fileName = currentFile;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        // QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = codeEditor->toPlainText();
    out << text;
    file.close();
}

void Notebook::saveAs()
{
    QString directory = homeDirectory + "/Documents";
    QString fileName = QFileDialog::getSaveFileName(this, "Save as", directory, nullptr);
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        // QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = codeEditor->toPlainText();
    out << text;
    file.close();
}

void Notebook::exit()
{
    QCoreApplication::quit();
}

void Notebook::copy()
{
#if QT_CONFIG(clipboard)
    codeEditor->copy();
#endif
}

void Notebook::paste()
{
#if QT_CONFIG(clipboard)
    codeEditor->paste();
#endif
}

void Notebook::undo()
{
     codeEditor->undo();
}

void Notebook::redo()
{
    codeEditor->redo();
}

void Notebook::selectFont()
{
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, this);
    if (fontSelected)
        codeEditor->setFont(font);
}

void Notebook::selectForegroundColor()
{
    QColor color = QColorDialog::getColor(Qt::black,this);

    QPalette p = codeEditor->palette();
    p.setColor(QPalette::Text, color);
    codeEditor->setPalette(p);
}

void Notebook::selectBackgroundColor()
{
    QColor color = QColorDialog::getColor(Qt::white,this);

    QPalette p = codeEditor->palette();
    p.setColor(QPalette::Base, color);
    codeEditor->setPalette(p);
}

void Notebook::selectHighlightColor()
{
    QColor color = QColorDialog::getColor(Qt::white,this);
    codeEditor->setHighlightColor(color);
}

void Notebook::toggleHighlight()
{
    this->codeEditor->toggleHighlight();
}
