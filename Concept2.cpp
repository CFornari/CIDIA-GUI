#include "Concept2.h"
#include "ui_Concept2.h"

#include "AppDataManager.h"

#include <QmitkStdMultiWidget.h>

Concept2::Concept2(QWidget *parent)
	:	QWidget(parent),
		ui(new Ui::Concept2)
{
	ui->setupUi(this);

	m_AppData = AppDataManager::GetInstance();
	m_AppData->createDataStorageFromMaster("Multi");
	auto dataStorage = m_AppData->getDataStorageByName("Multi");

	m_MultiWidget = new QmitkStdMultiWidget(this);
	m_MultiWidget->SetDataStorage(dataStorage);
	m_MultiWidget->InitializeMultiWidget();
	m_MultiWidget->ResetCrosshair();
	m_MultiWidget->AddPlanesToDataStorage();

	ui->MultiWidgetLayout->addWidget(m_MultiWidget);
}

Concept2::~Concept2()
{
	m_MultiWidget->RemovePlanesFromDataStorage();
	delete ui;
}

void Concept2::showEvent(QShowEvent* e)
{

}

void Concept2::hideEvent(QHideEvent* e)
{

}

