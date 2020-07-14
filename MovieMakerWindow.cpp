#include "MovieMakerWindow.h"
#include "ui_MovieMakerWindow.h"

#include <Qmitk2DRenderWidget.h>
#include <Qmitk3DRenderWidget.h>
#include <AppDataManager.h>
#include <QmitkMovieMakerView.h>

MovieMakerWindow::MovieMakerWindow(QWidget *parent)
	:	QWidget(parent),
		ui(new Ui::MovieMakerWindow)
{
	ui->setupUi(this);

	m_AppData = AppDataManager::GetInstance();
	m_AppData->createDataStorageFromMaster("MovieMaker");
	auto dataStorage = m_AppData->getDataStorageByName("MovieMaker");


	m_ViewAxial = new Qmitk2DRenderWidget(this, QString("MovieMaker0"));
	m_ViewAxial->SetDataStorage(dataStorage);

	m_ViewSagittal = new Qmitk2DRenderWidget(this, QString("MovieMaker1"));
	m_ViewSagittal->SetDataStorage(dataStorage);
	m_ViewSagittal->setSagittalView();
	m_ViewSagittal->hide();

	m_ViewCoronal = new Qmitk2DRenderWidget(this, QString("MovieMaker2"));
	m_ViewCoronal->SetDataStorage(dataStorage);
	m_ViewCoronal->setCoronalView();
	m_ViewCoronal->hide();

	m_View3D = new Qmitk3DRenderWidget(this, QString("MovieMaker3"));
	m_View3D->SetDataStorage(dataStorage);
	m_View3D->AppendChildPlaneNodes(m_ViewAxial->GetPlaneNode());

	ui->horizontalLayout_2->addWidget(m_ViewAxial);
	ui->horizontalLayout_2->addWidget(m_ViewSagittal);
	ui->horizontalLayout_2->addWidget(m_ViewCoronal);
	ui->horizontalLayout_3->addWidget(m_View3D);

	m_MovieView = new QmitkMovieMakerView(this);
	ui->verticalLayout_3->addWidget(m_MovieView);

	connect(m_AppData, &AppDataManager::newDataLoadedEnd, this, &MovieMakerWindow::onNewDataLoadedEnd);
	connect(m_MovieView, &QmitkMovieMakerView::renderWindowChanged, this, &MovieMakerWindow::onRenderWindowChanged);
	connect(m_MovieView, &QmitkMovieMakerView::addAnimationButtonClicked, this, &MovieMakerWindow::onAddAnimationButtonClicked);
	connect(m_MovieView, &QmitkMovieMakerView::removeAnimationButtonClicked, this, &MovieMakerWindow::onRemoveAnimationButtonClicked);


}

MovieMakerWindow::~MovieMakerWindow()
{
	delete ui;
}

void MovieMakerWindow::onNewDataLoadedEnd()
{
	resetViews();
}

void MovieMakerWindow::showEvent(QShowEvent* e)
{
	Q_UNUSED(e)
	m_AppData->debugListNodesNamesFromFiles();
	m_AppData->debugListNodesNamesFromTree();
}

void MovieMakerWindow::hideEvent(QHideEvent* e)
{
	Q_UNUSED(e)
}

void MovieMakerWindow::resetViews()
{
	m_ViewAxial->ResetView();
	m_ViewSagittal->ResetView();
	m_ViewCoronal->ResetView();
	m_View3D->ResetView();
}

void MovieMakerWindow::onRenderWindowChanged(int renderWindow)
{
	switch (renderWindow)
	{
	case 0: // Axial
		m_ViewAxial->show();
		m_ViewSagittal->hide();
		m_ViewCoronal->hide();

		m_View3D->RemovePlanesFromDataStorage();
		m_View3D->ClearChildPlaneNodes();
		m_View3D->AppendChildPlaneNodes(m_ViewAxial->GetPlaneNode());
		m_View3D->AddPlanesToDataStorage();
		break;

	case 1: // Sagittal
		m_ViewAxial->hide();
		m_ViewSagittal->show();
		m_ViewCoronal->hide();

		m_View3D->RemovePlanesFromDataStorage();
		m_View3D->ClearChildPlaneNodes();
		m_View3D->AppendChildPlaneNodes(m_ViewSagittal->GetPlaneNode());
		m_View3D->AddPlanesToDataStorage();
		break;

	case 2: // Coronal
		m_ViewAxial->hide();
		m_ViewSagittal->hide();
		m_ViewCoronal->show();

		m_View3D->RemovePlanesFromDataStorage();
		m_View3D->ClearChildPlaneNodes();
		m_View3D->AppendChildPlaneNodes(m_ViewCoronal->GetPlaneNode());
		m_View3D->AddPlanesToDataStorage();
		break;

	default:
		break;
	}
}

void MovieMakerWindow::onAddAnimationButtonClicked()
{
	m_View3D->AddPlanesToDataStorage();
}

void MovieMakerWindow::onRemoveAnimationButtonClicked()
{
	m_View3D->RemovePlanesFromDataStorage();
}
