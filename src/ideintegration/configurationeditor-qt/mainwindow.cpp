//
// This file is a part of libMule - Microcontroller-Universal 
// Library (that is extendable)
// Copyright (C) 2018 Tim K <timprogrammer@rambler.ru>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//

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
    ui->setupUi(this);
    ui->titleLabel->setText(ui->titleLabel->text().replace("^V^", QString::fromLocal8Bit(TOOLVERSION.c_str())).replace("^Y^", QString::number(QDateTime::currentDateTime().date().year())).replace("^L^", "GNU LGPLv2.1"));
    ui->titleLabel->resize(width(), ui->titleLabel->height());
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
        ui->targetLabel->setText(ui->targetLabel->text().replace("^T^", "custom"));
    }
    else
        fillUpInfo();
    ui->currentConfigEdit->setText(theConfigFile);
}

void MainWindow::fillUpInfo() {
    ui->compilerCppEdit->setText(QString::fromLocal8Bit(muleToolClassInstance->getVariableValue("C++").c_str()));
    ui->compilerFlagsEdit->setText(QString::fromLocal8Bit(muleToolClassInstance->getVariableValue("C++FLAGS").c_str()));
    ui->linkerLdEdit->setText(QString::fromLocal8Bit(muleToolClassInstance->getVariableValue("LD").c_str()));
    ui->linkerFlagsEdit->setText(QString::fromLocal8Bit(muleToolClassInstance->getVariableValue("LDFLAGS").c_str()));
    ui->deployEdit->setText(QString::fromLocal8Bit(muleToolClassInstance->getVariableValue("DEPLOY").c_str()));
    ui->runEdit->setText(QString::fromLocal8Bit(muleToolClassInstance->getVariableValue("RUN").c_str()));
    ui->targetLabel->setText(ui->targetLabel->text().replace("^T^", QString::fromLocal8Bit(muleToolClassInstance->getVariableValue("TARGET").c_str())));
    ui->sdkPathEdit->setText(QString::fromLocal8Bit(muleToolClassInstance->getVariableValue("LIBMULE").c_str()));
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
    ui->compilerCppEdit->setText(selectBinaryFile());
    probeCompiler();
}

void MainWindow::probeCompiler() {
    QString proposedcompiler = ui->compilerCppEdit->text();
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
    if (ui->compilerFlagsEdit->text().isEmpty() == true)
        ui->compilerFlagsEdit->setText(possibleFlags);
    if (ui->linkerLdEdit->text().isEmpty() == true && ui->linkerFlagsEdit->text().isEmpty()) {
        ui->linkerLdEdit->setText(proposedcompiler);
        probeLinker();
    }
    procrunner->close();
    procrunner->deleteLater();
}

void MainWindow::on_linkerLdSelectButton_clicked()
{
    ui->linkerLdEdit->setText(selectBinaryFile());
    probeLinker();
}

void MainWindow::probeLinker() {
    bool isGNU = false;
    bool isLLVM = false;
    QString proposedlinker = ui->linkerLdEdit->text();
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
    if (ui->linkerFlagsEdit->text().isEmpty() == true) {
        if (isGNU == true) {
            possibleFlags = "-Os";
            if (isLLVM == true)
                possibleFlags = possibleFlags + " -static-libc++ -static-libstdc++ -static-compiler-rt";
            else
                possibleFlags = possibleFlags + " -static-libstdc++ -static-libgcc";
        }
        ui->linkerFlagsEdit->setText(possibleFlags);
    }
}

QString MainWindow::selectDirectory() {
    return QFileDialog::getExistingDirectory(this, tr("Select an existing directory"), QDir::homePath(), QFileDialog::ShowDirsOnly);
}

void MainWindow::on_sdkPathSelectButton_clicked()
{
    ui->sdkPathEdit->setText(selectDirectory());
    if (QFile::exists(ui->sdkPathEdit->text() + "/libMule.a"))
        ui->sdkPathEdit->setText(ui->sdkPathEdit->text() + "/..");
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
    saveToFile(ui->currentConfigEdit->text());
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
    proposedconfig = proposedconfig + "# Specify the location of libMule first\nLIBMULE:" + ui->sdkPathEdit->text() + "\n# Now name the target\nTARGET:custom\n";
    proposedconfig = proposedconfig + "\n# Various compiler information\nCC:" + ui->compilerCppEdit->text() + "\nCXX:" + ui->compilerCppEdit->text() + "\n";
    proposedconfig = proposedconfig + "LD:" + ui->linkerLdEdit->text() + "\n\n# Flags for various compilers and utilities\n";
    proposedconfig = proposedconfig + "CFLAGS:" + ui->compilerFlagsEdit->text() + "\nCXXFLAGS:" + ui->compilerFlagsEdit->text() + "\n";
    proposedconfig = proposedconfig + "LDFLAGS:" + ui->linkerFlagsEdit->text() + "\n\n# Now various deployment info";
    proposedconfig = proposedconfig + "\nDEPLOY:" + ui->deployEdit->text() + "\nRUN:" + ui->runEdit->text() + "\n# End of file";
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
    close();
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
