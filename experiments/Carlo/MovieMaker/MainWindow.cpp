#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include <QmitkMovieMakerView.h>

#include <QmitkStdMultiWidget.h>
#include <QmitkRenderingManager.h>
#include <QmitkRenderWindow.h>
#include <QmitkRenderWindowWidget.h>
#include <QmitkIOUtil.h>

#include <mitkProperties.h>
#include <mitkRenderingManager.h>
#include <mitkTransferFunction.h>
#include <mitkTransferFunctionProperty.h>
#include <mitkDataInteractor.h>
#include <mitkTransferFunctionPropertySerializer.h>

#include <mitkStandaloneDataStorage.h>
#include <mitkDataStorage.h>
#include <mitkIOUtil.h>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	mitk::StandaloneDataStorage::Pointer ds1 = mitk::StandaloneDataStorage::New();

	mitk::StandaloneDataStorage::SetOfObjects::Pointer nodes1 = mitk::StandaloneDataStorage::SetOfObjects::New();

	try
	{
		nodes1 = mitk::IOUtil::Load(QString("/media/storage/carlo/Documents/mitk-data/Pic3D.nrrd").toStdString(), *ds1);
//		nodes1 = mitk::IOUtil::Load(QString("/usr/mitk-development/covid-gui/test-data/Pic3D.nrrd").toStdString(), *ds1);
//		mitk::IOUtil::Load(QString("/usr/mitk-development/covid-gui/test-data/lungs.vtk").toStdString(), *ds2);
	}
	catch (const mitk::Exception& e)
	{
		MITK_INFO << e;
		return;
	}

	m_MultiWidget = new QmitkStdMultiWidget();
	m_MultiWidget->SetDataStorage(ds1);
	m_MultiWidget->InitializeMultiWidget();
	m_MultiWidget->ResetCrosshair();
	ui->MultiWidgetLayout->addWidget(m_MultiWidget);

	m_View = new QmitkMovieMakerView(this);
	ui->horizontalLayout_2->addWidget(m_View);
}

MainWindow::~MainWindow()
{
	delete ui;
}

