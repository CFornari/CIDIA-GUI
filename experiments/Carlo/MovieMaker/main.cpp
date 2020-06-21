#include "MainWindow.h"

#include "QmitkRegisterClasses.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	// Register Qmitk-dependent global instances
	QmitkRegisterClasses();

	MainWindow w;
	w.showFullScreen();
	return a.exec();
}
