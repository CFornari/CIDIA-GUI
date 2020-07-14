#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include <QmitkMovieMakerView.h>
#include <QRenderWidget.h>
#include <Qmitk2DRenderWidget.h>

#include <mitkRenderingManager.h>
#include <mitkTransferFunction.h>
#include <mitkTransferFunctionProperty.h>

#include <mitkStandaloneDataStorage.h>
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

	mitk::StandaloneDataStorage::SetOfObjects::Pointer nodes1 = mitk::StandaloneDataStorage::SetOfObjects::New();

	viewAxial = new Qmitk2DRenderWidget(this,"MovieMaker0");
	viewAxial->SetDataStorage(ds1);

	viewSagittal = new QRenderWidget(this,"MovieMaker1",ds1);
	viewSagittal->setSagittalView();
	viewSagittal->hide();

	viewCoronal = new QRenderWidget(this,"MovieMaker2",ds1);
	viewCoronal->setCoronalView();
	viewCoronal->hide();

	view3D = new QRenderWidget(this,"MovieMaker3",ds1);
	view3D->set3DView();
	view3D->AppendChildPlaneNodes(viewAxial->GetPlaneNode());

	ui->horizontalLayout->addWidget(viewAxial);
	ui->horizontalLayout->addWidget(viewSagittal);
	ui->horizontalLayout->addWidget(viewCoronal);
	ui->horizontalLayout->addWidget(view3D);

	ui->horizontalLayout->setMargin(0);

	m_View = new QmitkMovieMakerView(this);
	ui->horizontalLayout_2->addWidget(m_View);

	connect(m_View, &QmitkMovieMakerView::renderWindowChanged, this, &MainWindow::onRenderWindowChanged);

	try
	{
//		nodes1 = mitk::IOUtil::Load(QString("/media/storage/carlo/Documents/mitk-data/Pic3D.nrrd").toStdString(), *ds1);
		nodes1 = mitk::IOUtil::Load(QString("/usr/mitk-development/covid-gui/test-data/Pic3D.nrrd").toStdString(), *ds1);
//		mitk::IOUtil::Load(QString("/usr/mitk-development/covid-gui/test-data/lungs.vtk").toStdString(), *ds2);
	}
	catch (const mitk::Exception& e)
	{
		MITK_INFO << e;
		return;
	}

	setTrasnferFunction(nodes1);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::onRenderWindowChanged(int renderWindow)
{
	switch (renderWindow)
	{
	case 0: // Axial
		viewAxial->show();
		viewSagittal->hide();
		viewCoronal->hide();

		view3D->RemovePlanesFromDataStorage();
		view3D->ClearChildPlaneNodes();
		view3D->AppendChildPlaneNodes(viewAxial->GetPlaneNode());
		view3D->AddPlanesToDataStorage();
		break;

	case 1: // Sagittal
		viewAxial->hide();
		viewSagittal->show();
		viewCoronal->hide();

		view3D->RemovePlanesFromDataStorage();
		view3D->ClearChildPlaneNodes();
		view3D->AppendChildPlaneNodes(viewSagittal->GetPlaneNode());
		view3D->AddPlanesToDataStorage();
		break;

	case 2: // Coronal
		viewAxial->hide();
		viewSagittal->hide();
		viewCoronal->show();

		view3D->RemovePlanesFromDataStorage();
		view3D->ClearChildPlaneNodes();
		view3D->AppendChildPlaneNodes(viewCoronal->GetPlaneNode());
		view3D->AddPlanesToDataStorage();
		break;

	default:
		break;
	}
}

void MainWindow::showEvent(QShowEvent* e)
{
	view3D->AddPlanesToDataStorage();
}

