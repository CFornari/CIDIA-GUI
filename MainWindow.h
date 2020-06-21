#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include "ThumbnailListWidgetInterface.h"

#include <mitkStandaloneDataStorage.h>
#include <mitkImage.h>

#include <QMainWindow>
#include <QtSvg>

class AppDataManager;
class DataManagerView;
class Concept1;
class Concept2;
class Concept3;

class QRect;

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

	void onDataManagerButtonToggled(bool state);
	void showDataManager();
	void hideDataManager();

signals:
	void viewConcept1VisibilityChanged(bool visible);
	void viewConcept2VisibilityChanged(bool visible);
	void viewConcept3VisibilityChanged(bool visible);

protected:
	void initDataManagerView();
	void initShadows();
	void initViews();
	void initConnections();

private:
//	mitk::Image::Pointer m_FirstImage;
	void addBorderShadowGloomEffect(QWidget *widget);

	Ui::MainWindow *ui;

	AppDataManager *m_AppData;

	DataManagerView *m_DataManagerView;
	QRect m_DataManagerShowPosition;
	QRect m_DataManagerHidePosition;

	Concept1 *m_Concept1;
	Concept2 *m_Concept2;
	Concept3 *m_Concept3;
};
#endif // MAINWINDOW_H
