#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include "AppDataManager.h"
#include "DataManagerView.h"

#include <QmitkStdMultiWidget.h>
#include <QmitkRenderingManager.h>
#include <QmitkRenderWindow.h>
#include <QmitkIOUtil.h>

#include <QDir>
#include <QFileDialog>
#include <QStandardPaths>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
		ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_AppData = AppDataManager::GetInstance();

	m_DataManagerView = new DataManagerView(this);
	ui->treeViewLayout->addWidget(m_DataManagerView);

	m_MultiWidget = new QmitkStdMultiWidget(this);
	m_MultiWidget->SetDataStorage(m_AppData->getDataStorage());
	m_MultiWidget->InitializeMultiWidget();
	m_MultiWidget->ResetCrosshair();

	ui->hLayout->addWidget(m_MultiWidget);

	connect(m_AppData, &AppDataManager::newDataLoadedEnd, this, &MainWindow::onNewDataLoaded);
	connect(m_AppData, &AppDataManager::newDataLoadedStart, m_MultiWidget, &QmitkStdMultiWidget::RemovePlanesFromDataStorage);
	connect(m_AppData, &AppDataManager::newDataLoadedEnd, m_MultiWidget, &QmitkStdMultiWidget::AddPlanesToDataStorage);
}

MainWindow::~MainWindow()
{
	delete ui;
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

void MainWindow::onNewDataLoaded()
{
	std::cout << "HERE!" << endl;
}
