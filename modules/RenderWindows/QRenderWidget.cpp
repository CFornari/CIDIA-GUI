// Workaround: I don't know why this is needed.
// This line should come before `#include <mitkPlaneGeometryDataMapper2D.h>` or
// this error will happen:
// #error Missing preprocessor define US_MODULE_NAME
#define US_MODULE_NAME "QRenderWidget"

#include "QRenderWidget.h"

// vtk
#include <vtkCornerAnnotation.h>
#include <vtkTextProperty.h>

#include <mitkDisplayActionEventHandlerStd.h>
#include <mitkPlaneGeometryDataMapper2D.h>
#include <mitkCameraController.h>

#include <mitkPropertyList.h>

#include <QMouseEvent>

QRenderWidget::QRenderWidget(QWidget* parent,
														 const QString& widgetName,
														 mitk::DataStorage* dataStorage)
	: QFrame(parent),
		m_WidgetName(widgetName),
		m_DataStorage(dataStorage),
		m_RenderWindow(nullptr),
		m_PointSetNode(nullptr),
		m_PointSet(nullptr),
		m_Is3DView(false)
{
	m_TimeNavigationController = mitk::RenderingManager::GetInstance()->GetTimeNavigationController();

	InitGUI();
	setAxialView();

	m_TimeNavigationController->ConnectGeometryTimeEvent(m_RenderWindow->GetSliceNavigationController(), false);
	m_RenderWindow->GetSliceNavigationController()->ConnectGeometryTimeEvent(m_TimeNavigationController, false);

	InitDisplayActionEventHandling();

	ResetView();
}

void QRenderWidget::SetDataStorage(mitk::DataStorage* dataStorage)
{
	if (dataStorage == m_DataStorage)
	{
		return;
	}

	m_DataStorage = dataStorage;
	if (nullptr != m_RenderWindow)
	{
		mitk::BaseRenderer::GetInstance(m_RenderWindow->GetRenderWindow())->SetDataStorage(dataStorage);
	}
}

void QRenderWidget::AppendChildPlaneNodes(mitk::DataNode::Pointer childPlaneNode)
{
	if(childPlaneNode == nullptr)
	{
		std::cout << "WARNING: QRenderWidget::AppendChildPlaneNodes() was called, "
								 "but children is nullptr." << endl;
		return;
	}

	m_ChildPlaneNodes.append(childPlaneNode);
}

void QRenderWidget::ClearChildPlaneNodes()
{
	if(m_ChildPlaneNodes.size() <= 0)
	{
		std::cout << "WARNING: QRenderWidget::ClearChildPlaneNodes() was called, "
								 "but no children nodes exist." << endl;
		return;
	}


	m_ChildPlaneNodes.clear();
}

void QRenderWidget::AddPlanesToDataStorage()
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

	if(m_Is3DView)
	{
		if(m_ChildPlaneNodes.size() <= 0)
		{
			std::cout << "WARNING: QRenderWidget::AddPlanesToDataStorage() was called, "
									 "but no children nodes exist for 3D view." << endl;
			return;
		}

		for(auto childNode : m_ChildPlaneNodes)
		{
			m_DataStorage->Add(childNode, m_PlaneNode);
		}
	}
}

void QRenderWidget::RemovePlanesFromDataStorage()
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

	if(m_Is3DView)
	{
		if(m_ChildPlaneNodes.size() <= 0)
		{
			std::cout << "WARNING: QRenderWidget::AddPlanesToDataStorage() was called, "
									 "but no children nodes exist for 3D view." << endl;
			return;
		}

		for(auto childNode : m_ChildPlaneNodes)
		{
			m_DataStorage->Remove(childNode);
		}
	}

	m_DataStorage->Remove(m_PlaneNode);
}

mitk::SliceNavigationController* QRenderWidget::GetSliceNavigationController() const
{
	return m_RenderWindow->GetSliceNavigationController();
}

void QRenderWidget::RequestUpdate()
{
	mitk::RenderingManager::GetInstance()->RequestUpdate(m_RenderWindow->GetRenderWindow());
}

void QRenderWidget::ForceImmediateUpdate()
{
	mitk::RenderingManager::GetInstance()->ForceImmediateUpdate(m_RenderWindow->GetRenderWindow());
}

void QRenderWidget::SetGradientBackgroundColors(const mitk::Color& upper, const mitk::Color& lower)
{
	vtkRenderer* vtkRenderer = m_RenderWindow->GetRenderer()->GetVtkRenderer();
	if (nullptr == vtkRenderer)
	{
		return;
	}

	m_GradientBackgroundColors.first = upper;
	m_GradientBackgroundColors.second = lower;
	vtkRenderer->SetBackground(lower[0], lower[1], lower[2]);
	vtkRenderer->SetBackground2(upper[0], upper[1], upper[2]);

	ShowGradientBackground(true);
}

