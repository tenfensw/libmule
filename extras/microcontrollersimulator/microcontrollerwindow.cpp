// Mule Microcontroller Simulator - Arduino-like microcontroller simulator for debugging and testing purposes
// Copyright (C) 2018 Tim K <timprogrammer@rambler.ru>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "microcontrollerwindow.h"
#include "ui_microcontrollerwindow.h"

MicrocontrollerWindow::MicrocontrollerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MicrocontrollerWindow)
{
    this->ui->setupUi(this);
    this->initializeSocket();
}

void MicrocontrollerWindow::flushPin(const MuleMicrocontrollerSimulatorPin &pin) {
    QString proposedSaveLocation = sockBaseDir + "/PIN" + QString::number(pin.num);
    QString saveString = "type=^T^,pud=^P^,pwm=^W^,val=^V^,dc=^D^,maxdc=^M^,freq=^F^,inout=^I^";
    if (pin.digital == true)
        saveString.replace("^T^", "d");
    else
        saveString.replace("^T^", "a");
    if (pin.pullUpDown == 0)
        saveString.replace("^P^", "off");
    else if (pin.pullUpDown == 1)
        saveString.replace("^P^", "down");
    else if (pin.pullUpDown == 2)
        saveString.replace("^P^", "up");
    else
        saveString.replace("^P^", "off");
    if (pin.pwmCapable == true)
        saveString.replace("^W^", "yes");
    else
        saveString.replace("^W^", "no");
    if (pin.digital == true) {
        saveString.replace("^V^", QString::number(pin.currentDigitalValue));
        saveString.replace("^F^", QString::number(MULEMICROSIM_DIGITALPINFREQUENCY));
    }
    else {
        saveString.replace("^V^", "U");
        saveString.replace("^F^", QString::number(MULEMICROSIM_ANALOGPINFREQUENCY));
    }
    if (pin.pwmCapable == false) {
        saveString.replace("^D^", "U");
        saveString.replace("^M^", "U");
    }
    else {
        saveString.replace("^D^", QString::number(pin.currentDutyCycle));
        saveString.replace("^M^", QString::number(pin.maxDutyCycle));
    }
    if (pin.inputPin == true)
    	saveString.replace("^I^", "i");
    else
	saveString.replace("^I^", "o");
    QFile* file = new QFile(proposedSaveLocation);
    if (file->open(QIODevice::WriteOnly | QIODevice::Text) == false) {
        file->close();
        delete file;
        qDebug() << "save failed";
        return;
    }
    file->write(saveString.toUtf8());
    file->close();
    delete file;

}

