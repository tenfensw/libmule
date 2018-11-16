#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QIODevice>
#include <QFileInfo>
#include <QDateTime>
#include <QDir>
#include <QCloseEvent>
#include <QFileDialog>
#include <QProcess>
#include <QDebug>
#include <QtDebug>
#include <string>
#include <cstdlib>
#include <cstring>
#include "muletoolclass.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initializeForm();
    void fillUpInfo();
    void probeCompiler();
    void probeLinker();
    void saveToFile(const QString& fn);
    QString getParentDirectory(const QString& origfn);
    QString selectBinaryFile();
    QString selectDirectory();
    void beforeQuittingTheProgram();

protected:
    void closeEvent(QCloseEvent* qce);

private slots:
    void on_compilerCppSelectButton_clicked();

    void on_linkerLdSelectButton_clicked();

    void on_sdkPathSelectButton_clicked();

    void on_currentConfigSelectButton_clicked();

    void on_actionAbout_this_tool_triggered();

    void on_actionAbout_Qt_framework_triggered();

    void on_saveButton_clicked();

    void on_actionExit_triggered();

    void on_actionSave_triggered();

    void on_actionLoad_triggered();

private:
    Ui::MainWindow *ui;
    MuleToolClass* muleToolClassInstance;
    QString theConfigFile;
};

#endif // MAINWINDOW_H
