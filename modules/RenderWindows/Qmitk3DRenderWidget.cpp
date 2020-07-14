// Workaround: I don't know why this is needed.
// This line should come before `#include <mitkPlaneGeometryDataMapper2D.h>` or
// this error will happen:
// #error Missing preprocessor define US_MODULE_NAME
#define US_MODULE_NAME "Qmitk3DRenderWidget"

#include "Qmitk3DRenderWidget.h"

#include <mitkPlaneGeometryDataMapper2D.h>

Qmitk3DRenderWidget::Qmitk3DRenderWidget(QWidget *parent,
																				 const QString& widgetName)
	: QmitkAbstractRenderWidget(parent, widgetName)
{
	float yellow[3] = { 1.0f, 1.0f, 0.0f }; // This is #0080FF in hex
	SetDecorationColor(yellow);
	SetCornerAnnotationText("3D");

	CreatePlaneNode();

	Set3DView();

	float white[3] = { 1.0f, 1.0f, 1.0f };
	SetGradientBackgroundColors(white, white);
}

void Qmitk3DRenderWidget::AppendChildPlaneNodes(mitk::DataNode::Pointer childPlaneNode)
{
	if(childPlaneNode == nullptr)
	{
		std::cout << "WARNING: QRenderWidget::AppendChildPlaneNodes() was called, "
								 "but children is nullptr." << endl;
		return;
	}

	m_ChildPlaneNodes.append(childPlaneNode);
}

void Qmitk3DRenderWidget::ClearChildPlaneNodes()
{
	if(m_ChildPlaneNodes.size() <= 0)
	{
		std::cout << "WARNING: QRenderWidget::ClearChildPlaneNodes() was called, "
								 "but no children nodes exist." << endl;
		return;
	}


	m_ChildPlaneNodes.clear();
}

void Qmitk3DRenderWidget::AddPlanesToDataStorage()
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

	if(m_ChildPlaneNodes.size() <= 0)
	{
		std::cout << "WARNING: QRenderWidget::AddPlanesToDataStorage() was called, "
								 "but no children nodes exist for 3D view." << endl;
		return;
	}

	if(!m_DataStorage->Exists(m_PlaneNode))
		m_DataStorage->Add(m_PlaneNode);

	for(auto childNode : m_ChildPlaneNodes)
	{
		if(!m_DataStorage->Exists(childNode))
			m_DataStorage->Add(childNode, m_PlaneNode);
	}
}

void Qmitk3DRenderWidget::RemovePlanesFromDataStorage()
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

	if(m_ChildPlaneNodes.size() <= 0)
	{
		std::cout << "WARNING: QRenderWidget::AddPlanesToDataStorage() was called, "
								 "but no children nodes exist for 3D view." << endl;
		return;
	}

	for(auto childNode : m_ChildPlaneNodes)
	{
		if(m_DataStorage->Exists(childNode))
			m_DataStorage->Remove(childNode);
	}

	if(m_DataStorage->Exists(m_PlaneNode))
		m_DataStorage->Remove(m_PlaneNode);
}

void Qmitk3DRenderWidget::CreatePlaneNode()
{
	m_PlaneNode = mitk::DataNode::New();
	m_PlaneNode->SetProperty("name", mitk::StringProperty::New(m_WidgetName.toStdString() + ".plane"));
	m_PlaneNode->SetProperty("helper object", mitk::BoolProperty::New(true));
}

void Qmitk3DRenderWidget::Set3DView()
{
	m_RenderWindow->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);

	// These two lines are present in QmitkStdMultiWidget, but they doesn't appear to have any effect.
	// When I remove them, I can't see any different behavior in the application.
	m_RenderWindow->GetSliceNavigationController()->SetDefaultViewDirection(mitk::SliceNavigationController::Original);
	m_RenderWindow->SetLayoutIndex(mitk::BaseRenderer::ViewDirection::THREE_D);

	ResetView();
}

void Qmitk3DRenderWidget::showEvent(QShowEvent* e)
{
	Q_UNUSED(e)

}

void Qmitk3DRenderWidget::hideEvent(QHideEvent* e)
{
	Q_UNUSED(e)

}
