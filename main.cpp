#include "MainWindow.h"

#include "framelesswindow.h"
#include "DarkStyle.h"

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
	framelessWindow.setWindowTitle("test title");
//	framelessWindow.setWindowIcon(a.style()->standardIcon(QStyle::SP_DesktopIcon));

	MainWindow *mainWindow = new MainWindow;
	framelessWindow.setContent(mainWindow);
	framelessWindow.show();
//	mainWindow->show();
	return a.exec();
}
