#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // set QsciScintilla widget in the orignal source tab
    originalSrcTextEdit = new QsciScintilla(ui->originalSrcTab);
    ui->verticalLayout_2->addWidget(originalSrcTextEdit);

    // set QsciScintilla widget in the formatted source tab
    formattedSrcTextEdit = new QsciScintilla(ui->formattedSrcTab);
    ui->verticalLayout_3->addWidget(formattedSrcTextEdit);

    initializeSrcTextEdit(originalSrcTextEdit);
    initializeSrcTextEdit(formattedSrcTextEdit);

    // FIXME: Should we really do this with QStandardPaths?
    // initialize to home directory of the current user
    defaultFileOpenDir = QDir::homePath();
    previewer = nullptr;

    // set focus to original source preview tab
    ui->srcPreviewTabWidget->setCurrentIndex(0);

    // TODO: revisit initial strecthing proportions of the splits
    ui->splitter_2->setStretchFactor(1, 10);

    qDebug() << "Default Direcotory: " << defaultFileOpenDir;
}

MainWindow::~MainWindow()
{
    // NOTE: QSciScintilla objects will be deleted by ownership concept
    delete previewer;
    delete ui;
}

void MainWindow::initializeSrcTextEdit(QsciScintilla *textEdit)
{
    // make the editor read only
    textEdit->setReadOnly(true);
    // make the whitesplaces visible with a centered dot
    textEdit->setWhitespaceVisibility(QsciScintilla::WsVisible);
}

void MainWindow::on_openOriginalSrcToolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Preview File"),
                                                    defaultFileOpenDir,
                                                    tr("Source Files (*.c *.cpp *.c++ *.cc *.h *.hpp)"));

    if (!fileName.isEmpty() && QFileInfo(fileName).exists()) {
        // open file name in the preview text area
        if (previewer)
            delete previewer;

        previewer = new SrcFilePreviewer(fileName);
        previewer->ShowPreview(originalSrcTextEdit);
    }
}

void MainWindow::on_srcPreviewTabWidget_currentChanged(int index)
{
    // if the user clicks on formatted src preview tab we disable open button
    if (index == 1)
        ui->openOriginalSrcToolButton->setEnabled(false);
    else
        ui->openOriginalSrcToolButton->setEnabled(true);
}
