QT += core widgets gui
TARGET = muletcm-qt
TEMPLATE = app

DEFINES += LIBMULESDKPREFIX=\\\"$$sdkprefix\\\"

SOURCES = main.cpp \
	  settingsdialog.cpp

HEADERS = settingsdialog.h

FORMS = settingsdialog.ui