void QRenderWidget::ShowGradientBackground(bool show)
{
	m_RenderWindow->GetRenderer()->GetVtkRenderer()->SetGradientBackground(show);
}

bool QRenderWidget::IsGradientBackgroundOn() const
{
	return m_RenderWindow->GetRenderer()->GetVtkRenderer()->GetGradientBackground();
}

void QRenderWidget::SetDecorationColor(const mitk::Color& color)
{
	m_DecorationColor = color;
	m_CornerAnnotation->GetTextProperty()->SetColor(m_DecorationColor[0], m_DecorationColor[1], m_DecorationColor[2]);

	QColor hexColor(m_DecorationColor[0] * 255, m_DecorationColor[1] * 255, m_DecorationColor[2] * 255);
	setStyleSheet("QRenderWidget { border: 2px solid " + hexColor.name(QColor::HexRgb) + "; }");
}

void QRenderWidget::ShowColoredRectangle(bool show)
{
	if (show)
	{
		setFrameStyle(QFrame::Box | QFrame::Plain);
	}
	else
	{
		setFrameStyle(NoFrame);
	}
}

bool QRenderWidget::IsColoredRectangleVisible() const
{
	return frameStyle() > 0;
}

void QRenderWidget::ShowCornerAnnotation(bool show)
{
	m_CornerAnnotation->SetVisibility(show);
}

bool QRenderWidget::IsCornerAnnotationVisible() const
{
	return m_CornerAnnotation->GetVisibility() > 0;
}

void QRenderWidget::SetCornerAnnotationText(const std::string& cornerAnnotation)
{
	if(m_CornerAnnotation != nullptr)
	{
		m_CornerAnnotation->SetText(0, cornerAnnotation.c_str());
	}
	else
	{
		std::cout << "m_CornerAnnotation was not initiated" << endl;
	}
}

std::string QRenderWidget::GetCornerAnnotationText() const
{
	return std::string(m_CornerAnnotation->GetText(0));
}

bool QRenderWidget::IsRenderWindowMenuActivated() const
{
	return m_RenderWindow->GetActivateMenuWidgetFlag();
}

void QRenderWidget::ActivateCrosshair(bool activate)
{
	if (nullptr == m_DataStorage)
	{
		return;
	}

	if (activate)
	{
		try
		{
			m_DataStorage->Add(m_PointSetNode);
		}
		catch(std::invalid_argument& /*e*/)
		{
			// crosshair already existing
			return;
		}
	}
	else
	{
		m_DataStorage->Remove(m_PointSetNode);
	}
}

void QRenderWidget::SetCrosshairVisibility(bool visible)
{
	if (m_PlaneNode.IsNotNull())
	{
		m_PlaneNode->SetVisibility(visible);
	}

	RequestUpdate();
}

bool QRenderWidget::GetCrosshairVisibility() const
{
	if (m_PlaneNode.IsNotNull())
	{
		return m_PlaneNode->IsVisible(mitk::BaseRenderer::GetInstance(m_RenderWindow->GetRenderWindow()));
	}
	return false;
}

void QRenderWidget::SetWidgetPlaneMode(int mode)
{
	MITK_DEBUG << "Changing crosshair mode to " << mode;

	switch (mode)
	{
	case 0:
		SetInteractionScheme(mitk::InteractionSchemeSwitcher::MITKStandard);
		break;
	case 1:
		SetInteractionScheme(mitk::InteractionSchemeSwitcher::MITKRotationUncoupled);
		break;
	case 2:
		SetInteractionScheme(mitk::InteractionSchemeSwitcher::MITKRotationCoupled);
		break;
	case 3:
		SetInteractionScheme(mitk::InteractionSchemeSwitcher::MITKSwivel);
		break;
	}
}

void QRenderWidget::SetInteractionScheme(mitk::InteractionSchemeSwitcher::InteractionScheme scheme)
{
	auto interactionSchemeSwitcher = mitk::InteractionSchemeSwitcher::New();
	auto interactionEventHandler = m_DisplayActionEventBroadcast.GetPointer();
	try
	{
		interactionSchemeSwitcher->SetInteractionScheme(interactionEventHandler, scheme);
	}
	catch (const mitk::Exception&)
	{
		return;
	}
}

