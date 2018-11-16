#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    initializeForm();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeForm() {
    this->ui->setupUi(this);
    this->ui->titleLabel->setText(this->ui->titleLabel->text().replace("^V^", QString::fromLocal8Bit(TOOLVERSION.c_str())).replace("^Y^", QString::number(QDateTime::currentDateTime().date().year())).replace("^L^", "GNU LGPLv2.1"));
    this->ui->titleLabel->resize(this->width(), this->ui->titleLabel->height());
    char* argvfake[2];
    argvfake[0] = "one";
    argvfake[1] = "-lazy";
    int argcfake = 2;
    muleToolClassInstance = new MuleToolClass(argcfake, argvfake);
    theConfigFile = QString::fromLocal8Bit(muleToolClassInstance->getVariableValue("INTERNAL_CONFNAME").c_str());
    if (theConfigFile == "nf" || theConfigFile.isEmpty()) {
        theConfigFile = QDir::homePath() + "/.timkoisoft/libmule/MuleTool.mcfg";
#ifdef Q_OS_WIN
        theConfigFile = userHomeDir + "\\timkoisoft\\libmule\\MuleTool.mcfg";
#endif
        this->ui->targetLabel->setText(this->ui->targetLabel->text().replace("^T^", "custom"));
    }
    else
        fillUpInfo();
    this->ui->currentConfigEdit->setText(theConfigFile);
}

void MainWindow::fillUpInfo() {
    this->ui->compilerCppEdit->setText(QString::fromLocal8Bit(muleToolClassInstance->getVariableValue("C++").c_str()));
    this->ui->compilerFlagsEdit->setText(QString::fromLocal8Bit(muleToolClassInstance->getVariableValue("C++FLAGS").c_str()));
    this->ui->linkerLdEdit->setText(QString::fromLocal8Bit(muleToolClassInstance->getVariableValue("LD").c_str()));
    this->ui->linkerFlagsEdit->setText(QString::fromLocal8Bit(muleToolClassInstance->getVariableValue("LDFLAGS").c_str()));
    this->ui->deployEdit->setText(QString::fromLocal8Bit(muleToolClassInstance->getVariableValue("DEPLOY").c_str()));
    this->ui->runEdit->setText(QString::fromLocal8Bit(muleToolClassInstance->getVariableValue("RUN").c_str()));
    this->ui->targetLabel->setText(this->ui->targetLabel->text().replace("^T^", QString::fromLocal8Bit(muleToolClassInstance->getVariableValue("TARGET").c_str())));
    this->ui->sdkPathEdit->setText(QString::fromLocal8Bit(muleToolClassInstance->getVariableValue("LIBMULE").c_str()));
}

QString MainWindow::selectBinaryFile() {
    QString binType = "Anything (*)";
#ifdef Q_OS_WIN
    binType = "Anything (*.exe)";
#endif
    return QFileDialog::getOpenFileName(this, tr("Select binary file"), QDir::homePath(), tr(binType.toStdString().c_str()));
}

void MainWindow::on_compilerCppSelectButton_clicked()
{
    this->ui->compilerCppEdit->setText(selectBinaryFile());
    probeCompiler();
}

void MainWindow::probeCompiler() {
    QString proposedcompiler = this->ui->compilerCppEdit->text();
    QString possibleFlags = "-c";
    QStringList args;
    bool isGNU = false;
    bool isLLVM = false;
    args.append("-v");
    QProcess* procrunner = new QProcess(this);
    procrunner->start(proposedcompiler, args);
    procrunner->waitForFinished();
    if (procrunner->exitCode() == 0) {
        QString wholestderr = procrunner->readAllStandardError().toLower();
        if (wholestderr.contains("clang") || wholestderr.contains("gcc")) {
            isGNU = true;
            if (wholestderr.contains("clang"))
                isLLVM = true;
        }
    }
    if (isGNU == true)
        possibleFlags = "-c -std=c++0x -Os -Wall -Wextra";
    if (this->ui->compilerFlagsEdit->text().isEmpty() == true)
        this->ui->compilerFlagsEdit->setText(possibleFlags);
    if (this->ui->linkerLdEdit->text().isEmpty() == true && this->ui->linkerFlagsEdit->text().isEmpty()) {
        this->ui->linkerLdEdit->setText(proposedcompiler);
        probeLinker();
    }
    procrunner->close();
    procrunner->deleteLater();
}

void MainWindow::on_linkerLdSelectButton_clicked()
{
    this->ui->linkerLdEdit->setText(selectBinaryFile());
    probeLinker();
}

void MainWindow::probeLinker() {
    bool isGNU = false;
    bool isLLVM = false;
    QString proposedlinker = this->ui->linkerLdEdit->text();
    QString possibleFlags = "";
    QStringList args;
    args.append("-v");
    QProcess* procrunner = new QProcess(this);
    procrunner->start(proposedlinker, args);
    procrunner->waitForFinished();
    if (procrunner->exitCode() == 0) {
        QString wholestderr = procrunner->readAllStandardError().toLower();
        if (wholestderr.contains("clang") || wholestderr.contains("gcc")) {
            isGNU = true;
            if (wholestderr.contains("clang"))
                isLLVM = true;
        }
    }
    procrunner->close();
    procrunner->deleteLater();
    if (this->ui->linkerFlagsEdit->text().isEmpty() == true) {
        if (isGNU == true) {
            possibleFlags = "-Os";
            if (isLLVM == true)
                possibleFlags = possibleFlags + " -static-libc++ -static-libstdc++ -static-compiler-rt";
            else
                possibleFlags = possibleFlags + " -static-libstdc++ -static-libgcc";
        }
        this->ui->linkerFlagsEdit->setText(possibleFlags);
    }
}

