#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include "ThumbnailListWidgetInterface.h"

#include <mitkStandaloneDataStorage.h>
#include <mitkImage.h>

#include <QMainWindow>

struct AppDataStorage;
class Concept1;
class Concept2;
class Concept3;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

public slots:
	void showConcept1();
	void showConcept2();
	void showConcept3();
	void openFileDialog();

signals:
	void dataLoaded(AppDataStorage *appData);

private:
	void setupLayout();
	void setupMenus();

	void loadImage(const QString filePath);

private:
	mitk::Image::Pointer m_FirstImage;

//	ThumbnailListWidgetInterface *m_ThumbnailListWidget;

	Ui::MainWindow *ui;

	AppDataStorage *m_AppData;
	Concept1 *m_Concept1;
	Concept2 *m_Concept2;
	Concept3 *m_Concept3;
};
#endif // MAINWINDOW_H
