# Mule Microcontroller Simulator - Arduino-like microcontroller simulator for debugging and testing purposes
# Copyright (C) 2018 Tim K <timprogrammer@rambler.ru>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

QT       += core gui
TOOLMAJORVERSION = 0
TOOLMINORVERSION = 4
TOOLUPDATEVERSION = 1

DEFINES += TOOLMV=$$TOOLMAJORVERSION
DEFINES += TOOLIV=$$TOOLMINORVERSION
DEFINES += TOOLUV=$$TOOLUPDATEVERSION
DEFINES += MULEMICROSIM_ENABLEPICTURES
DEFINES += QT_NO_DEBUG_OUTPUT

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mulemicrosim
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        microcontrollerwindow.cpp

HEADERS += \
        microcontrollerwindow.h

FORMS += \
        microcontrollerwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
