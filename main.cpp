#include "MainWindow.h"

#include "SpaceXStyle.h"

#include <QmitkRegisterClasses.h>

#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	a.setStyle(new SpaceXStyle);

	// Register Qmitk-dependent global instances
	QmitkRegisterClasses();

	MainWindow *mainWindow = new MainWindow;
	mainWindow->showFullScreen();
	return a.exec();
}
