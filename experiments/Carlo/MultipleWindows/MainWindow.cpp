#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include "w3D.h"
#include "wMulti.h"

#include <QmitkStdMultiWidget.h>
#include <QmitkRenderingManager.h>
#include <QmitkRenderWindow.h>
#include <QmitkIOUtil.h>

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
//		nodes1 = QmitkIOUtil::Load(QString("/usr/mitk-development/covid-gui/test-data/JJA10ZCU.nii"), *ds1);
		nodes1 = mitk::IOUtil::Load(QString("/usr/mitk-development/covid-gui/test-data/Pic3D.nrrd").toStdString(), *ds1);
//		nodes2 = mitk::IOUtil::Load(QString("/usr/mitk-development/covid-gui/test-data/lungs.vtk").toStdString(), *ds2);
	}
	catch (const mitk::Exception& e)
	{
		MITK_INFO << e;
		return;
	}

//	ui->widget->setTrasnferFunction(nodes1);
//	ui->widget->init(ds1);
	ui->widget_2->setData(ds1);
	QWidget *multi = ui->widget_2->getRenderWindow()->GetRenderWindow4();
//	QWidget *multi = ui->widget_2->getRenderWindow()->GetRenderWindowWidget(1,1).;
//	QSizePolicy policy;
//	policy.setHorizontalStretch(1);
//	multi->setSizePolicy(policy);

	ui->widget->layout()->addWidget(multi);
}

MainWindow::~MainWindow()
{
	delete ui;
}

