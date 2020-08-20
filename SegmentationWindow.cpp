#include "SegmentationWindow.h"
#include "ui_SegmentationWindow.h"

#include "AppDataManager.h"

#include <QmitkStdMultiWidget.h>

#include <QmitkMultiLabelSegmentationView.h>
#include <SegmentationSettings.h>

SegmentationWindow::SegmentationWindow(QWidget *parent)
	:	QWidget(parent),
		ui(new Ui::SegmentationWindow)
{
	ui->setupUi(this);

	m_AppData = AppDataManager::GetInstance();
	m_AppData->createDataStorageFromMaster("Multi");
	auto dataStorage = m_AppData->getDataStorageByName("Multi");

	m_MultiWidget = new QmitkStdMultiWidget(this);
	m_MultiWidget->SetDataStorage(dataStorage);
	m_MultiWidget->InitializeMultiWidget();
	m_MultiWidget->ResetCrosshair();
	m_MultiWidget->RequestUpdateAll();
	m_MultiWidget->AddPlanesToDataStorage();

	ui->MultiWidgetLayout->addWidget(m_MultiWidget);

	m_SegView = new QmitkMultiLabelSegmentationView(ui->scrollAreaWidgetContents, m_MultiWidget, dataStorage);
//	ui->horizontalLayout_2->addWidget(m_SegView);
}

SegmentationWindow::~SegmentationWindow()
{
	m_MultiWidget->RemovePlanesFromDataStorage();
	delete ui;
}

void SegmentationWindow::showEvent(QShowEvent* e)
{

}

void SegmentationWindow::hideEvent(QHideEvent* e)
{

}

