#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "AppDataManager.h"
#include "DataManagerView.h"
#include "Concept1.h"
#include "Concept2.h"
#include "Concept3.h"
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
//		m_FirstImage(mitk::Image::New())
{
	ui->setupUi(this);

	m_AppData = AppDataManager::GetInstance();

	initDataManagerView();
	initShadows();
	initViews();

	// Needs to be the last one call from all init methods.
	initConnections();

	showConcept1();
	emit viewConcept1VisibilityChanged(false);
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
	m_Concept1 = new Concept1(ui->windowView);
	ui->windowViewLayout->addWidget(m_Concept1);

	m_Concept2 = new Concept2(ui->windowView);
	ui->windowViewLayout->addWidget(m_Concept2);

	m_Concept3 = new Concept3(ui->windowView);
	ui->windowViewLayout->addWidget(m_Concept3);
}

void MainWindow::initConnections()
{
	connect(m_AppData, &AppDataManager::newDataLoadedEnd, m_Concept3, &Concept3::onDataLoaded);
	connect(this, &MainWindow::viewConcept1VisibilityChanged, m_Concept1, &Concept1::onVisibilityChanged);
	connect(this, &MainWindow::viewConcept2VisibilityChanged, m_Concept2, &Concept2::onVisibilityChanged);
	connect(this, &MainWindow::viewConcept3VisibilityChanged, m_Concept3, &Concept3::onVisibilityChanged);
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
	emit viewConcept1VisibilityChanged(true);
	emit viewConcept2VisibilityChanged(false);
	emit viewConcept3VisibilityChanged(false);

//	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void MainWindow::showConcept2()
{
	emit viewConcept1VisibilityChanged(false);
	emit viewConcept2VisibilityChanged(true);
	emit viewConcept3VisibilityChanged(false);

//	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void MainWindow::showConcept3()
{
	emit viewConcept1VisibilityChanged(false);
	emit viewConcept2VisibilityChanged(false);
	emit viewConcept3VisibilityChanged(true);

//	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void MainWindow::openFileDialog()
{
		const QString directory =
				QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).value(0, QDir::homePath());
		QFileDialog dialog(this, tr("Open Image"), directory);
		dialog.setAcceptMode(QFileDialog::AcceptOpen);
		dialog.setFileMode(QFileDialog::ExistingFile);

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
	animation->setDuration(1000);
	animation->setStartValue(m_DataManagerHidePosition);
	animation->setEndValue(m_DataManagerShowPosition);
	animation->start();
}

void MainWindow::hideDataManager()
{
	//	Create animation
	QPropertyAnimation *animation = new QPropertyAnimation(m_DataManagerView, "geometry");
	animation->setDuration(1000);
	animation->setStartValue(m_DataManagerShowPosition);
	animation->setEndValue(m_DataManagerHidePosition);
	animation->start();
}



