#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include "Qmitk2DRenderWidget.h"
#include "Qmitk3DRenderWidget.h"
#include "QRenderWidget.h"

#include <QmitkRenderingManager.h>
#include <QmitkDataStorageTreeModel.h>

#include <mitkProperties.h>
#include <mitkRenderingManager.h>
#include <mitkTransferFunction.h>
#include <mitkTransferFunctionProperty.h>

#include <mitkStandaloneDataStorage.h>
#include <mitkDataStorage.h>
#include <mitkIOUtil.h>

#include <QDebug>

static void setTrasnferFunction(mitk::DataNode::Pointer node)
{
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

	try
	{
//		nodes1 = mitk::IOUtil::Load(QString("/media/storage/carlo/Documents/mitk-data/Lung.nrrd").toStdString(), *ds1);
		nodes1 = mitk::IOUtil::Load(QString("/usr/mitk-development/covid-gui/test-data/Pic3D.nrrd").toStdString(), *ds1);
		mitk::IOUtil::Load(QString("/usr/mitk-development/covid-gui/test-data/lungs.vtk").toStdString(), *ds1);
	}
	catch (const mitk::Exception& e)
	{
		MITK_INFO << e;
		return;
	}

	QmitkDataStorageTreeModel *m_Tree = new QmitkDataStorageTreeModel(ds1, true, this);
	QList<mitk::DataNode::Pointer> nodeList = m_Tree->GetNodeSet();

	ds2->Add(nodeList[1]);
	setTrasnferFunction(nodeList[1]);

//	Qmitk2DRenderWidget *renderWindowWidget1 = new Qmitk2DRenderWidget(ui->widget,QString("main"));
//	renderWindowWidget1->SetDataStorage(ds1);
//	renderWindowWidget1->setSagittalView();

	QRenderWidget *renderWindowWidget1 = new QRenderWidget(ui->widget,QString("main"), ds1);
	renderWindowWidget1->setSagittalView();

	Qmitk3DRenderWidget *renderWindowWidget2 = new Qmitk3DRenderWidget(ui->widget,QString("main2"));
	renderWindowWidget2->SetDataStorage(ds1);
	renderWindowWidget2->AppendChildPlaneNodes(renderWindowWidget1->GetPlaneNode());
	renderWindowWidget2->AddPlanesToDataStorage();

	Qmitk3DRenderWidget *renderWindowWidget3 = new Qmitk3DRenderWidget(ui->widget,QString("main3"));
	renderWindowWidget3->SetDataStorage(ds2);

	ui->horizontalLayout->addWidget(renderWindowWidget1);
	ui->horizontalLayout->setMargin(0);

	ui->horizontalLayout_2->addWidget(renderWindowWidget2);
	ui->horizontalLayout_2->setMargin(0);

	ui->horizontalLayout_3->addWidget(renderWindowWidget3);
	ui->horizontalLayout_3->setMargin(0);
}

MainWindow::~MainWindow()
{
	delete ui;
}
