#include "wMulti.h"

#include <QmitkRenderWindow.h>
#include <QmitkRenderingManager.h>
#include <mitkTimeGeometry.h>

#include <mitkIOUtil.h>

#include <QHBoxLayout>
#include <QVBoxLayout>

wMulti::wMulti(QWidget *parent)
	: QWidget(parent),
		m_DataStorage(mitk::StandaloneDataStorage::New())
{

}

void wMulti::setData(mitk::StandaloneDataStorage::Pointer ds)
{
	m_DataStorage = ds;
	//*************************************************************************
	// Part I: Create windows and pass the tree to it
	//*************************************************************************

	// Create toplevel widget with vertical layout
	QVBoxLayout *vlayout = new QVBoxLayout(this);
	vlayout->setMargin(0);
	vlayout->setSpacing(2);

	// Create viewParent widget with horizontal layout
	QWidget *viewParent = new QWidget(this);
	vlayout->addWidget(viewParent);
	QHBoxLayout *hlayout = new QHBoxLayout(viewParent);
	hlayout->setMargin(0);

	//*************************************************************************
	// Part Ia: create and initialize QmitkStdMultiWidget
	//*************************************************************************
	QmitkStdMultiWidget *m_MultiWidget = new QmitkStdMultiWidget(viewParent);
	hlayout->addWidget(m_MultiWidget);

	// Tell the multiWidget which DataStorage to render
	m_MultiWidget->SetDataStorage(m_DataStorage);
	m_MultiWidget->InitializeMultiWidget();

	// Initialize views as axial, sagittal, coronar (from
	// top-left to bottom)
	auto geo = m_DataStorage->ComputeBoundingGeometry3D(m_DataStorage->GetAll());
	QmitkRenderingManager::GetInstance()->InitializeViews(geo);

	// Initialize bottom-right view as 3D view
	m_MultiWidget->GetRenderWindow4()->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);

	// Add the displayed views to the DataStorage to see their positions in 2D and 3D
	m_MultiWidget->AddDisplayPlaneSubTree();
	m_MultiWidget->AddPlanesToDataStorage();
	m_MultiWidget->SetWidgetPlanesVisibility(true);
}

QmitkRenderWindow* wMulti::get3DRenderWindow()
{
	return m_MultiWidget->GetRenderWindow4();
}
