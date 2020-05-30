#include "MainWindow.h"

#include "QmitkRegisterClasses.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	// Register Qmitk-dependent global instances
	QmitkRegisterClasses();

	MainWindow w;
	w.show();
	return a.exec();
}
