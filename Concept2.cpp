#include "Concept2.h"
#include "ui_Concept2.h"

#include <QmitkStdMultiWidget.h>

Concept2::Concept2(QWidget *parent,
									 mitk::StandaloneDataStorage::Pointer ds)
	:	QWidget(parent),
		m_DataStorage(ds),
		ui(new Ui::Concept2)
{
	ui->setupUi(this);

	m_MultiWidget = new QmitkStdMultiWidget(this);
	m_MultiWidget->SetDataStorage(m_DataStorage);
	m_MultiWidget->InitializeMultiWidget();
	m_MultiWidget->ResetCrosshair();
	ui->MultiWidgetLayout->addWidget(m_MultiWidget);
}

Concept2::~Concept2()
{
	m_MultiWidget->RemovePlanesFromDataStorage();
	delete ui;
}

void Concept2::AddPlanesToDataStorage()
{
	if(m_MultiWidget)
		m_MultiWidget->AddPlanesToDataStorage();
}

void Concept2::RemovePlanesFromDataStorage()
{
	if(m_MultiWidget)
		m_MultiWidget->RemovePlanesFromDataStorage();
}

