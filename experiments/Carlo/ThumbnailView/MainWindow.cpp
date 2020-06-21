#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include <ThumbnailListLoader.h>
#include <ThumbnailListView.h>

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

	QmitkRenderWindowWidget *renderWidget1 = new QmitkRenderWindowWidget(ui->widget_2,QString("main"),ds1);
	renderWidget1->GetRenderWindow()->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);
	renderWidget1->GetRenderWindow()->GetRenderer()->GetVtkRenderer()->ResetCamera();

	ui->horizontalLayout->addWidget(renderWidget1);
	ui->horizontalLayout->setMargin(0);

	ThumbnailListLoader *loader = ThumbnailListLoader::GetInstance();
	loader->loadPictures(QString("/media/storage/carlo/Documents/mitk-data/covid19-SlicesPng"));

	ThumbnailListView *list = new ThumbnailListView(ui->widget_3);
//	list->setActiveAlbum(ThumbnailListView::Sagittal);


	ui->horizontalLayout_2->addWidget(list);
	ui->horizontalLayout_2->setMargin(0);
}

MainWindow::~MainWindow()
{
	delete ui;
}