void QRenderWidget::ResetView()
{
	auto geo = m_DataStorage->ComputeBoundingGeometry3D(m_DataStorage->GetAll());
	mitk::RenderingManager::GetInstance()->InitializeView(m_RenderWindow->GetRenderWindow(), geo);

	mitk::BaseRenderer::GetInstance(m_RenderWindow->GetRenderWindow())->GetVtkRenderer()->ResetCamera();
	mitk::BaseRenderer::GetInstance(m_RenderWindow->GetRenderWindow())->GetCameraController()->Fit();
}

void QRenderWidget::setAxialView()
{
	m_Is3DView = false;

	m_RenderWindow->SetLayoutIndex(mitk::BaseRenderer::ViewDirection::AXIAL);
	m_RenderWindow->GetSliceNavigationController()->SetDefaultViewDirection(mitk::SliceNavigationController::Axial);
	SetCornerAnnotationText("Axial");
	mitk::BaseRenderer::GetInstance(m_RenderWindow->GetRenderWindow())->SetMapperID(mitk::BaseRenderer::Standard2D);

	float red[3] = { 0.753f, 0.0f, 0.0f }; // This is #C00000 in hex
	SetDecorationColor(red);

	CreatePlaneNode();
	m_PlaneNode->SetColor(red);

	ResetView();
}

void QRenderWidget::setSagittalView()
{
	m_Is3DView = false;

	m_RenderWindow->SetLayoutIndex(mitk::BaseRenderer::ViewDirection::SAGITTAL);
	m_RenderWindow->GetSliceNavigationController()->SetDefaultViewDirection(mitk::SliceNavigationController::Sagittal);
	SetCornerAnnotationText("Sagittal");
	mitk::BaseRenderer::GetInstance(m_RenderWindow->GetRenderWindow())->SetMapperID(mitk::BaseRenderer::Standard2D);

	float green[3] = { 0.0f, 0.69f, 0.0f }; // This is #00B000 in hex
	SetDecorationColor(green);

	CreatePlaneNode();
	m_PlaneNode->SetColor(green);

	ResetView();
}

void QRenderWidget::setCoronalView()
{
	m_Is3DView = false;

	m_RenderWindow->SetLayoutIndex(mitk::BaseRenderer::ViewDirection::CORONAL);
	m_RenderWindow->GetSliceNavigationController()->SetDefaultViewDirection(mitk::SliceNavigationController::Frontal);
	SetCornerAnnotationText("Coronal");
	mitk::BaseRenderer::GetInstance(m_RenderWindow->GetRenderWindow())->SetMapperID(mitk::BaseRenderer::Standard2D);

	float blue[3] = { 0.0f, 0.502f, 1.0f }; // This is #0080FF in hex
	SetDecorationColor(blue);

	CreatePlaneNode();
	m_PlaneNode->SetColor(blue);

	ResetView();
}

void QRenderWidget::set3DView()
{
	m_Is3DView = true;

	m_RenderWindow->GetSliceNavigationController()->SetDefaultViewDirection(mitk::SliceNavigationController::Original);
	m_RenderWindow->SetLayoutIndex(mitk::BaseRenderer::ViewDirection::THREE_D);
	mitk::BaseRenderer::GetInstance(m_RenderWindow->GetRenderWindow())->SetMapperID(mitk::BaseRenderer::Standard3D);
	SetCornerAnnotationText("3D");

	float yellow[3] = { 1.0f, 1.0f, 0.0f }; // This is #0080FF in hex
	SetDecorationColor(yellow);

	CreatePlaneNode();

	ResetView();
}

