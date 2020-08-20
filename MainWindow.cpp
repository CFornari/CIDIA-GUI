#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "AppDataManager.h"
#include "DataManagerView.h"
#include "StatisticsWindow.h"
#include "SegmentationWindow.h"
#include "VolumeVisualizationWindow.h"
#include "MovieMakerWindow.h"
#include "Utils.h"

// Qmitk
#include <QmitkRenderingManager.h>
#include <QmitkRenderWindow.h>
#include <QmitkIOUtil.h>

// MITK
//#include <mitkDataStorage.h>

// Qt GUI
#include <QSplitter>
#include <QTabWidget>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QtWidgets>
#include <stdlib.h>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
		ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_AppData = AppDataManager::GetInstance();

	initDataManagerView();
	initShadows();
	initViews();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::initDataManagerView()
{
	m_DataManagerView = new DataManagerView(this);
	Widgets::MoveCenter(m_DataManagerView);
	Widgets::MoveTop(m_DataManagerView);
	m_DataManagerShowPosition = m_DataManagerView->geometry();
	Widgets::MoveBaseTop(m_DataManagerView);
	m_DataManagerHidePosition = m_DataManagerView->geometry();
}

void MainWindow::initShadows()
{
	addBorderShadowGloomEffect(ui->widget_3);
	addBorderShadowGloomEffect(m_DataManagerView);
}

void MainWindow::initViews()
{
	m_StatisticsWindow = new StatisticsWindow(ui->windowView);
	ui->windowViewLayout->addWidget(m_StatisticsWindow);

	m_SegmentationWindow = new SegmentationWindow(ui->windowView);
	ui->windowViewLayout->addWidget(m_SegmentationWindow);

	m_VolumeVisualizationWindow = new VolumeVisualizationWindow(ui->windowView);
	ui->windowViewLayout->addWidget(m_VolumeVisualizationWindow);

	m_MovieMakerWindow = new MovieMakerWindow(ui->windowView);
	ui->windowViewLayout->addWidget(m_MovieMakerWindow);
}

void MainWindow::addBorderShadowGloomEffect(QWidget* widget)
{
	QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
	shadow->setBlurRadius(8.0);
	shadow->setColor(palette().color(QPalette::Highlight));
	shadow->setOffset(0);
	widget->setGraphicsEffect(shadow);
}

void MainWindow::showConcept1()
{
	m_StatisticsWindow->show();
	m_SegmentationWindow->hide();
	m_VolumeVisualizationWindow->hide();
	m_MovieMakerWindow->hide();
}

void MainWindow::showConcept2()
{
	m_StatisticsWindow->hide();
	m_SegmentationWindow->show();
	m_VolumeVisualizationWindow->hide();
	m_MovieMakerWindow->hide();
}

void MainWindow::showConcept3()
{
	m_StatisticsWindow->hide();
	m_SegmentationWindow->hide();
	m_VolumeVisualizationWindow->show();
	m_MovieMakerWindow->hide();
}

void MainWindow::showConcept4()
{
	m_StatisticsWindow->hide();
	m_SegmentationWindow->hide();
	m_VolumeVisualizationWindow->hide();
	m_MovieMakerWindow->show();
}

void MainWindow::openFileDialog()
{
		const QString directory =
				QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).value(0, QDir::homePath());
		QFileDialog dialog(this, tr("Open Image"), directory);
		dialog.setAcceptMode(QFileDialog::AcceptOpen);
		dialog.setFileMode(QFileDialog::ExistingFile);

		Widgets::MoveCenter(&dialog);
		dialog.raise();
		dialog.show();

		if (dialog.exec() == QDialog::Accepted)
			m_AppData->loadFile(dialog.selectedFiles().constFirst());
}

void MainWindow::onDataManagerButtonToggled(bool state)
{
	if(state)
		showDataManager();
	else
		hideDataManager();
}

void MainWindow::showDataManager()
{
	//	Create animation
	QPropertyAnimation *animation = new QPropertyAnimation(m_DataManagerView, "geometry");
	animation->setDuration(500);
	animation->setStartValue(m_DataManagerHidePosition);
	animation->setEndValue(m_DataManagerShowPosition);
	animation->start();
}

void MainWindow::hideDataManager()
{
	//	Create animation
	QPropertyAnimation *animation = new QPropertyAnimation(m_DataManagerView, "geometry");
	animation->setDuration(500);
	animation->setStartValue(m_DataManagerShowPosition);
	animation->setEndValue(m_DataManagerHidePosition);
	animation->start();
}

void MainWindow::showEvent(QShowEvent *e)
{
	Q_UNUSED(e)

	showConcept1();
}
