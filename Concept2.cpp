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

	m_MultiWidget = new QmitkStdMultiWidget(this);
	m_MultiWidget->SetDataStorage(m_AppData->getDataStorage());
	m_MultiWidget->InitializeMultiWidget();
	m_MultiWidget->ResetCrosshair();
	ui->MultiWidgetLayout->addWidget(m_MultiWidget);
}

Concept2::~Concept2()
{
	m_MultiWidget->RemovePlanesFromDataStorage();
	delete ui;
}

void Concept2::onVisibilityChanged(bool visible)
{
	this->setVisible(visible);
}

