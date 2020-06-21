#include "Concept3.h"
#include "ui_Concept3.h"

#include "AppDataManager.h"
#include "Utils.h"
#include "VolumeVisualizationView.h"

#include <mitkTransferFunctionPropertySerializer.h>

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

#include <mitkStandaloneDataStorage.h>
#include <mitkDataStorage.h>
#include <mitkIOUtil.h>

Concept3::Concept3(QWidget *parent)
	:	QWidget(parent),
		ui(new Ui::Concept3)
{
	ui->setupUi(this);

	m_AppData = AppDataManager::GetInstance();

	renderWidget = new QmitkRenderWindowWidget(this, QString("3D_transferFunction"), m_AppData->getDataStorage());
	renderWidget->GetRenderWindow()->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);
	renderWidget->GetRenderWindow()->GetRenderer()->GetVtkRenderer()->ResetCamera();
	renderWidget->setStyleSheet("border: 0px");
	renderWidget->SetCornerAnnotationText("3D");

	volumeVisualizationView = new VolumeVisualizationView(this);

	ui->viewLayout->addWidget(renderWidget);
	ui->controlsLayout->addWidget(volumeVisualizationView);

	ui->horizontalLayout->setMargin(0);
}

Concept3::~Concept3()
{
	delete ui;
}

void Concept3::onDataLoaded()
{
	volumeVisualizationView->SetDataNode(m_AppData->getSetOfObjects()->at(0));
}

void Concept3::onVisibilityChanged(bool visible)
{
	this->setVisible(visible);
}



