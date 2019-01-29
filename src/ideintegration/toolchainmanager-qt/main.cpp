#include <QApplication>
#include <settingsdialog.h>

int main(int argc, char** argv) {
	QApplication qtApp(argc, argv);
	SettingsDialog dlg;
	QStringList args = qtApp.arguments();
	args.removeAt(0);
	if (args.contains("-tcselect") == true)
		return dlg.selectToolchains(args.contains("-cli"), args);
	else
		dlg.exec();
	return qtApp.exec();
}
