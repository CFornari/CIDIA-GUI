#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include "w3D.h"
#include "wMulti.h"

#include <QmitkRenderingManager.h>
#include <QmitkRenderWindow.h>
#include <QmitkIOUtil.h>

#include <mitkStandaloneDataStorage.h>
#include <mitkIOUtil.h>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	mitk::StandaloneDataStorage::Pointer m_DataStorage = mitk::StandaloneDataStorage::New();
	mitk::StandaloneDataStorage::Pointer copy = mitk::StandaloneDataStorage::New();

	mitk::StandaloneDataStorage::SetOfObjects::Pointer dataNodes =mitk::StandaloneDataStorage::SetOfObjects::New();

	try
	{
		QString filePath("/usr/mitk-development/covid-gui/test-data/Pic3D.nrrd");
		dataNodes = mitk::IOUtil::Load(filePath.toStdString(), *m_DataStorage);
//		mitk::IOUtil::Load(filePath.toStdString(), *copy);
	}
	catch (const mitk::Exception& e)
	{
		MITK_INFO << e;
		return;
	}

	copy = m_DataStorage->Clone();
	ui->widget->setData(m_DataStorage, dataNodes);
	ui->widget_2->setData(copy);
//	QmitkRenderingManager::GetInstance()->InitializeViewsByBoundingObjects(m_DataStorage);
}

MainWindow::~MainWindow()
{
	delete ui;
}

