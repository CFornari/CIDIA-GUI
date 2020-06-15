#include "wMulti.h"

#include <QmitkStdMultiWidget.h>
#include <QmitkRenderWindow.h>
#include <QmitkRenderingManager.h>

#include <QHBoxLayout>
#include <QVBoxLayout>

wMulti::wMulti(QWidget *parent)
	: QWidget(parent)
{
	m_DataStorage = mitk::StandaloneDataStorage::New();
	m_MultiWidget = new QmitkStdMultiWidget;
}

void wMulti::setData(mitk::StandaloneDataStorage::Pointer ds)
{
	m_DataStorage = ds;

	QHBoxLayout *hlayout = new QHBoxLayout(this);
	hlayout->setMargin(0);

	hlayout->addWidget(m_MultiWidget);

	// Tell the multiWidget which DataStorage to render
	m_MultiWidget->SetDataStorage(m_DataStorage);

	m_MultiWidget->InitializeMultiWidget();

//	m_MultiWidget->ResetCrosshair();

//	m_MultiWidget->GetMultiWidgetLayoutManager()->SetLayoutDesign(QmitkMultiWidgetLayoutManager::LayoutDesign::ONLY_2D_VERTICAL);

	// Add the displayed views to the DataStorage to see their positions in 2D and 3D
	m_MultiWidget->AddPlanesToDataStorage();
}

QmitkStdMultiWidget* wMulti::getRenderWindow()
{
	return m_MultiWidget;
}