void MicrocontrollerWindow::initializeSocket() {
    qDebug() << "Mule Microcontroller Simulator";
    qDebug() << "Compiled on " << __DATE__;
    sockBaseDir = "/tmp/mulemicrosim";
#ifdef Q_OS_WIN
    qDebug() << "Microsoft triggers enabled";
    sockBaseDir = QString(qgetenv("TEMP")).replace("\\", "/") + "/mulemicrosim";
#endif
    qDebug() << "Proposed socket dir is " << sockBaseDir;
    if (QDir(sockBaseDir).exists() == true) {
        if (QMessageBox::question(this, "Mule Microcontroller Simulator", "It looks like another instance of Mule Microcontroller Simulator is already running or it crashed previously. As such, all previous sockets were left open. Would you like to close these old sockets and open new ones? Notice that you cannot run more than two instances of Mule Microcontroller Simulator at the same time and, if you will try too, they both won't work correctly.", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
            QString rmCmd = "rm -r -f \"" + sockBaseDir + "\"";
#ifdef Q_OS_WIN
            rmCmd = "rd /q \"" + sockBaseDir + "\"";
#endif
            std::system(rmCmd.toStdString().c_str());
        }
        else
            std::exit(-4);
    }
    QString mkdirCmd = "mkdir -p \"" + sockBaseDir + "\"";
#ifdef Q_OS_WIN
    mkdirCmd = "mkdir \"" + sockBaseDir + "\"";
#endif
    if (std::system(mkdirCmd.toStdString().c_str()) != 0) {
        QMessageBox::critical(this, "Mule Microcontroller Simulator", "Creating new sockets ultimately failed. :-(", QMessageBox::Ok);
        QApplication::exit(1);
    }
    // create the fake devices now
    for (int i = 0; i < 4; i++) {
        MuleMicrocontrollerSimulatorPin mspin;
        mspin.num = i;
        if (i < 2)
            mspin.digital = true;
        else
            mspin.digital = false;
        mspin.pullUpDown = 0;
        if (mspin.digital == true && i != 1) {
            mspin.pwmCapable = false;
            mspin.maxDutyCycle = -1;
            mspin.currentDutyCycle = -1;
            mspin.currentDigitalValue = 0;
        }
        else {
            mspin.pwmCapable = true;
            mspin.maxDutyCycle = 255;
            mspin.currentDutyCycle = 0;
            if (i != 1)
                mspin.currentDigitalValue = -1;
            else
                mspin.currentDigitalValue = 0;
        }
        flushPin(mspin);
        qDebug() << "Created pin #" << i;
    }
    reloadDeviceVector();
    //deviceConnected = (QString*)(calloc(4, sizeof(QString)));
    for (int p = 0; p < 4; p++)
        deviceConnected.push_back(QString("U"));
    qDebug() << "Now initialize the serial output";
    QFile* serialFile = new QFile(sockBaseDir + "/SERIAL");
    if (serialFile->open(QIODevice::Text | QIODevice::WriteOnly) == false) {
        serialFile->close();
        delete serialFile;
        this->ui->serialEdit->setText("Connection lost");
    }
    serialFile->write(QString(QString("Initialized serial output on ") + QDateTime::currentDateTime().toString() + "\n\n\n").toUtf8());
    serialFile->close();
    delete serialFile;
    qDebug() << "initialize QTimer";
    timerUpdateBlocker = false;
    dcUpdateBlocker = false;
    globalTimer = new QTimer(this);
    globalTimer->setInterval(MULEMICROSIM_UPDATEINTERVAL);
    connect(globalTimer, SIGNAL(timeout()), this, SLOT(onGlobalTimerTimeout()));
    globalTimer->start();
}

void MicrocontrollerWindow::reloadDeviceVector() {
    deviceVector.clear();
    for (int i = 0; i < 4; i++) {
        qDebug() << "start loading pin " << i;
        QString proposedPath = sockBaseDir + "/PIN" + QString::number(i);
        qDebug() << "possible path is " << proposedPath;
        if (QFile::exists(proposedPath) == false) {
            QMessageBox::critical(this, "Mule Microcontroller Simulator", "At least information for pin " + QString::number(i) + " is corrupted, exiting now.", QMessageBox::Ok);
            QApplication::exit(3);
        }
        QFile* readFile = new QFile(proposedPath);
        if (readFile->open(QIODevice::ReadOnly | QIODevice::Text) == false) {
            readFile->close();
            delete readFile;
            QMessageBox::critical(this, "Mule Microcontroller Simulator", "Access is denied, exiting now.", QMessageBox::Ok);
            QApplication::exit(4);
        }
        QString readVal = QString(readFile->readAll());
        readFile->close();
        delete readFile;
        qDebug() << readVal;
        MuleMicrocontrollerSimulatorPin toadd;
        toadd.num = i;
        QStringList commaseperatedlist = readVal.split(",");
        for (int j = 0; j < commaseperatedlist.count(); j++) {
            QString halfone = commaseperatedlist.at(j).split("=").at(0);
            QString halftwo = commaseperatedlist.at(j).split("=").at(1);
            halfone = halfone.toLower();
            halftwo = halftwo.toLower();
            qDebug() << halfone;
            qDebug() << halftwo;
            if (halfone == "type") {
                if (halftwo == "d")
                    toadd.digital = true;
                else
                    toadd.digital = false;
            }
            else if (halfone == "pud") {
                if (halftwo == "down")
                    toadd.pullUpDown = 1;
                else if (halftwo == "up")
                    toadd.pullUpDown = 2;
                else
                    toadd.pullUpDown = 0;
            }
            else if (halfone == "pwm") {
                if (halftwo == "yes")
                    toadd.pwmCapable = true;
                else
                    toadd.pwmCapable = false;
            }
            else if (halfone == "val") {
                if (halftwo != "u")
                    toadd.currentDigitalValue = halftwo.toInt();
            }
            else if (halfone == "dc") {
                if (halftwo != "u")
                    toadd.currentDutyCycle = halftwo.toInt();
            }
            else if (halfone == "maxdc") {
                if (halftwo != "u")
                    toadd.maxDutyCycle = halftwo.toInt();
            }
	    else if (halfone == "inout") {
		if (halftwo == "i")
		    toadd.inputPin = true;
		else
		    toadd.inputPin = false;
	    }
            else if (halfone == "freq")
                qDebug() << "ignore frequency";
            else
                qDebug() << "unknown parameter " << halfone << ", continuing anyway, though";

        }
        qDebug() << "\ni'll be adding this now:";
        toadd.debugdump();
        qDebug() << "\n";
        deviceVector.push_back(toadd);
    }
    for (int k = 0; k < deviceVector.size(); k++) {
        qDebug() << "";
        qDebug() << "element " << k;
        deviceVector.at(k).debugdump();
    }
    qDebug() << "finished reloading deviceVector";
}

void MicrocontrollerWindow::updateSerial() {
    QString serial = sockBaseDir + "/SERIAL";
    qDebug() << "serial path might be " << serial;
    if (QFile::exists(serial) == false) {
        qDebug() << "nope, that's not it";
        this->ui->serialEdit->setText("Connection lost");
        return;
    }
    qDebug() << "yes it is, open QFile then";
    QFile* reader = new QFile(serial);
    if (reader->open(QIODevice::Text | QIODevice::ReadOnly) == false) {
        qDebug() << "access is denied";
        reader->close();
        delete reader;
        this->ui->serialEdit->setText("Connection lost");
        return;
    }
    QString serialOut = QString(reader->readAll());
    reader->close();
    delete reader;
    qDebug() << "cool, got the data, now display it on the screen";
    this->ui->serialEdit->setText(serialOut);
    this->ui->serialEdit->verticalScrollBar()->setValue(this->ui->serialEdit->verticalScrollBar()->maximum());
}

void MicrocontrollerWindow::updateGUI() {
    //qDebug() << "dummy function, does not work yet";
    qDebug() << "update the basic device info first";
    for (int i = 0; i < deviceVector.size(); i++) {
        onOffPWM(i, deviceVector.at(i).pwmCapable, false);
        dcUpdateBlocker = true;
        if (i == 0) {
            this->ui->pin0_dval->setText("Digital value: " + QString::number(deviceVector.at(i).currentDigitalValue));
            this->ui->pin0_analogdcval->setText("Current DC: " + QString::number(deviceVector.at(i).currentDutyCycle));
            this->ui->pin0_dcBox->setValue(deviceVector.at(i).maxDutyCycle);
        }
        else if (i == 1) {
            this->ui->pin1_dval->setText("Digital value: " + QString::number(deviceVector.at(i).currentDigitalValue));
            this->ui->pin1_analogdcval->setText("Current DC: " + QString::number(deviceVector.at(i).currentDutyCycle));
            this->ui->pin1_dcBox->setValue(deviceVector.at(i).maxDutyCycle);
        }
        else if (i == 2) {
            this->ui->pin3_analogdcval->setText("Current DC: " + QString::number(deviceVector.at(i).currentDutyCycle));
            this->ui->pin3_dcBox->setValue(deviceVector.at(i).maxDutyCycle);
        }
        else if (i == 3) {
            this->ui->pin4_analogdcval->setText("Current DC: " + QString::number(deviceVector.at(i).currentDutyCycle));
            this->ui->pin4_dcBox->setValue(deviceVector.at(i).maxDutyCycle);
        }
        dcUpdateBlocker = false;
    }
    for (int j = 0; j < deviceConnected.size(); j++) {
        if (deviceConnected.at(j) == "U")
            qDebug() << "skip " << j;
        else {
#ifndef MULEMICROSIM_ENABLEPICTURES
            QString toSet = "Internal error";
            if (deviceConnected.at(j) == "led") {
                if (deviceVector.at(j).currentDigitalValue == 0)
                    toSet = "Lamp is off";
                else
                    toSet = "Lamp is on";
            }
            else if (deviceConnected.at(j) == "servo") {
                toSet = "Angle: ^A^";
                if (deviceVector.at(j).currentDutyCycle > 0)
                    toSet.replace("^A^", QString::number((int)(std::floor(deviceVector.at(j).currentDutyCycle / 1.41))));
                else
                    toSet.replace("^A^", "off");

            }
            else if (deviceConnected.at(j) == "photoresistor") {
                toSet = "PR not triggered";
                int trval = 1;
                if (deviceVector.at(j).pullUpDown == 2)
                    trval = 0;
                if (deviceVector.at(j).currentDigitalValue == trval)
                    toSet = "PR triggered";
            }
            if (j == 0)
                this->ui->pin0_cdevstatus->setText(toSet);
            else if (j == 1)
                this->ui->pin1_cdevstatus->setText(toSet);
            else if (j == 2)
                this->ui->pin3_cdevstatus->setText(toSet);
            else if (j == 3)
                this->ui->pin4_cdevstatus->setText(toSet);
#else
            QString resToSet = "";
            if (deviceConnected.at(j) == "led") {
                if (deviceVector.at(j).currentDigitalValue == 0)
                    resToSet = ":/visualresources/resources/lightoff.png";
                else
                    resToSet = ":/visualresources/resources/lighton.png";
            }
            else if (deviceConnected.at(j) == "servo") {
                int ranpic = qrand() % 4;
                if (ranpic < 1)
                    ranpic = 1;
                resToSet = ":/visualresources/resources/servoon" + QString::number(ranpic) + ".png";
                if (deviceVector.at(j).currentDutyCycle == 0)
                    resToSet = ":/visualresources/resources/servooff.png";
            }
            else if (deviceConnected.at(j) == "photoresistor")
                resToSet = ":/visualresources/resources/photoresistor.png";
            if (j == 0)
                this->ui->pin0_cdevstatus->setPixmap(QPixmap(resToSet).scaled(this->ui->pin0_cdevstatus->size().width(), this->ui->pin0_cdevstatus->size().height(), Qt::KeepAspectRatio));
            else if (j == 1)
                this->ui->pin1_cdevstatus->setPixmap(QPixmap(resToSet).scaled(this->ui->pin1_cdevstatus->size().width(), this->ui->pin1_cdevstatus->size().height(), Qt::KeepAspectRatio));
            else if (j == 2)
                this->ui->pin3_cdevstatus->setPixmap(QPixmap(resToSet).scaled(this->ui->pin3_cdevstatus->size().width(), this->ui->pin3_cdevstatus->size().height(), Qt::KeepAspectRatio));
            else if (j == 3)
                this->ui->pin4_cdevstatus->setPixmap(QPixmap(resToSet).scaled(this->ui->pin4_cdevstatus->size().width(), this->ui->pin4_cdevstatus->size().height(), Qt::KeepAspectRatio));
#endif
        }
    }
}

void MicrocontrollerWindow::onGlobalTimerTimeout() {
    if (timerUpdateBlocker == true)
        return;
    reloadDeviceVector();
    updateGUI();
    updateSerial();
}

MicrocontrollerWindow::~MicrocontrollerWindow()
{
    //delete deviceConnected;
    delete ui;
}

void MicrocontrollerWindow::on_clearButton_clicked()
{
    clearSerial();
}

void MicrocontrollerWindow::onMaxDutyCycleChanged(int pin, int dc) {
    if (dcUpdateBlocker == true)
            return;
    timerUpdateBlocker = true;
    deviceVector.at(pin).maxDutyCycle = dc;
    flushPin(deviceVector.at(pin));
    timerUpdateBlocker = false;
}

void MicrocontrollerWindow::onOffPWM(int pin, bool val, bool resave) {
    timerUpdateBlocker = true;
    deviceVector.at(pin).pwmCapable = val;
    if (pin == 0) {
        this->ui->pin0_pwmbox->setChecked(val);
        this->ui->pin0_dcBox->setEnabled(val);
        this->ui->pin0_analogdcval->setVisible(val);
    }
    else if (pin == 1) {
        this->ui->pin1_pwmBox->setChecked(val);
        this->ui->pin1_dcBox->setEnabled(val);
        this->ui->pin1_analogdcval->setVisible(val);
    }
    else if (pin == 2) {
        this->ui->pin3_pwmbox->setChecked(val);
        this->ui->pin3_dcBox->setEnabled(val);
        this->ui->pin3_analogdcval->setVisible(val);
    }
    else if (pin == 3) {
        this->ui->pin4_pwmBox->setChecked(val);
        this->ui->pin4_dcBox->setEnabled(val);
        this->ui->pin4_analogdcval->setVisible(val);
    }
    if (resave == true)
        flushPin(deviceVector.at(pin));
    timerUpdateBlocker = false;
}

void MicrocontrollerWindow::clearSerial() {
    QString serial = sockBaseDir + "/SERIAL";
    qDebug() << "serial path might be " << serial;
    if (QFile::exists(serial) == false) {
        qDebug() << "nope, that's not it";
        this->ui->serialEdit->setText("Connection lost");
        return;
    }
    qDebug() << "yes it is, open QFile then";
    QFile* writer = new QFile(serial);
    if (writer->open(QIODevice::Text | QIODevice::WriteOnly) == false) {
        qDebug() << "access is denied";
        writer->close();
        delete writer;
        this->ui->serialEdit->setText("Connection lost");
        return;
    }
    writer->write(QString("").toUtf8());
    writer->close();
    delete writer;
    updateSerial();
}

void MicrocontrollerWindow::on_pin0_pwmbox_clicked()
{
    onOffPWM(0, this->ui->pin0_pwmbox->isChecked());
}

void MicrocontrollerWindow::on_pin1_pwmBox_clicked()
{
    onOffPWM(1, this->ui->pin1_pwmBox->isChecked());
}

void MicrocontrollerWindow::on_pin3_pwmbox_clicked()
{
    onOffPWM(2, this->ui->pin3_pwmbox->isChecked());
}

void MicrocontrollerWindow::on_pin4_pwmBox_clicked()
{
    onOffPWM(3, this->ui->pin4_pwmBox->isChecked());
}

void MicrocontrollerWindow::on_pin0_dcBox_valueChanged(int arg1)
{
    onMaxDutyCycleChanged(0, arg1);
}

void MicrocontrollerWindow::on_pin1_dcBox_valueChanged(int arg1)
{
    onMaxDutyCycleChanged(1, arg1);
}

void MicrocontrollerWindow::on_pin3_dcBox_valueChanged(int arg1)
{
    onMaxDutyCycleChanged(2, arg1);
}

void MicrocontrollerWindow::on_pin4_dcBox_valueChanged(int arg1)
{
    onMaxDutyCycleChanged(3, arg1);
}

void MicrocontrollerWindow::on_pin0_button_clicked()
{
    connectDisconnectPin(0);
}

void MicrocontrollerWindow::connectDisconnectPin(int pin) {
    timerUpdateBlocker = true;
    if (deviceConnected.at(pin) == "U") {
        QStringList possibleDeviceList;
        possibleDeviceList.push_back("LED");
        if (deviceVector.at(pin).pwmCapable == true)
            possibleDeviceList.push_back("Servo");
        possibleDeviceList.push_back("Photoresistor");
        if (deviceVector.at(pin).digital == false) {
            possibleDeviceList.removeAt(0);
            possibleDeviceList.removeAt(1);
        }
        qDebug() << "these devices can be connected:";
        qDebug() << possibleDeviceList;
        QString item = QInputDialog::getItem(this, "Mule Microcontroller Simulator", "What would you like to connect?", possibleDeviceList, 0, false);
        if (item.isEmpty()) {
            qDebug() << "user cancelled the whole thing";
            timerUpdateBlocker = false;
            return;
        }
        qDebug() << "user chose " << item;
        deviceConnected.at(pin) = item.toLower();
        if (pin == 0)
            this->ui->pin0_button->setText("Disconnect " + item);
        else if (pin == 1)
            this->ui->pin1_button->setText("Disconnect " + item);
        else if (pin == 2)
            this->ui->pin2_button->setText("Disconnect " + item);
        else if (pin == 3)
            this->ui->pin3_button->setText("Disconnect " + item);
    }
    else {
        deviceConnected.at(pin) = "U";
        if (pin == 0)
            this->ui->pin0_button->setText("Connect device");
        else if (pin == 1)
            this->ui->pin1_button->setText("Connect device");
        else if (pin == 2)
            this->ui->pin2_button->setText("Connect device");
        else if (pin == 3)
            this->ui->pin3_button->setText("Connect device");
        //qDebug() << "not implemented";
    }
    timerUpdateBlocker = false;
}

void MicrocontrollerWindow::on_pin1_button_clicked()
{
    connectDisconnectPin(1);
}

void MicrocontrollerWindow::on_pin2_button_clicked()
{
    connectDisconnectPin(2);
}

void MicrocontrollerWindow::on_pin3_button_clicked()
{
    connectDisconnectPin(3);
}

void MicrocontrollerWindow::on_actionAbout_Qt_framework_triggered()
{
    QApplication::aboutQt();
}


void MicrocontrollerWindow::manualValuePin(int pin) {
    timerUpdateBlocker = true;
    int vl = QInputDialog::getInt(this, "Mule Microcontroller Simulator", "Which value (high or low, default: low)?", 0, 0, 1, 1);
    if (vl < 0 && vl > 1)
        vl = 0;
    deviceVector.at(pin).currentDigitalValue = vl;
    flushPin(deviceVector.at(pin));
    timerUpdateBlocker = false;
}

void MicrocontrollerWindow::safeQuit() {
    timerUpdateBlocker = true;
    dcUpdateBlocker = true;
    QString rdirCmd = "rm -r -f \"" + sockBaseDir + "\"";
#ifdef Q_OS_WIN
    rdirCmd = "rd /q \"" + sockBaseDir + "\"";
#endif
    std::system(rdirCmd.toStdString().c_str());
}

void MicrocontrollerWindow::closeEvent(QCloseEvent *ev) {
    safeQuit();
    ev->accept();
}

void MicrocontrollerWindow::on_actionQuit_triggered()
{
    this->close();
}

void MicrocontrollerWindow::on_actionAbout_Mule_Microcontroller_Simulator_triggered()
{
    QMessageBox::information(this, "Mule Microcontroller Simulator", "Version " + QString::number(TOOLMV) + "." + QString::number(TOOLIV) + "." + QString::number(TOOLUV));
}

void MicrocontrollerWindow::on_pin0_changedv_clicked()
{
    manualValuePin(0);
}

void MicrocontrollerWindow::on_pin1_changedv_clicked()
{
    manualValuePin(1);
}
