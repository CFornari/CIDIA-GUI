#include "MainWindow.h"

#include <QApplication>

#include <QmitkRegisterClasses.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QCoreApplication::setOrganizationName("UFRGS");
	QCoreApplication::setApplicationName("CIDIA-GUI");

	// Register Qmitk-dependent global instances
	QmitkRegisterClasses();

	MainWindow w;
	w.showFullScreen();
	return a.exec();
}
