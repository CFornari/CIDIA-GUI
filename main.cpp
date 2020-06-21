#include "MainWindow.h"

#include "DarkStyle.h"

#include <QmitkRegisterClasses.h>

#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	a.setStyle(new DarkStyle);

	// Register Qmitk-dependent global instances
	QmitkRegisterClasses();

	MainWindow *mainWindow = new MainWindow;
	mainWindow->showFullScreen();
	return a.exec();
}
