// Workaround: I don't know why this is needed.
// This line should come before `#include <mitkPlaneGeometryDataMapper2D.h>` or
// this error will happen:
// #error Missing preprocessor define US_MODULE_NAME
#define US_MODULE_NAME "Qmitk2DRenderWidget"

#include "Qmitk2DRenderWidget.h"

#include <mitkPlaneGeometryDataMapper2D.h>

Qmitk2DRenderWidget::Qmitk2DRenderWidget(QWidget *parent,
																				 const QString& widgetName)
	: QmitkAbstractRenderWidget(parent, widgetName)
{
	mitk::BaseRenderer::GetInstance(m_RenderWindow->GetRenderWindow())->SetMapperID(mitk::BaseRenderer::Standard2D);

	setAxialView();
}

void Qmitk2DRenderWidget::AddPlaneToDataStorage()
{
	if (nullptr == m_DataStorage)
	{
		std::cout << "WARNING: QRenderWidget::AddPlanesToDataStorage() was called, "
								 "but data storage is nullptr." << endl;
		return;
	}

	if(m_PlaneNode.IsNull())
	{
		std::cout << "WARNING: QRenderWidget::AddPlanesToDataStorage() was called, "
								 "but no plane node was created." << endl;
		return;
	}

	m_DataStorage->Add(m_PlaneNode);
}

void Qmitk2DRenderWidget::RemovePlaneFromDataStorage()
{
	if (nullptr == m_DataStorage)
	{
		std::cout << "WARNING: QRenderWidget::AddPlanesToDataStorage() was called, "
								 "but data storage is nullptr." << endl;
		return;
	}

	if(m_PlaneNode.IsNull())
	{
		std::cout << "WARNING: QRenderWidget::AddPlanesToDataStorage() was called, "
								 "but no plane node was created." << endl;
		return;
	}

	m_DataStorage->Remove(m_PlaneNode);
}

void Qmitk2DRenderWidget::setAxialView()
{
	m_RenderWindow->SetLayoutIndex(mitk::BaseRenderer::ViewDirection::AXIAL);
	m_RenderWindow->GetSliceNavigationController()->SetDefaultViewDirection(mitk::SliceNavigationController::Axial);

	float red[3] = { 0.753f, 0.0f, 0.0f }; // This is #C00000 in hex
	SetDecorationColor(red);
	SetCornerAnnotationText("Axial");

	CreatePlaneNode();
	m_PlaneNode->SetColor(red);

	ResetView();
}

void Qmitk2DRenderWidget::setSagittalView()
{
	m_RenderWindow->SetLayoutIndex(mitk::BaseRenderer::ViewDirection::SAGITTAL);
	m_RenderWindow->GetSliceNavigationController()->SetDefaultViewDirection(mitk::SliceNavigationController::Sagittal);

	float green[3] = { 0.0f, 0.69f, 0.0f }; // This is #00B000 in hex
	SetDecorationColor(green);
	SetCornerAnnotationText("Sagittal");

	CreatePlaneNode();
	m_PlaneNode->SetColor(green);

	ResetView();
}

void Qmitk2DRenderWidget::setCoronalView()
{
	m_RenderWindow->SetLayoutIndex(mitk::BaseRenderer::ViewDirection::CORONAL);
	m_RenderWindow->GetSliceNavigationController()->SetDefaultViewDirection(mitk::SliceNavigationController::Frontal);

	float blue[3] = { 0.0f, 0.502f, 1.0f }; // This is #0080FF in hex
	SetDecorationColor(blue);
	SetCornerAnnotationText("Coronal");

	CreatePlaneNode();
	m_PlaneNode->SetColor(blue);

	ResetView();
}

void Qmitk2DRenderWidget::CreatePlaneNode()
{
	mitk::PlaneGeometryDataMapper2D::Pointer mapper;
	mitk::BaseRenderer* renderer = mitk::BaseRenderer::GetInstance(m_RenderWindow->GetRenderWindow());
	m_PlaneNode = renderer->GetCurrentWorldPlaneGeometryNode();
	m_PlaneNode->SetProperty("visible", mitk::BoolProperty::New(true));
	m_PlaneNode->SetProperty("name", mitk::StringProperty::New(m_WidgetName.toStdString() + ".plane"));
	m_PlaneNode->SetProperty("includeInBoundingBox", mitk::BoolProperty::New(false));
	m_PlaneNode->SetProperty("helper object", mitk::BoolProperty::New(true));
	mapper = mitk::PlaneGeometryDataMapper2D::New();
	m_PlaneNode->SetMapper(mitk::BaseRenderer::Standard2D, mapper);
}
