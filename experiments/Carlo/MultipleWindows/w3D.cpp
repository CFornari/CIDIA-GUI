#include "w3D.h"

#include <QmitkRenderWindow.h>

#include <mitkIOUtil.h>
#include <mitkProperties.h>
#include <mitkRenderingManager.h>
#include <mitkStandaloneDataStorage.h>
#include <mitkTransferFunction.h>
#include <mitkTransferFunctionProperty.h>
#include <mitkDataInteractor.h>

#include <itksys/SystemTools.hxx>
#include <QVTKOpenGLWidget.h>

#include <QHBoxLayout>

w3D::w3D(QWidget *parent)
	: QWidget(parent)
{
	QHBoxLayout *hLayout = new QHBoxLayout();
	setLayout(hLayout);

	m_RenderWindow = new QmitkRenderWindow;
	hLayout->addWidget(m_RenderWindow);
}

void w3D::setTrasnferFunction(mitk::StandaloneDataStorage::SetOfObjects::Pointer dataNodes)
{
	mitk::DataNode::Pointer node = dataNodes->at(0);

	mitk::Image::Pointer image = dynamic_cast<mitk::Image *>(node->GetData());
	if (image.IsNotNull())
	{
		// Set the property "volumerendering" to the Boolean value "true"
		node->SetProperty("volumerendering", mitk::BoolProperty::New(true));

		// Create a transfer function to assign optical properties (color and opacity) to grey-values of the data
		mitk::TransferFunction::Pointer tf = mitk::TransferFunction::New();
		tf->InitializeByMitkImage(image);

		// Set the color transfer function AddRGBPoint(double x, double r, double g, double b)
		tf->GetColorTransferFunction()->AddRGBPoint(tf->GetColorTransferFunction()->GetRange()[0], 1.0, 0.0, 0.0);
		tf->GetColorTransferFunction()->AddRGBPoint(tf->GetColorTransferFunction()->GetRange()[1], 1.0, 1.0, 0.0);

		// Set the piecewise opacity transfer function AddPoint(double x, double y)
		tf->GetScalarOpacityFunction()->AddPoint(0, 0);
		tf->GetScalarOpacityFunction()->AddPoint(tf->GetColorTransferFunction()->GetRange()[1], 1);

		node->SetProperty("TransferFunction", mitk::TransferFunctionProperty::New(tf.GetPointer()));
	}
}

void w3D::setRenderWindow(QmitkRenderWindow* win)
{
	m_RenderWindow = win;
}

void w3D::init(mitk::StandaloneDataStorage::Pointer ds)
{
	// Tell the renderwindow which (part of) the datastorage to render
	m_RenderWindow->GetRenderer()->SetDataStorage(ds);

	// Use it as a 3D view
	m_RenderWindow->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);

	// Reposition the camera to include all visible actors
	m_RenderWindow->GetRenderer()->GetVtkRenderer()->ResetCamera();
}
