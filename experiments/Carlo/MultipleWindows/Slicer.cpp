#include "Slicer.h"

#include "QmitkRenderWindow.h"
#include "QmitkSliceWidget.h"

#include "mitkNodePredicateDataType.h"
#include "mitkProperties.h"
#include "mitkRenderingManager.h"
#include "mitkStandaloneDataStorage.h"
#include <mitkIOUtil.h>

#include <QHBoxLayout>

Slicer::Slicer(QWidget *parent) : QWidget(parent)
{
	m_SliceRenderWindow = new QmitkSliceWidget(this);
}

void Slicer::setData(mitk::StandaloneDataStorage::Pointer ds)
{
	QHBoxLayout *layout = new QHBoxLayout();
	setLayout(layout);

	// Create QmitkSliceWidget, which is based on the class
	// QmitkRenderWindow, but additionally provides sliders
	layout->addWidget(m_SliceRenderWindow);
	m_SliceRenderWindow->SetLevelWindowEnabled(true);
	// Tell the QmitkSliceWidget which (part of) the tree to render.
	// By default, it slices the data axially
	m_SliceRenderWindow->SetDataStorage(ds);

	// Get the image from the data storage. A predicate (mitk::NodePredicateBase)
	// is used to get only nodes of the type mitk::Image.
	mitk::DataStorage::SetOfObjects::ConstPointer rs = ds->GetSubset(mitk::TNodePredicateDataType<mitk::Image>::New());

	m_SliceRenderWindow->SetData(rs->Begin(), mitk::SliceNavigationController::Axial);
	// We want to see the position of the slice in 2D and the
	// slice itself in 3D: add it to the datastorage!
	ds->Add(m_SliceRenderWindow->GetRenderer()->GetCurrentWorldPlaneGeometryNode());
}
