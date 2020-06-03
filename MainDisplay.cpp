#include "MainDisplay.h"

#include <QmitkStdMultiWidget.h>
#include <QmitkRenderWindow.h>

#include <QHBoxLayout>
#include <QVBoxLayout>

MainDisplay::MainDisplay(QWidget *parent)
	:	QWidget(parent)
{
	m_MultiWidget = new QmitkStdMultiWidget;
}

void MainDisplay::setDataStorage(mitk::StandaloneDataStorage::Pointer dataStorage)
{
	m_DataStorage = dataStorage;

	QHBoxLayout *hlayout = new QHBoxLayout(this);
	hlayout->setMargin(0);

	hlayout->addWidget(m_MultiWidget);

	// Tell the multiWidget which DataStorage to render
	m_MultiWidget->SetDataStorage(m_DataStorage);
	m_MultiWidget->InitializeMultiWidget();
	m_MultiWidget->GetMultiWidgetLayoutManager()->SetLayoutDesign(QmitkMultiWidgetLayoutManager::LayoutDesign::ONLY_2D_VERTICAL);

	// Add the displayed views to the DataStorage to see their positions in 2D and 3D
	m_MultiWidget->AddPlanesToDataStorage();
}

QmitkRenderWindow* MainDisplay::get3DRenderWindow() const
{
	return m_MultiWidget->GetRenderWindow4();
}

