#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

//#include "ThumbnailListWidgetInterface.h"

#include <mitkStandaloneDataStorage.h>
#include <mitkImage.h>

#include <QMainWindow>
#include <QtSvg>

class AppDataManager;
class DataManagerView;
class StatisticsWindow;
class SegmentationWindow;
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

	StatisticsWindow *m_StatisticsWindow;
	SegmentationWindow *m_SegmentationWindow;
	VolumeVisualizationWindow *m_VolumeVisualizationWindow;
	MovieMakerWindow *m_MovieMakerWindow;
};
#endif // MAIN_WINDOW_H
