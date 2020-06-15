#include "Concept3.h"
#include "ui_Concept3.h"

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

Concept3::Concept3(QWidget *parent,
									 AppDataStorage *appData)
	:	QWidget(parent),
		m_AppData(appData),
		ui(new Ui::Concept3)
{
	ui->setupUi(this);

	renderWidget = new QmitkRenderWindowWidget(this, QString("3D_transferFunction"), m_AppData->storage);
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
	delete m_AppData;
	delete ui;
}

void Concept3::dataLoaded(AppDataStorage* appData)
{
	volumeVisualizationView->SetDataNode(appData->nodes->at(0));
}


