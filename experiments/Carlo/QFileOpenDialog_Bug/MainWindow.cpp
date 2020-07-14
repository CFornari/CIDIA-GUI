#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include <QGuiApplication>
#include <QScreen>

#include <QPropertyAnimation>
#include <QDir>
#include <QStandardPaths>
#include <QFileDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>

#include <iostream>


namespace Widgets
{
	void MoveCenter(QWidget* widget)
	{
		QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
		int x = (screenGeometry.width() - widget->width()) / 2;
		int y = (screenGeometry.height() - widget->height()) / 2;
		widget->move(x, y);
	}

	void MoveTop(QWidget* widget)
	{
		QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
		int x = widget->geometry().x();
		int y = screenGeometry.top();
		widget->move(x, y);
	}

	void MoveBaseTop(QWidget* widget)
	{
		QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
		int x = widget->geometry().x();
		int y = screenGeometry.top() - widget->geometry().height();
		y = y - 5; // This helps hides the border.
		widget->move(x, y);
	}
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	initDataManagerView();
	addBorderShadowGloomEffect(m_DataManagerView);
	addBorderShadowGloomEffect(ui->pushButton);

}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::initDataManagerView()
{
	m_DataManagerView = new QWidget(this);
	m_DataManagerView->setGeometry(QRect(0,0,200,200));
	Widgets::MoveCenter(m_DataManagerView);
	Widgets::MoveTop(m_DataManagerView);
	m_DataManagerShowPosition = m_DataManagerView->geometry();
	Widgets::MoveBaseTop(m_DataManagerView);
	m_DataManagerHidePosition = m_DataManagerView->geometry();

	m_DataManagerView->setStyleSheet("background-color: black;");
}

void MainWindow::addBorderShadowGloomEffect(QWidget* widget)
{
	QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
	shadow->setBlurRadius(8.0);
	shadow->setColor(Qt::green);
	shadow->setOffset(5);
	widget->setGraphicsEffect(shadow);
}


void MainWindow::openFileDialog()
{
		const QString directory =
				QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).value(0, QDir::homePath());
		QFileDialog dialog(this, tr("Open Image"), directory);
		dialog.setAcceptMode(QFileDialog::AcceptOpen);
		dialog.setFileMode(QFileDialog::ExistingFile);

				Widgets::MoveCenter(&dialog);
//		dialog.raise();
//		dialog.show();
		dialog.exec();
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
	// Create animation
	QPropertyAnimation *animation = new QPropertyAnimation(m_DataManagerView, "geometry");
	animation->setDuration(1000);
	animation->setStartValue(m_DataManagerHidePosition);
	animation->setEndValue(m_DataManagerShowPosition);
	animation->start();
}

void MainWindow::hideDataManager()
{
	// Create animation
	QPropertyAnimation *animation = new QPropertyAnimation(m_DataManagerView, "geometry");
	animation->setDuration(1000);
	animation->setStartValue(m_DataManagerShowPosition);
	animation->setEndValue(m_DataManagerHidePosition);
	animation->start();
}


