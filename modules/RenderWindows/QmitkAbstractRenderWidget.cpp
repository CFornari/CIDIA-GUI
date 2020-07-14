#include "QmitkAbstractRenderWidget.h"

#include <QmitkDataStorageTreeModel.h>
#include <mitkDataStorageSelection.h>
#include <mitkMessage.h>

// vtk
#include <vtkCornerAnnotation.h>
#include <vtkTextProperty.h>

#include <mitkDisplayActionEventHandlerStd.h>
#include <mitkCameraController.h>

#include <QMouseEvent>

QmitkAbstractRenderWidget::QmitkAbstractRenderWidget(QWidget* parent,
														 const QString& widgetName)
	: QFrame(parent),
		m_WidgetName(widgetName),
		m_DataStorage(mitk::StandaloneDataStorage::New()),
		m_DataStorageTree(nullptr),
		m_RenderWindow(nullptr),
		m_PointSetNode(nullptr),
		m_PointSet(nullptr)
{
	InitGUI();

	InitDisplayActionEventHandling();
}

QmitkAbstractRenderWidget::~QmitkAbstractRenderWidget()
{

}

void QmitkAbstractRenderWidget::SetDataStorage(mitk::DataStorage* dataStorage)
{
	if(dataStorage == m_DataStorage)
		return;

	m_DataStorage = dataStorage;

	if (nullptr != m_RenderWindow)
	{
		mitk::BaseRenderer::GetInstance(m_RenderWindow->GetRenderWindow())->SetDataStorage(m_DataStorage);
	}

	m_TimeNavigationController = mitk::RenderingManager::GetInstance()->GetTimeNavigationController();
	m_TimeNavigationController->ConnectGeometryTimeEvent(m_RenderWindow->GetSliceNavigationController(), false);
	m_RenderWindow->GetSliceNavigationController()->ConnectGeometryTimeEvent(m_TimeNavigationController, false);
}

mitk::SliceNavigationController* QmitkAbstractRenderWidget::GetSliceNavigationController() const
{
	return m_RenderWindow->GetSliceNavigationController();
}

void QmitkAbstractRenderWidget::RequestUpdate()
{
	mitk::RenderingManager::GetInstance()->RequestUpdate(m_RenderWindow->GetRenderWindow());
}

void QmitkAbstractRenderWidget::ForceImmediateUpdate()
{
	mitk::RenderingManager::GetInstance()->ForceImmediateUpdate(m_RenderWindow->GetRenderWindow());
}

void QmitkAbstractRenderWidget::SetGradientBackgroundColors(const mitk::Color& upper, const mitk::Color& lower)
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

void QmitkAbstractRenderWidget::ShowGradientBackground(bool show)
{
	m_RenderWindow->GetRenderer()->GetVtkRenderer()->SetGradientBackground(show);
}

bool QmitkAbstractRenderWidget::IsGradientBackgroundOn() const
{
	return m_RenderWindow->GetRenderer()->GetVtkRenderer()->GetGradientBackground();
}

void QmitkAbstractRenderWidget::SetDecorationColor(const mitk::Color& color)
{
	m_DecorationColor = color;
	m_CornerAnnotation->GetTextProperty()->SetColor(m_DecorationColor[0], m_DecorationColor[1], m_DecorationColor[2]);

	QColor hexColor(m_DecorationColor[0] * 255, m_DecorationColor[1] * 255, m_DecorationColor[2] * 255);
	setStyleSheet("QmitkAbstractRenderWidget { border: 2px solid " + hexColor.name(QColor::HexRgb) + "; }");
}

void QmitkAbstractRenderWidget::ShowColoredRectangle(bool show)
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

bool QmitkAbstractRenderWidget::IsColoredRectangleVisible() const
{
	return frameStyle() > 0;
}

void QmitkAbstractRenderWidget::ShowCornerAnnotation(bool show)
{
	m_CornerAnnotation->SetVisibility(show);
}

bool QmitkAbstractRenderWidget::IsCornerAnnotationVisible() const
{
	return m_CornerAnnotation->GetVisibility() > 0;
}

void QmitkAbstractRenderWidget::SetCornerAnnotationText(const std::string& cornerAnnotation)
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

std::string QmitkAbstractRenderWidget::GetCornerAnnotationText() const
{
	return std::string(m_CornerAnnotation->GetText(0));
}

