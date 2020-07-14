#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include "ThumbnailListWidgetInterface.h"

#include <mitkStandaloneDataStorage.h>
#include <mitkImage.h>

#include <QMainWindow>
#include <QtSvg>

class AppDataManager;
class DataManagerView;
class StatisticsWindow;
class Concept2;
class VolumeVisualizationWindow;
class MovieMakerWindow;

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
	void showConcept4();
	void openFileDialog();

	void onDataManagerButtonToggled(bool state);
	void showDataManager();
	void hideDataManager();

protected:
	void initDataManagerView();
	void initShadows();
	void initViews();

private:
	void addBorderShadowGloomEffect(QWidget *widget);
	virtual void showEvent(QShowEvent *e) override;

	Ui::MainWindow *ui;

	AppDataManager *m_AppData;

	DataManagerView *m_DataManagerView;
	QRect m_DataManagerShowPosition;
	QRect m_DataManagerHidePosition;

	StatisticsWindow *m_Concept1;
	Concept2 *m_Concept2;
	VolumeVisualizationWindow *m_Concept3;
	MovieMakerWindow *m_Concept4;
};
#endif // MAINWINDOW_H
