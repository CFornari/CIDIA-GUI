#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include <mitkTransferFunctionProperty.h>
#include <mitkDataInteractor.h>

//#include "Qmitk3DRenderWidget.h"
#include <QmitkStdMultiWidget.h>

#include <mitkStandaloneDataStorage.h>
#include <mitkIOUtil.h>

static void setTrasnferFunction(mitk::StandaloneDataStorage::SetOfObjects::Pointer dataNodes)
{
	mitk::DataNode::Pointer node = dataNodes->at(0);

	mitk::Image::Pointer image = dynamic_cast<mitk::Image *>(node->GetData());
	if (image.IsNotNull())
	{
		// Set the property "volumerendering" to the Boolean value "true"
		node->SetProperty("volumerendering", mitk::BoolProperty::New(true));

		// Create a transfer function to assign optical properties (color and opacity) to grey-values of the data
		mitk::TransferFunction::Pointer tf = mitk::TransferFunction::New();
		tf->InitializeByMitkImage(image);

		// Set the color transfer function AddRGBPoint(double x, double r, double g, double b)
		tf->GetColorTransferFunction()->AddRGBPoint(tf->GetColorTransferFunction()->GetRange()[0], 1.0, 0.0, 0.0);
		tf->GetColorTransferFunction()->AddRGBPoint(tf->GetColorTransferFunction()->GetRange()[1], 1.0, 1.0, 0.0);

		// Set the piecewise opacity transfer function AddPoint(double x, double y)
		tf->GetScalarOpacityFunction()->AddPoint(0, 0);
		tf->GetScalarOpacityFunction()->AddPoint(tf->GetColorTransferFunction()->GetRange()[1], 1);

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
	mitk::StandaloneDataStorage::SetOfObjects::Pointer nodes2 = mitk::StandaloneDataStorage::SetOfObjects::New();

	try
	{
//		nodes1 = QmitkIOUtil::Load(QString("/usr/mitk-development/covid-gui/test-data/JJA10ZCU.nii"), *ds1);
		nodes1 = mitk::IOUtil::Load(QString("/usr/mitk-development/covid-gui/test-data/Pic3D.nrrd").toStdString(), *ds1);
//		nodes1 = mitk::IOUtil::Load(QString("/media/storage/carlo/Documents/mitk-data/Lung.nrrd").toStdString(), *ds1);
//		nodes2 = mitk::IOUtil::Load(QString("/usr/mitk-development/covid-gui/test-data/lungs.vtk").toStdString(), *ds2);
	}
	catch (const mitk::Exception& e)
	{
		MITK_INFO << e;
		return;
	}

	setTrasnferFunction(nodes1);

	m_MultiWidget = new QmitkStdMultiWidget(this);
	m_MultiWidget->SetDataStorage(ds1);
	m_MultiWidget->InitializeMultiWidget();
	m_MultiWidget->ResetCrosshair();
	m_MultiWidget->RequestUpdateAll();
	m_MultiWidget->AddPlanesToDataStorage();

	ui->horizontalLayout->addWidget(m_MultiWidget);
}

MainWindow::~MainWindow()
{
	delete ui;
}

