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

#ifndef MICROCONTROLLERWINDOW_H
#define MICROCONTROLLERWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QMessageBox>
#include <QtDebug>
#include <QTimer>
#include <QFile>
#include <QInputDialog>
#include <QProcess>
#include <QDateTime>
#include <QScrollBar>
#include <QCloseEvent>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <stdlib.h>

#define MULEMICROSIM_DIGITALPINFREQUENCY 50
#define MULEMICROSIM_ANALOGPINFREQUENCY 50
#define MULEMICROSIM_UPDATEINTERVAL 150
#ifdef __sun
#define nullptr 0
#endif

struct MuleMicrocontrollerSimulatorPin {
    int num;
    bool digital;
    bool pwmCapable;
    int pullUpDown;
    int currentDigitalValue;
    int currentDutyCycle;
    int maxDutyCycle;
    bool inputPin;

    void debugdump() {
        qDebug() << "num = " << num;
        qDebug() << "digital = " << digital;
        qDebug() << "pwmCapable = " << pwmCapable;
        qDebug() << "pullUpDown = " << pullUpDown;
        qDebug() << "currentDigitalValue = " << currentDigitalValue;
        qDebug() << "currentDutyCycle = " << currentDutyCycle;
        qDebug() << "maxDutyCycle = " << maxDutyCycle;
    }
};

namespace Ui {
class MicrocontrollerWindow;
}

class MicrocontrollerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MicrocontrollerWindow(QWidget *parent = nullptr);
    ~MicrocontrollerWindow();
    void initializeSocket();
    void flushPin(const MuleMicrocontrollerSimulatorPin& pin);
    void reloadDeviceVector();
    void updateSerial();
    void updateGUI();
    void clearSerial();
    void onOffPWM(int pin, bool val, bool resave = true);
    void manualValuePin(int pin);
    void onMaxDutyCycleChanged(int pin, int dc);
    void connectDisconnectPin(int pin);
    void safeQuit();
    void closeEvent(QCloseEvent* ev);
#ifdef __sun
    int sunsystem(const char* in) {
	extern "C" {
		return system(in);
	}
    }
#endif


public slots:
    void onGlobalTimerTimeout();

private slots:
    void on_clearButton_clicked();

    void on_pin0_pwmbox_clicked();

    void on_pin1_pwmBox_clicked();

    void on_pin3_pwmbox_clicked();

    void on_pin4_pwmBox_clicked();

    void on_pin0_dcBox_valueChanged(int arg1);

    void on_pin1_dcBox_valueChanged(int arg1);

    void on_pin3_dcBox_valueChanged(int arg1);

    void on_pin4_dcBox_valueChanged(int arg1);

    void on_pin0_button_clicked();

    void on_pin1_button_clicked();

    void on_pin2_button_clicked();

    void on_pin3_button_clicked();

    void on_actionAbout_Qt_framework_triggered();

    void on_actionQuit_triggered();

    void on_actionAbout_Mule_Microcontroller_Simulator_triggered();

    void on_pin0_changedv_clicked();

    void on_pin1_changedv_clicked();

private:
    Ui::MicrocontrollerWindow *ui;
    QTimer* globalTimer;
    QString sockBaseDir;
    std::vector<MuleMicrocontrollerSimulatorPin> deviceVector;
    std::vector<QString> deviceConnected;
    bool timerUpdateBlocker;
    bool dcUpdateBlocker;
};

#endif // MICROCONTROLLERWINDOW_H
