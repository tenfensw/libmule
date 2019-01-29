#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#ifdef Q_OS_WIN
# define DELFILECLI(fn) { \
	QString cmd = "del /q \"" + fn.replace("/", "\\") + "\""; \
	std::system(cmd.toStdString().c_str()); \
}
#else
# define DELFILECLI(fn) { \
	QString cmd = "rm -r -f \"" + fn + "\""; \
	std::system(cmd.toStdString().c_str()); \
}
#endif

SettingsDialog::SettingsDialog(QWidget *parent, int defaulttab) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    this->ui->setupUi(this);
    MuleStudioGlobalSettingsStructure str = this->findTheConfigFile();
    initializeSettingsDialog(defaulttab, str);
#ifndef MULETCM_ENABLEBROKENFEATURES
    this->ui->editToolchainButton->setVisible(false);
#endif
}

int SettingsDialog::selectToolchains(bool consoleOnly, QStringList args) {
    QStringList tclist;
    QString cachePath = "/tmp/muletcm-cache";
#ifdef Q_OS_WIN
    cachePath = qgetenv("TEMP") + "\\muletcm-cache";
#endif
    for (int i = 0; i < outStructure.configDefinedToolchains.count(); i++)
	tclist.append(outStructure.configDefinedToolchains.at(i)->toolchainName + " [" + outStructure.configDefinedToolchains.at(i)->toolchainMuleToolPath + "]");
    QString selectedToolchain;
    if (QFile::exists(cachePath) == false) {
	    if (consoleOnly == false)
	    	selectedToolchain = QInputDialog::getItem(this, tr("Select toolchain"), tr("Select a toolchain which should be used to build this project"), tclist, 0, false);
	    else {
		std::cout << "Pick a toolchain from this list:" << std::endl;
		for (int k = 0; k < tclist.count(); k++)
			std::cout << "[" << k + 1 << "]" << " " << QString(tclist[k].replace("[", "{").replace("]", "}")).toStdString() << std::endl;
		std::cout << "[" << tclist.count() + 1 << "]" << " Quit" << std::endl;
		std::cout << "You choose (number): ";
		int innum;
		std::cin >> innum;
		innum = innum - 1;
		if (innum > (tclist.count() - 1) || innum < 0)
			selectedToolchain = "";
		else
			selectedToolchain = tclist.at(innum);
		selectedToolchain.replace("{", "[");
		selectedToolchain.replace("}", "]");
	    }
	    QFile* qfcwrite = new QFile(cachePath);
	    if (qfcwrite->open(QIODevice::WriteOnly | QIODevice::Text) == true)
		qfcwrite->write(selectedToolchain.toUtf8());
	    qfcwrite->close();
	    delete qfcwrite;
    }
    else {
          QFile* qfcache = new QFile(cachePath);
	  if (qfcache->open(QIODevice::ReadOnly | QIODevice::Text) == false) {
		qfcache->close();
		delete qfcache;
		if (consoleOnly == false)
			QMessageBox::critical(this, "Toolchain Manager", "Saved cache file is corrupted or is unreadable.", QMessageBox::Ok);
		else
			std::cerr << "Saved cache file is corrupted or is unreadable." << std::endl;
		DELFILECLI(cachePath);
		return 2;
	  }
	  selectedToolchain = qfcache->readAll();
	  qfcache->close();
	  delete qfcache;
	  if (args.contains("-deploy") == true)
		DELFILECLI(cachePath);
    }
    if (selectedToolchain.isEmpty() == true)
	return 1;
    QString mtPath = selectedToolchain.split("[")[1].replace("]", "");
    QStringList newargs = args;
    for (int j = 0; j < newargs.count(); j++) {
	if (newargs.at(j).contains(" ") == true)
		newargs[j] = "\"" + newargs.at(j) + "\"";
	else if (newargs.at(j) == "-cli" || newargs.at(j) == "-tcselect" || newargs.at(j) == "-nosuccessmsg")
		newargs[j] = "";
    }
    QString cmd = mtPath + " " + newargs.join(" ");
    QMessageBox* msg = new QMessageBox();
    msg->setStandardButtons(0);
    msg->setText("Operation in progress, please wait, this might take a while...");
    if (consoleOnly == false) {
#ifndef Q_OS_WIN
	cmd = cmd + " > /tmp/muletcm-muletoolout.log 2>&1";
#else
	cmd = cmd + " > \"" + qgetenv("TEMP") + "\\muletcm-muletoolout.log\"";
#endif
	msg->show();
    }
    int returnres = std::system(cmd.toStdString().c_str());
    //qDebug() << returnres;
    delete msg;
    if (consoleOnly == false) {
	QString toread = "/tmp/muletcm-muletoolout.log";
#ifdef Q_OS_WIN
	toread = qgetenv("TEMP").replace("\\", "/") + "/muletcm-muletoolout.log";
#endif
	QFile* qfread = new QFile(toread);
	QString readcont = "";
	if (qfread->open(QIODevice::Text | QIODevice::ReadOnly) == true)
		readcont = qfread->readAll();
	qfread->close();
	delete qfread;
	DELFILECLI(toread);
	std::cerr << readcont.toStdString().c_str() << std::endl;
    	if (returnres != 0 || newargs.contains("-h") || newargs.join(" ").contains("-help"))
		QMessageBox::critical(this, "Toolchain Manager", "An error occured, here is what MuleTool says:\n" + readcont, QMessageBox::Ok);
	else {
		if (args.contains("-nosuccessmsg") == false)
			QMessageBox::information(this, "Toolchain Manager", "Operation completed successfully!", QMessageBox::Ok);
	}
    }
    return returnres;
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

MuleStudioGlobalSettingsStructure SettingsDialog::findTheConfigFile() {
	MuleStudioGlobalSettingsStructure result;
	QStringList possibleLocations;
	QString defaultLocation;
	QString foundLocation;
#ifdef Q_OS_WIN
	possibleLocations.append(QDir::homePath() + "/AppData/Roaming/Timkoisoft/libMule/ToolchainManager.mcfg");
	possibleLocations.append(QDir::homePath() + "/Timkoisoft/libMule/ToolchainManager.mcfg");
	possibleLocations.append("/Timkoisoft/libMule-SDK/Tools/SciTE/ToolchainManager.mcfg");
#else
	possibleLocations.append(QDir::homePath() + "/.config/Timkoisoft/mule/ToolchainManager.mcfg");
	possibleLocations.append(QDir::homePath() + "/.timkoisoft/mule/ToolchainManager.mcfg");
#endif
	//qDebug() << possibleLocations;
	defaultLocation = possibleLocations.at(1);
	for (int i = 0; i < possibleLocations.count(); i++) {
		if (QFile::exists(possibleLocations.at(i))) {
			foundLocation = possibleLocations.at(i);
			break;
		}
		else
			foundLocation = defaultLocation;
	}
	if (QFile::exists(foundLocation) == false) {
#ifndef Q_OS_WIN
		QProcess::startDetached("touch \"" + foundLocation + "\"");
#else
		QProcess::startDetached("echo > \"" + foundLocation + "\"");
#endif
	}
	result.baseConfigFile = foundLocation;
	QFile* fn = new QFile(foundLocation);
	if (fn->open(QIODevice::Text | QIODevice::ReadOnly) == false) {
		//qDebug() << fn << " returned false";
		fn->close();
		delete fn;
		return result;
	}
	QString wholeFile = QString(fn->readAll());
	//qDebug() << wholeFile;
	fn->close();
	delete fn;
	QString splitChar = "\n";
#ifdef Q_OS_WIN
	wholeFile.replace("\r\n", "\n");
#endif
	QStringList fileSplit = wholeFile.split("\n");
	//qDebug() << fileSplit;
	for (int j = 0; j < fileSplit.count(); j++) {
		if (fileSplit.at(j) != '#' && fileSplit.at(j).isEmpty() == false) {
			QString firstword = fileSplit.at(j).split(":").at(0);
			QString secondword = fileSplit.at(j).split(":").at(1);
			//qDebug() << "firstword = " << firstword << ", secondword = " << secondword;
			if (firstword.toLower() == "addtoolchain") {
				QString name;
				QString path;
				if (secondword.contains(",") == false) {
					name = "Unknown libMule SDK at line " + QString::number(j);
					path = secondword;
				}
				else {
					name = secondword.split(",").at(0);
					path = secondword.split(",").at(1);
				}
				MuleStudioToolchain* tc = new MuleStudioToolchain();
				tc->toolchainName = name;
				tc->toolchainMuleToolPath = path;
				result.configDefinedToolchains.append(tc);
				//qDebug() << tc->toolchainName;
				//qDebug() << tc->toolchainMuleToolPath;
			}
		}
	}
	return result;
}

void SettingsDialog::on_okayButton_clicked()
{
    convertChangedSettingsToStructure();
    flushToolchains(outStructure);
    QApplication::exit(0);
}

void SettingsDialog::on_cancelButton_clicked()
{
    std::exit(0);
}

void SettingsDialog::flushToolchains(MuleStudioGlobalSettingsStructure structure) {
	QString out = "# Automatically saved by libMule SDK Toolchain Manager " + QString(TOOLVERSION);
	out = out + "\n";
	for (int i = 0; i < structure.configDefinedToolchains.count(); i++)
		out = out + "AddToolchain:" + structure.configDefinedToolchains.at(i)->toolchainName + "," + structure.configDefinedToolchains.at(i)->toolchainMuleToolPath + "\n";
#ifdef Q_OS_WIN
	out = out.replace("\n", "\r\n");
#endif
	QFile* qf = new QFile(structure.baseConfigFile);
	if (qf->open(QIODevice::WriteOnly | QIODevice::Text) == false) {
		qf->close();
		delete qf;
		return;
	}
	qf->write(out.toUtf8());
	qf->close();
	delete qf;
}

void SettingsDialog::convertChangedSettingsToStructure() {
    MuleStudioGlobalSettingsStructure newStructure;
    newStructure.baseConfigFile = outConfigFile;
    newStructure.projectDir = this->ui->projectDirEdit->text();
    newStructure.font = this->ui->fontBox->currentFont().family();
    newStructure.fontSize = this->ui->fontSizeBox->value();
    newStructure.darkThemeFriendly = this->ui->darkThemeCheckBox->isChecked();
    QList<MuleStudioToolchain*> newToolchainsList;
    for (int i = 0; i < this->ui->listWidget->count(); i++) {
        MuleStudioToolchain* tc = new MuleStudioToolchain();
        QString tcname = QString((this->ui->listWidget->item(i)->text().split("["))[0]);
        QString tcmuletool = QString((this->ui->listWidget->item(i)->text().split("["))[1]);
        tcname = tcname.remove(tcname.length() - 1, 1);
        tcmuletool = tcmuletool.remove(tcmuletool.length() - 1, 1);
        tc->toolchainName = tcname;
        tc->toolchainMuleToolPath = tcmuletool;
        newToolchainsList.append(tc);
    }
    newStructure.configDefinedToolchains = newToolchainsList;
    outStructure = newStructure;
}

void SettingsDialog::initializeSettingsDialog(int defaulttab, MuleStudioGlobalSettingsStructure settingsStructure) {
    outStructure = settingsStructure;
    outConfigFile = settingsStructure.baseConfigFile;
    //qDebug() << outConfigFile;
    if (outConfigFile.isEmpty())
        setEnabledStateToEverything(false);
    else {
        this->ui->projectDirEdit->setText(settingsStructure.projectDir);
        this->ui->fontBox->setFont(QFont(settingsStructure.font, 10));
        this->ui->fontSizeBox->setValue(settingsStructure.fontSize);
        this->ui->darkThemeCheckBox->setChecked(settingsStructure.darkThemeFriendly);
	//qDebug() << settingsStructure.configDefinedToolchains.size();
        for (int i = 0; i < settingsStructure.configDefinedToolchains.size(); i++) {
	    //qDebug() << settingsStructure.configDefinedToolchains.at(i)->toolchainName;
            this->ui->listWidget->addItem(settingsStructure.configDefinedToolchains[i]->toolchainName + " [" + settingsStructure.configDefinedToolchains[i]->toolchainMuleToolPath + "]");
	}

    }
    this->ui->muleStudioAboutLabel->setText(this->ui->muleStudioAboutLabel->text().replace("^V^", TOOLVERSION));
    this->ui->tabWidget->removeTab(0);
    this->ui->tabWidget->setCurrentIndex(0);
}

void SettingsDialog::setEnabledStateToEverything(bool state) {
    this->ui->fontBox->setEnabled(state);
    this->ui->fontSizeBox->setEnabled(state);
    this->ui->addToolchainButton->setEnabled(state);
    this->ui->removeToolchainButton->setEnabled(state);
    this->ui->editToolchainButton->setEnabled(state);
    this->ui->listWidget->setEnabled(state);
    this->ui->projectDirEdit->setEnabled(state);
    this->ui->projectDirButton->setEnabled(state);
}

void SettingsDialog::on_projectDirButton_clicked()
{
    QString out = QFileDialog::getExistingDirectory(this, tr("Default projects directory"), QDir::homePath());
    if (out.isEmpty())
        return;
    this->ui->projectDirEdit->setText(out);
}

void SettingsDialog::on_addToolchainButton_clicked()
{
    QString tcName = QInputDialog::getText(this, tr("Add"), tr("Enter toolchain name. It will be used so that you could distinguish it later from other toolchains."), QLineEdit::Normal, "My toolchain");
    if (tcName.isEmpty())
        return;
    tcName.replace(",", " ");
    tcName.replace("(", " ");
    tcName.replace(")", " ");
    tcName.replace("[", " ");
    tcName.replace("]", " ");
    tcName.replace("{", " ");
    tcName.replace("}", " ");
    QString tcMuleToolFileType = "MuleTool application (muletool)";
#ifdef Q_OS_WIN
    tcMuleToolFileType = "MuleTool application (muletool.exe)";
#endif
    QString tcMuleToolPath = QFileDialog::getOpenFileName(this, tr("Add"), QDir::homePath(), tcMuleToolFileType + ";;" + tr("Anything (*)"));
    if (tcMuleToolPath.isEmpty())
        return;
    this->ui->listWidget->addItem(tcName + " [" + tcMuleToolPath + "]");
}

void SettingsDialog::on_editToolchainButton_clicked()
{
    if (this->ui->listWidget->selectedItems().count() != 1)
        return;
    editToolchain(this->ui->listWidget->currentItem()->text());
}

void SettingsDialog::editToolchain(const QString& tcitemstring) {
    QString configFile;
    QString muleTool = tcitemstring;
    muleTool = muleTool.split("[")[1];
    muleTool = muleTool.remove((muleTool.length() - 1), 1);
    QStringList muleToolArgs;
    muleToolArgs.append("-quiet");
    muleToolArgs.append("-internal-printconfigfile");
    QProcess* qproc = new QProcess(this);
    qproc->start(muleTool, muleToolArgs, QIODevice::ReadWrite);
    qproc->waitForFinished();
    configFile = qproc->readAllStandardOutput();
    configFile.replace("\r", "\n");
    configFile.replace("\n", "");
    //qDebug() << "muleTool = " << muleTool << ", configFile = " << configFile << ", STDERR = " << qproc->readAllStandardError();
    if (qproc->exitCode() != 0) {
        qproc->close();
        delete qproc;
        QMessageBox::critical(this, tr("Error"), tr("Something went wrong"), QMessageBox::Ok);
        return;
    }
    qproc->close();
    delete qproc;
    if (configFile.isEmpty()) {
        QMessageBox::critical(this, tr("Error"), tr("Something went wrong"), QMessageBox::Ok);
        return;
    }

}

void SettingsDialog::on_removeToolchainButton_clicked()
{
    if (this->ui->listWidget->selectedItems().count() != 1)
        return;
    if (QMessageBox::question(this, tr("Remove"), tr("Are you sure?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        delete this->ui->listWidget->takeItem(this->ui->listWidget->currentRow());
}

MuleStudioGlobalSettingsStructure SettingsDialog::getNewSettingsStructure() {
    return outStructure;
}
