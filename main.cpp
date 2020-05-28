#include "MainWindow.h"
#include "Concept.h"

#include "DarkStyle.h"
#include "framelesswindow.h"

#include <QmitkRegisterClasses.h>
//#include <itksys/SystemTools.hxx>

#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

//	QFile f(":/qdarkstyle/style.qss");
//	QFile f(":/dark.qss");
//	QFile f(":/darkstyle.qss");

//	if (!f.exists())   {
//			printf("Unable to set stylesheet, file not found\n");
//	}
//	else   {
//			f.open(QFile::ReadOnly | QFile::Text);
//			QTextStream ts(&f);
//			qApp->setStyleSheet(ts.readAll());
//	}

	a.setStyle(new DarkStyle);

	// Register Qmitk-dependent global instances
	QmitkRegisterClasses();

	FramelessWindow framelessWindow;
	framelessWindow.setWindowTitle("covid-gui");
//	framelessWindow.setWindowIcon(a.style()->standardIcon(QStyle::SP_DesktopIcon));

//	MainWindow *mainWindow = new MainWindow;
//	framelessWindow.setContent(mainWindow);

	Concept *conceptWindow = new Concept;
	framelessWindow.setContent(conceptWindow);

	framelessWindow.show();
//	mainWindow->show();
	return a.exec();
}
