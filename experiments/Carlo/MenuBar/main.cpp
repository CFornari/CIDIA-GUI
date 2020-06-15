#include "MainWindow.h"

#include <SpaceXStyle.h>

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	a.setStyle(new SpaceXStyle);

	MainWindow w;
	w.showFullScreen();
	return a.exec();
}