QString MainWindow::selectDirectory() {
    return QFileDialog::getExistingDirectory(this, tr("Select an existing directory"), QDir::homePath(), QFileDialog::ShowDirsOnly);
}

void MainWindow::on_sdkPathSelectButton_clicked()
{
    this->ui->sdkPathEdit->setText(selectDirectory());
    if (QFile::exists(this->ui->sdkPathEdit->text() + "/libMule.a"))
        this->ui->sdkPathEdit->setText(this->ui->sdkPathEdit->text() + "/..");
}

void MainWindow::on_currentConfigSelectButton_clicked()
{
    QString theFileName = QFileDialog::getOpenFileName(this, tr("Select a custom configuration file"), QDir::homePath(), tr("libMule Toolchain Manager configuration files (*.mcfg)"));
    if (QFile::exists(theFileName)) {
        // reinitialize MuleToolClass
        delete muleToolClassInstance;
        int fakeargc = 4;
        char* fakeargv[fakeargc];
        fakeargv[0] = "two";
        fakeargv[1] = "-config";
        fakeargv[2] = strdup(theFileName.toStdString().c_str());
        fakeargv[3] = "-lazy";
        muleToolClassInstance = new MuleToolClass(fakeargc, fakeargv);
        fillUpInfo();
    }
}

void MainWindow::on_actionAbout_this_tool_triggered()
{
    QMessageBox::information(this, tr("About this tool"), "libMule Toolchain Configuration Editor\nA Qt-based graphical application for creating MuleTool.mcfg files that can be used with muletool later.\nCopyright (c) Tim K 2018. Licensed under GNU LGPLv2.1.", QMessageBox::Ok);
}

void MainWindow::on_actionAbout_Qt_framework_triggered()
{
    QApplication::aboutQt();
}

void MainWindow::on_saveButton_clicked()
{
    saveToFile(this->ui->currentConfigEdit->text());
}

void MainWindow::saveToFile(const QString &fn) {
    if (QFile::exists(fn))
        QFile::remove(fn);
    QString parentdirectory = getParentDirectory(fn);
    qDebug() << parentdirectory;
    if (QDir(parentdirectory).exists() == false)
#ifdef Q_OS_WIN
        QProcess::startDetached("mkdir \"" + parentdirectory.replace("/", "\\") + "\"");
#else
        QProcess::startDetached("mkdir -p \"" + parentdirectory + "\"");
#endif

    QString proposedconfig = "# This configuration file was generated via libMule Toolchain Configuration Manager on " + QDateTime::currentDateTime().toString() + "\n";
    proposedconfig = proposedconfig + "# Specify the location of libMule first\nLIBMULE:" + this->ui->sdkPathEdit->text() + "\n# Now name the target\nTARGET:custom\n";
    proposedconfig = proposedconfig + "\n# Various compiler information\nCC:" + this->ui->compilerCppEdit->text() + "\nCXX:" + this->ui->compilerCppEdit->text() + "\n";
    proposedconfig = proposedconfig + "LD:" + this->ui->linkerLdEdit->text() + "\n\n# Flags for various compilers and utilities\n";
    proposedconfig = proposedconfig + "CFLAGS:" + this->ui->compilerFlagsEdit->text() + "\nCXXFLAGS:" + this->ui->compilerFlagsEdit->text() + "\n";
    proposedconfig = proposedconfig + "LDFLAGS:" + this->ui->linkerFlagsEdit->text() + "\n\n# Now various deployment info";
    proposedconfig = proposedconfig + "\nDEPLOY:" + this->ui->deployEdit->text() + "\nRUN:" + this->ui->runEdit->text() + "\n# End of file";
    QFile* fwriter = new QFile(fn);
    if (fwriter->open(QIODevice::WriteOnly | QIODevice::Text) == true) {
        fwriter->write(proposedconfig.toUtf8());
        QMessageBox::information(this, tr("Information"), tr("Saved successfully!"), QMessageBox::Ok);
    }
    else
        QMessageBox::critical(this, tr("Information"), tr("Failed to save!"), QMessageBox::Ok);
    fwriter->close();
    fwriter->deleteLater();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}


void MainWindow::closeEvent(QCloseEvent *qce) {
    beforeQuittingTheProgram();
    qce->accept();
}


void MainWindow::beforeQuittingTheProgram() {
    if (QMessageBox::question(this, tr("Question"), tr("Save changes?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        on_saveButton_clicked();
}

void MainWindow::on_actionSave_triggered()
{
    on_saveButton_clicked();
}

void MainWindow::on_actionLoad_triggered()
{
    on_currentConfigSelectButton_clicked();
}

QString MainWindow::getParentDirectory(const QString &origfn) {
    QString result = origfn;
#ifdef Q_OS_WIN
    result = result.replace("\\", "/");
#endif
    bool foundfirstslash = false;
    int index = result.length();
    while (foundfirstslash == false) {
        index = index - 1;
        if (result[index] == '/')
            foundfirstslash = true;
        result[index] = '&';
    }
    result = result.replace("&", "");
    return result;
}