void QRenderWidget::CreatePlaneNode()
{
//	mitk::IntProperty::Pointer layer;
//	m_PlaneNode = mitk::BaseRenderer::GetInstance(m_RenderWindow->GetRenderWindow())->GetCurrentWorldPlaneGeometryNode();
//	layer = mitk::IntProperty::New(1000);
//	m_PlaneNode->SetProperty("layer", layer);

	if(m_Is3DView)
	{
		// Is a 3D View!
		m_PlaneNode = mitk::DataNode::New();
		m_PlaneNode->SetProperty("name", mitk::StringProperty::New(m_WidgetName.toStdString() + ".plane"));
		m_PlaneNode->SetProperty("helper object", mitk::BoolProperty::New(true));
	}
	else
	{
		// Is a 2D View!
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
}

void QRenderWidget::InitGUI()
{
	m_Layout = new QHBoxLayout(this);
	m_Layout->setMargin(0);
	setLayout(m_Layout);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setContentsMargins(0, 0, 0, 0);

	if (nullptr == m_DataStorage)
	{
		return;
	}

	// create render window for this render window widget
	m_RenderWindow = new QmitkRenderWindow(this, m_WidgetName, nullptr);
	m_RenderWindow->GetSliceNavigationController()->SetCrosshairEvent.AddListener(mitk::MessageDelegate1<QRenderWidget, mitk::Point3D>(this, &QRenderWidget::SetCrosshair));
	m_RenderWindow->ActivateMenuWidget(true);

	connect(m_RenderWindow, &QVTKOpenGLWidget::mouseEvent, this, &QRenderWidget::MouseEvent);

	mitk::BaseRenderer::GetInstance(m_RenderWindow->GetRenderWindow())->SetDataStorage(m_DataStorage);

	m_Layout->addWidget(m_RenderWindow);

	// add point set as a crosshair
	m_PointSetNode = mitk::DataNode::New();
	m_PointSetNode->SetProperty("name", mitk::StringProperty::New("Crosshair of render window " + m_WidgetName.toStdString()));
	m_PointSetNode->SetProperty("helper object", mitk::BoolProperty::New(true)); // crosshair-node should typically be invisible

	// set the crosshair only visible for this specific renderer
	m_PointSetNode->SetBoolProperty("fixedLayer", true, m_RenderWindow->GetRenderer());
	m_PointSetNode->SetVisibility(true, m_RenderWindow->GetRenderer());
	m_PointSetNode->SetVisibility(false);

	m_PointSet = mitk::PointSet::New();
	m_PointSetNode->SetData(m_PointSet);

	// set colors and corner annotation
	InitDecorations();

	connect(m_RenderWindow, &QmitkRenderWindow::ResetView, this, &QRenderWidget::ResetView);
	connect(m_RenderWindow, &QmitkRenderWindow::CrosshairVisibilityChanged, this, &QRenderWidget::SetCrosshairVisibility);
	connect(m_RenderWindow, &QmitkRenderWindow::CrosshairRotationModeChanged, this, &QRenderWidget::SetWidgetPlaneMode);
}

void QRenderWidget::InitDecorations()
{
	vtkRenderer* vtkRenderer = m_RenderWindow->GetRenderer()->GetVtkRenderer();
	if (nullptr == vtkRenderer)
	{
		return;
	}

	// initialize background color gradients
	float black[3] = { 0.0f, 0.0f, 0.0f };
	SetGradientBackgroundColors(black, black);

	// initialize decoration color, rectangle and annotation text
	float white[3] = { 1.0f, 1.0f, 1.0f };
	m_DecorationColor = white;

	setFrameStyle(QFrame::Box | QFrame::Plain);
	QColor hexColor(m_DecorationColor[0] * 255, m_DecorationColor[1] * 255, m_DecorationColor[2] * 255);
	setStyleSheet("border: 2px solid " + hexColor.name(QColor::HexRgb));

	m_CornerAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
	m_CornerAnnotation->SetText(0, "Sagittal");
	m_CornerAnnotation->SetMaximumFontSize(12);
	m_CornerAnnotation->GetTextProperty()->SetColor(m_DecorationColor[0], m_DecorationColor[1], m_DecorationColor[2]);
	if (0 == vtkRenderer->HasViewProp(m_CornerAnnotation))
	{
		vtkRenderer->AddViewProp(m_CornerAnnotation);
	}
}

void QRenderWidget::InitDisplayActionEventHandling()
{
	m_DisplayActionEventBroadcast = mitk::DisplayActionEventBroadcast::New();
	m_DisplayActionEventBroadcast->LoadStateMachine("DisplayInteraction.xml");
	m_DisplayActionEventBroadcast->SetEventConfig("DisplayConfigPACS.xml");

	m_DisplayActionEventHandler = std::move(std::make_unique<mitk::DisplayActionEventHandlerStd>());
	m_DisplayActionEventHandler->SetObservableBroadcast(m_DisplayActionEventBroadcast);
	m_DisplayActionEventHandler->InitActions();

	SetInteractionScheme(mitk::InteractionSchemeSwitcher::MITKStandard);
}


void QRenderWidget::SetCrosshair(mitk::Point3D selectedPoint)
{
	m_PointSet->SetPoint(1, selectedPoint, 0);
	RequestUpdate();
}

void QRenderWidget::showEvent(QShowEvent *e)
{
	Q_UNUSED(e)

	if(m_Is3DView)
		set3DView();

	ResetView();
}

void QRenderWidget::hideEvent(QHideEvent* e)
{
	Q_UNUSED(e)

	std::cout << m_WidgetName.toStdString() << " hide event activated." << endl;
	if(m_Is3DView)
	{
		setAxialView();
		// setAxialView sets m_Is3DView, but should be true in this workaround.
		m_Is3DView = true;
		std::cout << m_WidgetName.toStdString() << " has been set as axial while hidden." << endl;
	}

	ResetView();
}

