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
