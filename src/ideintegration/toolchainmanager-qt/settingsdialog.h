#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QInputDialog>
#include <iostream>
#include <cstdlib>
#include <csignal>

#define TOOLVERSION QString("0.4.0")
#ifndef LIBMULESDKPREFIX
#define LIBMULESDKPREFIX "."
#endif

namespace Ui {
    class SettingsDialog;
}

struct MuleStudioToolchain {
    QString toolchainName;
    QString toolchainMuleToolPath;
};

struct MuleStudioGlobalSettingsStructure {
    QString baseConfigFile;
    QString projectDir;
    QString font;
    bool darkThemeFriendly;
    int fontSize;
    QList<MuleStudioToolchain*> configDefinedToolchains;

};

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0, int defaulttab = 0);
    ~SettingsDialog();
    int selectToolchains(bool consoleOnly = false, QStringList args = QStringList());
    MuleStudioGlobalSettingsStructure getNewSettingsStructure();

private slots:
    void on_okayButton_clicked();

    void on_cancelButton_clicked();

    void on_projectDirButton_clicked();

    void on_addToolchainButton_clicked();

    void on_editToolchainButton_clicked();

    void on_removeToolchainButton_clicked();

private:
    void initializeSettingsDialog(int defaulttab, MuleStudioGlobalSettingsStructure settingsStructure);
    void setEnabledStateToEverything(bool state = true);
    void editToolchain(const QString& tcitemstring);
    void convertChangedSettingsToStructure();
    void flushToolchains(MuleStudioGlobalSettingsStructure structure);
    MuleStudioGlobalSettingsStructure findTheConfigFile();

    Ui::SettingsDialog *ui;
    QString outConfigFile;
    MuleStudioGlobalSettingsStructure outStructure;
};

#endif // SETTINGSDIALOG_H
