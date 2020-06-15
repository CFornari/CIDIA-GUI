#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include "w3D.h"
#include "wMulti.h"

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

static void setTrasnferFunction(mitk::StandaloneDataStorage::SetOfObjects::Pointer dataNodes)
{
	mitk::DataNode::Pointer node = dataNodes->at(0);

	mitk::Image::Pointer image = dynamic_cast<mitk::Image *>(node->GetData());
	if (image.IsNotNull())
	{
		// Set the property "volumerendering" to the Boolean value "true"
		node->SetProperty("volumerendering", mitk::BoolProperty::New(true));

//		// Create a transfer function to assign optical properties (color and opacity) to grey-values of the data
//		mitk::TransferFunction::Pointer tf = mitk::TransferFunction::New();
//		tf->InitializeByMitkImage(image);

//		// Set the color transfer function AddRGBPoint(double x, double r, double g, double b)
//		tf->GetColorTransferFunction()->AddRGBPoint(tf->GetColorTransferFunction()->GetRange()[0], 1.0, 0.0, 0.0);
//		tf->GetColorTransferFunction()->AddRGBPoint(tf->GetColorTransferFunction()->GetRange()[1], 1.0, 1.0, 0.0);

//		// Set the piecewise opacity transfer function AddPoint(double x, double y)
//		tf->GetScalarOpacityFunction()->AddPoint(0, 0);
//		tf->GetScalarOpacityFunction()->AddPoint(tf->GetColorTransferFunction()->GetRange()[1], 1);

		QString presetFileName("/media/storage/carlo/Documents/mitk-data/transfer.xml");
		mitk::TransferFunction::Pointer tf =
				mitk::TransferFunctionPropertySerializer::DeserializeTransferFunction(presetFileName.toLatin1());

		node->SetProperty("TransferFunction", mitk::TransferFunctionProperty::New(tf.GetPointer()));
	}
}


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	mitk::StandaloneDataStorage::Pointer ds1 = mitk::StandaloneDataStorage::New();
	mitk::StandaloneDataStorage::Pointer ds2 = mitk::StandaloneDataStorage::New();

	mitk::StandaloneDataStorage::SetOfObjects::Pointer nodes1 = mitk::StandaloneDataStorage::SetOfObjects::New();

	try
	{
		nodes1 = mitk::IOUtil::Load(QString("/media/storage/carlo/Documents/mitk-data/Lung.nrrd").toStdString(), *ds1);
//		nodes1 = mitk::IOUtil::Load(QString("/usr/mitk-development/covid-gui/test-data/Pic3D.nrrd").toStdString(), *ds1);
//		mitk::IOUtil::Load(QString("/usr/mitk-development/covid-gui/test-data/lungs.vtk").toStdString(), *ds2);
	}
	catch (const mitk::Exception& e)
	{
		MITK_INFO << e;
		return;
	}

	setTrasnferFunction(nodes1);

//	mitk::RenderingManager::GetInstance()->SetDataStorage(ds1);

	QmitkRenderWindowWidget *renderWidget1 = new QmitkRenderWindowWidget(ui->widget,QString("main"),ds1);
	renderWidget1->GetRenderWindow()->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);
	renderWidget1->GetRenderWindow()->GetRenderer()->GetVtkRenderer()->ResetCamera();

	ui->horizontalLayout->addWidget(renderWidget1);
	ui->horizontalLayout->setMargin(0);


//	float black[3] = { 0.0f, 0.0f, 0.0f };
//	float red[3] = { 1.0f, 0.0f, 0.0f };
//	QmitkRenderWindowWidget *renderWidget2 = new QmitkRenderWindowWidget(ui->widget,QString("main2"),ds2);
//	renderWidget2->GetRenderWindow()->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);
//	renderWidget2->GetRenderWindow()->GetRenderer()->GetVtkRenderer()->ResetCamera();
////	renderWidget2->SetGradientBackgroundColors(black,red);

//	ui->horizontalLayout_2->addWidget(renderWidget2);
//	ui->horizontalLayout_2->setMargin(0);
}

MainWindow::~MainWindow()
{
	delete ui;
}

