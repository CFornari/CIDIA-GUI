#include "VolumeVisualizationWindow.h"
#include "ui_VolumeVisualizationWindow.h"

#include "AppDataManager.h"
#include "Utils.h"
#include "VolumeVisualizationView.h"
#include <Qmitk3DRenderWidget.h>

//#include <mitkTransferFunctionPropertySerializer.h>

#include <mitkProperties.h>
#include <mitkRenderingManager.h>
#include <mitkTransferFunction.h>
#include <mitkTransferFunctionProperty.h>
#include <mitkDataInteractor.h>

#include <mitkStandaloneDataStorage.h>
#include <mitkDataStorage.h>
#include <mitkIOUtil.h>

VolumeVisualizationWindow::VolumeVisualizationWindow(QWidget *parent)
	:	QWidget(parent),
		ui(new Ui::VolumeVisualizationWindow)
{
	ui->setupUi(this);

	m_AppData = AppDataManager::GetInstance();
	m_AppData->createDataStorageFromMaster("VolumeVisualization");
	auto dataStorage = m_AppData->getDataStorageByName("VolumeVisualization");

	view3D = new Qmitk3DRenderWidget(this, QString("3D_VolumeVisualization"));
	view3D->SetDataStorage(dataStorage);

	volumeVisualizationView = new VolumeVisualizationView(this);

	ui->viewLayout->addWidget(view3D);
	ui->controlsLayout->addWidget(volumeVisualizationView);

	ui->horizontalLayout->setMargin(0);

	connect(m_AppData, &AppDataManager::newDataLoadedEnd, this, &VolumeVisualizationWindow::onNewDataLoadedEnd);
}

VolumeVisualizationWindow::~VolumeVisualizationWindow()
{
	delete ui;
}

void VolumeVisualizationWindow::onNewDataLoadedEnd()
{
	for( auto node : m_AppData->getFileNodeList())
	{
		volumeVisualizationView->SetDataNode(node);
	}

	view3D->ResetView();
}

void VolumeVisualizationWindow::showEvent(QShowEvent* e)
{
	Q_UNUSED(e)
}