bool QmitkAbstractRenderWidget::IsRenderWindowMenuActivated() const
{
	return m_RenderWindow->GetActivateMenuWidgetFlag();
}

void QmitkAbstractRenderWidget::ActivateCrosshair(bool activate)
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

void QmitkAbstractRenderWidget::SetCrosshairVisibility(bool visible)
{
	if (m_PlaneNode.IsNotNull())
	{
		m_PlaneNode->SetVisibility(visible);
	}

	RequestUpdate();
}

bool QmitkAbstractRenderWidget::GetCrosshairVisibility() const
{
	if (m_PlaneNode.IsNotNull())
	{
		return m_PlaneNode->IsVisible(mitk::BaseRenderer::GetInstance(m_RenderWindow->GetRenderWindow()));
	}
	return false;
}

void QmitkAbstractRenderWidget::SetWidgetPlaneMode(int mode)
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

void QmitkAbstractRenderWidget::SetInteractionScheme(mitk::InteractionSchemeSwitcher::InteractionScheme scheme)
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

void QmitkAbstractRenderWidget::ResetView()
{
	if(m_DataStorage == nullptr)
	{
		std::cout << "QmitkAbstractRenderWidget::ResetView() called with datastorage == nullptr." << endl;
		return;
	}

	auto geo = m_DataStorage->ComputeBoundingGeometry3D(m_DataStorage->GetAll());
	mitk::RenderingManager::GetInstance()->InitializeView(m_RenderWindow->GetRenderWindow(), geo);

	mitk::BaseRenderer::GetInstance(m_RenderWindow->GetRenderWindow())->GetVtkRenderer()->ResetCamera();
	mitk::BaseRenderer::GetInstance(m_RenderWindow->GetRenderWindow())->GetCameraController()->Fit();
}

void QmitkAbstractRenderWidget::InitGUI()
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
	m_RenderWindow->GetSliceNavigationController()->SetCrosshairEvent.AddListener(mitk::MessageDelegate1<QmitkAbstractRenderWidget, mitk::Point3D>(this, &QmitkAbstractRenderWidget::SetCrosshair));
	m_RenderWindow->ActivateMenuWidget(true);

	connect(m_RenderWindow, &QVTKOpenGLWidget::mouseEvent, this, &QmitkAbstractRenderWidget::MouseEvent);

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

	connect(m_RenderWindow, &QmitkRenderWindow::ResetView, this, &QmitkAbstractRenderWidget::ResetView);
	connect(m_RenderWindow, &QmitkRenderWindow::CrosshairVisibilityChanged, this, &QmitkAbstractRenderWidget::SetCrosshairVisibility);
	connect(m_RenderWindow, &QmitkRenderWindow::CrosshairRotationModeChanged, this, &QmitkAbstractRenderWidget::SetWidgetPlaneMode);
}

void QmitkAbstractRenderWidget::InitDecorations()
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

void QmitkAbstractRenderWidget::InitDisplayActionEventHandling()
{
	m_DisplayActionEventBroadcast = mitk::DisplayActionEventBroadcast::New();
	m_DisplayActionEventBroadcast->LoadStateMachine("DisplayInteraction.xml");
	m_DisplayActionEventBroadcast->SetEventConfig("DisplayConfigPACS.xml");

	m_DisplayActionEventHandler = std::move(std::make_unique<mitk::DisplayActionEventHandlerStd>());
	m_DisplayActionEventHandler->SetObservableBroadcast(m_DisplayActionEventBroadcast);
	m_DisplayActionEventHandler->InitActions();

	SetInteractionScheme(mitk::InteractionSchemeSwitcher::MITKStandard);
}

void QmitkAbstractRenderWidget::SetCrosshair(mitk::Point3D selectedPoint)
{
	m_PointSet->SetPoint(1, selectedPoint, 0);
	RequestUpdate();
}

void QmitkAbstractRenderWidget::showEvent(QShowEvent *e)
{
	Q_UNUSED(e)

	ResetView();
}

void QmitkAbstractRenderWidget::hideEvent(QHideEvent* e)
{
	Q_UNUSED(e)

//	std::cout << m_WidgetName.toStdString() << " hide event activated." << endl;

	ResetView();
}

