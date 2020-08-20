/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#define US_MODULE_NAME "MultiLabelSegmentation"

#include "QmitkMultiLabelSegmentationView.h"
#include "ui_QmitkMultiLabelSegmentationView.h"

// blueberry
#include <berryConstants.h>
#include <berryIWorkbenchPage.h>
#include <berryIPreferences.h>

// mitk
#include "mitkApplicationCursor.h"
#include "mitkLabelSetImage.h"
#include "mitkStatusBar.h"
#include "mitkToolManagerProvider.h"
//#include "mitkSegmentationObjectFactory.h"
#include "mitkInteractionEventObserver.h"
#include "mitkPlanePositionManager.h"
#include "mitkSegTool2D.h"
#include <mitkDICOMSegmentationPropertyHelper.h>

// mitk DICOM
#include <mitkAnatomicalStructureColorPresets.h>
#include <mitkDICOMSegmentationConstants.h>
#include <mitkDICOMTag.h>
#include <mitkIDICOMTagsOfInterest.h>
#include <mitkLabel.h>
#include <mitkPropertyList.h>
#include <mitkPropertyNameHelper.h>
#include <mitkTemporoSpatialStringProperty.h>

// Qmitk
#include "QmitkNewSegmentationDialog.h"
#include "QmitkRenderWindow.h"
#include "QmitkSegmentationOrganNamesHandling.cpp"
#include <QmitkStdMultiWidget.h>

// us
#include <usGetModuleContext.h>
#include <usModule.h>
#include <usModuleContext.h>
#include <usModuleResource.h>
#include <usModuleResourceStream.h>

#include <vtkSmartPointer.h>

// Qt
#include <QDateTime>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QComboBox>

#include "tinyxml.h"

#include <itksys/SystemTools.hxx>

#include <regex>

namespace mitk
{


	static void SetReferenceDICOMProperty(PropertyList *referencedPropertyList,
																				PropertyList *propertyList,
																				const DICOMTag &tag,
																				const std::string &defaultString = "")
	{
		std::string tagString = GeneratePropertyNameForDICOMTag(tag.GetGroup(), tag.GetElement());

		// Get DICOM property from referenced image
		BaseProperty::Pointer originalProperty = referencedPropertyList->GetProperty(tagString.c_str());

		// if property exists, copy the informtaion to the segmentation
		if (originalProperty.IsNotNull())
			propertyList->SetProperty(tagString.c_str(), originalProperty);
		else // use the default value, if there is one
		{
			if (!defaultString.empty())
				propertyList->SetProperty(tagString.c_str(), TemporoSpatialStringProperty::New(defaultString).GetPointer());
		}
	}

	static PropertyList::Pointer GetDICOMSegmentationProperties(PropertyList *referencedPropertyList)
	{
		PropertyList::Pointer propertyList = PropertyList::New();

		// Add DICOM Tag (0008, 0060) Modality "SEG"
		propertyList->SetProperty(GeneratePropertyNameForDICOMTag(0x0008, 0x0060).c_str(),
															TemporoSpatialStringProperty::New("SEG"));
		// Add DICOM Tag (0008,103E) Series Description
		propertyList->SetProperty(GeneratePropertyNameForDICOMTag(0x0008, 0x103E).c_str(),
															TemporoSpatialStringProperty::New("MITK Segmentation"));
		// Add DICOM Tag (0070,0084) Content Creator Name
		propertyList->SetProperty(GeneratePropertyNameForDICOMTag(0x0070, 0x0084).c_str(),
															TemporoSpatialStringProperty::New("MITK"));
		// Add DICOM Tag (0012, 0071) Clinical Trial Series ID
		propertyList->SetProperty(GeneratePropertyNameForDICOMTag(0x0012, 0x0071).c_str(),
															TemporoSpatialStringProperty::New("Session 1"));
		// Add DICOM Tag (0012,0050) Clinical Trial Time Point ID
		propertyList->SetProperty(GeneratePropertyNameForDICOMTag(0x0012, 0x0050).c_str(),
															TemporoSpatialStringProperty::New("0"));
		// Add DICOM Tag (0012, 0060) Clinical Trial Coordinating Center Name
		propertyList->SetProperty(GeneratePropertyNameForDICOMTag(0x0012, 0x0060).c_str(),
															TemporoSpatialStringProperty::New("Unknown"));

		// Check if original image is a DICOM image; if so, store relevant DICOM Tags into the PropertyList of new
		// segmentation image
		bool parentIsDICOM = false;

		for (const auto &element : *(referencedPropertyList->GetMap()))
		{
			if (element.first.find("DICOM") == 0)
			{
				parentIsDICOM = true;
				break;
			}
		}

		if (!parentIsDICOM)
			return propertyList;

		//====== Patient information ======

		// Add DICOM Tag (0010,0010) patient's name; default "No Name"
		SetReferenceDICOMProperty(referencedPropertyList, propertyList, DICOMTag(0x0010, 0x0010), "NO NAME");
		// Add DICOM Tag (0010,0020) patient id; default "No Name"
		SetReferenceDICOMProperty(referencedPropertyList, propertyList, DICOMTag(0x0010, 0x0020), "NO NAME");
		// Add DICOM Tag (0010,0030) patient's birth date; no default
		SetReferenceDICOMProperty(referencedPropertyList, propertyList, DICOMTag(0x0010, 0x0030));
		// Add DICOM Tag (0010,0040) patient's sex; default "U" (Unknown)
		SetReferenceDICOMProperty(referencedPropertyList, propertyList, DICOMTag(0x0010, 0x0040), "U");

		//====== General study ======

		// Add DICOM Tag (0020,000D) Study Instance UID; no default --> MANDATORY!
		SetReferenceDICOMProperty(referencedPropertyList, propertyList, DICOMTag(0x0020, 0x000D));
		// Add DICOM Tag (0080,0020) Study Date; no default (think about "today")
		SetReferenceDICOMProperty(referencedPropertyList, propertyList, DICOMTag(0x0080, 0x0020));
		// Add DICOM Tag (0008,0050) Accession Number; no default
		SetReferenceDICOMProperty(referencedPropertyList, propertyList, DICOMTag(0x0008, 0x0050));
		// Add DICOM Tag (0008,1030) Study Description; no default
		SetReferenceDICOMProperty(referencedPropertyList, propertyList, DICOMTag(0x0008, 0x1030));

		//====== Reference DICOM data ======

		// Add reference file paths to referenced DICOM data
		BaseProperty::Pointer dcmFilesProp = referencedPropertyList->GetProperty("files");
		if (dcmFilesProp.IsNotNull())
			propertyList->SetProperty("referenceFiles", dcmFilesProp);

		return propertyList;
	}
}

QmitkMultiLabelSegmentationView::QmitkMultiLabelSegmentationView(QWidget* parent,
																																 QmitkStdMultiWidget* multiWidget,
																																 mitk::StandaloneDataStorage::Pointer dataStorage)
	: QWidget(parent),
		m_Parent(parent),
		m_DataStorage(dataStorage),
		m_Controls(new Ui::QmitkMultiLabelSegmentationView),
		m_MultiWidget(multiWidget),
    m_ToolManager(nullptr),
    m_ReferenceNode(nullptr),
    m_WorkingNode(nullptr),
    m_AutoSelectionEnabled(false),
    m_MouseCursorSet(false)
{
	Q_INIT_RESOURCE(segmentation);

  m_SegmentationPredicate = mitk::NodePredicateAnd::New();
  m_SegmentationPredicate->AddPredicate(mitk::TNodePredicateDataType<mitk::LabelSetImage>::New());
  m_SegmentationPredicate->AddPredicate(mitk::NodePredicateNot::New(mitk::NodePredicateProperty::New("helper object")));

  mitk::TNodePredicateDataType<mitk::Image>::Pointer isImage = mitk::TNodePredicateDataType<mitk::Image>::New();
  mitk::NodePredicateProperty::Pointer isBinary =
    mitk::NodePredicateProperty::New("binary", mitk::BoolProperty::New(true));
  mitk::NodePredicateAnd::Pointer isMask = mitk::NodePredicateAnd::New(isBinary, isImage);

  mitk::NodePredicateDataType::Pointer isDwi = mitk::NodePredicateDataType::New("DiffusionImage");
  mitk::NodePredicateDataType::Pointer isDti = mitk::NodePredicateDataType::New("TensorImage");
  mitk::NodePredicateDataType::Pointer isOdf = mitk::NodePredicateDataType::New("OdfImage");
  auto isSegment = mitk::NodePredicateDataType::New("Segment");

  mitk::NodePredicateOr::Pointer validImages = mitk::NodePredicateOr::New();
  validImages->AddPredicate(mitk::NodePredicateAnd::New(isImage, mitk::NodePredicateNot::New(isSegment)));
  validImages->AddPredicate(isDwi);
  validImages->AddPredicate(isDti);
  validImages->AddPredicate(isOdf);

  m_ReferencePredicate = mitk::NodePredicateAnd::New();
  m_ReferencePredicate->AddPredicate(validImages);
  m_ReferencePredicate->AddPredicate(mitk::NodePredicateNot::New(m_SegmentationPredicate));
  m_ReferencePredicate->AddPredicate(mitk::NodePredicateNot::New(isMask));
  m_ReferencePredicate->AddPredicate(mitk::NodePredicateNot::New(mitk::NodePredicateProperty::New("helper object")));
	CreateQtPartControl(parent);
}

QmitkMultiLabelSegmentationView::~QmitkMultiLabelSegmentationView()
{
  // m_ToolManager->ActivateTool(-1);
  /*
  todo: check this
	m_Controls->m_SliceBasedInterpolatorWidget->EnableInterpolation(false);
  ctkPluginContext* context = mitk::PluginActivator::getContext();
  ctkServiceReference ppmRef = context->getServiceReference<mitk::PlanePositionManagerService>();
  mitk::PlanePositionManagerService* service = context->getService<mitk::PlanePositionManagerService>(ppmRef);
  service->RemoveAllPlanePositions();
  context->ungetService(ppmRef);
  */
  // m_ToolManager->SetReferenceData(nullptr);
  // m_ToolManager->SetWorkingData(nullptr);

  // m_ServiceRegistration.Unregister();

  // Loose LabelSetConnections
  OnLooseLabelSetConnection();
}

void QmitkMultiLabelSegmentationView::CreateQtPartControl(QWidget *parent)
{
	m_Controls->setupUi(parent);

  // *------------------------
  // * DATA SELECTION WIDGETS
  // *------------------------

	m_Controls->m_cbReferenceNodeSelector->SetAutoSelectNewItems(true);
	m_Controls->m_cbReferenceNodeSelector->SetPredicate(m_ReferencePredicate);
	m_Controls->m_cbReferenceNodeSelector->SetDataStorage(m_DataStorage);

	m_Controls->m_cbWorkingNodeSelector->SetAutoSelectNewItems(true);
	m_Controls->m_cbWorkingNodeSelector->SetPredicate(m_SegmentationPredicate);
	m_Controls->m_cbWorkingNodeSelector->SetDataStorage(m_DataStorage);

	connect(m_Controls->m_cbReferenceNodeSelector,
					&QmitkDataStorageComboBox::OnSelectionChanged,
          this,
					&QmitkMultiLabelSegmentationView::OnReferenceSelectionChanged);

	connect(m_Controls->m_cbWorkingNodeSelector,
					&QmitkDataStorageComboBox::OnSelectionChanged,
          this,
					&QmitkMultiLabelSegmentationView::OnSegmentationSelectionChanged);

  // *------------------------
  // * ToolManager
  // *------------------------

  m_ToolManager = mitk::ToolManagerProvider::GetInstance()->GetToolManager();
  assert(m_ToolManager);
	m_ToolManager->SetDataStorage(*(m_DataStorage));
  m_ToolManager->InitializeTools();
  // use the same ToolManager instance for our 3D Tools
	m_Controls->m_ManualToolSelectionBox3D->SetToolManager(*m_ToolManager);

  // *------------------------
  // * LabelSetWidget
  // *------------------------

	m_Controls->m_LabelSetWidget->SetDataStorage(m_DataStorage);
	m_Controls->m_LabelSetWidget->SetOrganColors(mitk::OrganNamesHandling::GetDefaultOrganColorString());
	m_Controls->m_LabelSetWidget->hide();

  // *------------------------
  // * Interpolation
  // *------------------------

	m_Controls->m_SurfaceBasedInterpolatorWidget->SetDataStorage(*(m_DataStorage));
	m_Controls->m_SliceBasedInterpolatorWidget->SetDataStorage(*(m_DataStorage));
	connect(m_Controls->m_cbInterpolation, SIGNAL(activated(int)),
					this, SLOT(OnInterpolationSelectionChanged(int)));

	m_Controls->m_cbInterpolation->setCurrentIndex(0);
	m_Controls->m_swInterpolation->hide();

  QString segTools2D = tr("Add Subtract Fill Erase Paint Wipe 'Region Growing' FastMarching2D Correction 'Live Wire'");
  QString segTools3D = tr("Threshold 'Two Thresholds' 'Auto Threshold' 'Multiple Otsu'");

  std::regex extSegTool2DRegEx("SegTool2D$");
  std::regex extSegTool3DRegEx("SegTool3D$");

  auto tools = m_ToolManager->GetTools();

  for (const auto &tool : tools)
  {
    if (std::regex_search(tool->GetNameOfClass(), extSegTool2DRegEx))
    {
      segTools2D.append(QString(" '%1'").arg(tool->GetName()));
    }
    else if (std::regex_search(tool->GetNameOfClass(), extSegTool3DRegEx))
    {
      segTools3D.append(QString(" '%1'").arg(tool->GetName()));
    }
  }

  // *------------------------
  // * ToolSelection 2D
  // *------------------------

	m_Controls->m_ManualToolSelectionBox2D->SetGenerateAccelerators(true);
	m_Controls->m_ManualToolSelectionBox2D->SetToolGUIArea(m_Controls->m_ManualToolGUIContainer2D);
	m_Controls->m_ManualToolSelectionBox2D->SetDisplayedToolGroups(segTools2D.toStdString()); // todo: "Correction
  // 'Live Wire'"
	m_Controls->m_ManualToolSelectionBox2D->SetEnabledMode(
    QmitkToolSelectionBox::EnabledWithReferenceAndWorkingDataVisible);
	connect(m_Controls->m_ManualToolSelectionBox2D, &QmitkToolSelectionBox::ToolSelected,
					this, &QmitkMultiLabelSegmentationView::OnManualTool2DSelected);

  // *------------------------
  // * ToolSelection 3D
  // *------------------------

	m_Controls->m_ManualToolSelectionBox3D->SetGenerateAccelerators(true);
	m_Controls->m_ManualToolSelectionBox3D->SetToolGUIArea(m_Controls->m_ManualToolGUIContainer3D);
	m_Controls->m_ManualToolSelectionBox3D->SetDisplayedToolGroups(segTools3D.toStdString()); // todo add : FastMarching3D RegionGrowing Watershed
	m_Controls->m_ManualToolSelectionBox3D->SetLayoutColumns(2);
	m_Controls->m_ManualToolSelectionBox3D->SetEnabledMode(
    QmitkToolSelectionBox::EnabledWithReferenceAndWorkingDataVisible);

  // *------------------------*
  // * Connect PushButtons (pb)
  // *------------------------*

	connect(m_Controls->m_pbNewLabel, &QPushButton::clicked,
					this, &QmitkMultiLabelSegmentationView::OnNewLabel);
	connect(m_Controls->m_pbNewSegmentationSession,&QPushButton::clicked,
					this, &QmitkMultiLabelSegmentationView::OnNewSegmentationSession);
	connect(m_Controls->m_pbShowLabelTable, &QPushButton::toggled,
					this, &QmitkMultiLabelSegmentationView::OnShowLabelTable);

  // *------------------------*
  // * Connect LabelSetWidget
  // *------------------------*

	connect(m_Controls->m_LabelSetWidget, &QmitkLabelSetWidget::goToLabel,
					this, &QmitkMultiLabelSegmentationView::OnGoToLabel);

	connect(m_Controls->m_LabelSetWidget, &QmitkLabelSetWidget::resetView,
					this, &QmitkMultiLabelSegmentationView::OnResetView);

  // *------------------------*
  // * DATA SLECTION WIDGET
  // *------------------------*
	if (m_MultiWidget)
  {
    QList<mitk::SliceNavigationController *> controllers;
		controllers.push_back(m_MultiWidget->GetRenderWindow1()->GetSliceNavigationController());
		controllers.push_back(m_MultiWidget->GetRenderWindow2()->GetSliceNavigationController());
		controllers.push_back(m_MultiWidget->GetRenderWindow3()->GetSliceNavigationController());
		m_Controls->m_SliceBasedInterpolatorWidget->SetSliceNavigationControllers(controllers);
		//    m_Controls->m_LabelSetWidget->SetRenderWindowPart(this->m_IRenderWindowPart);
  }

  //  this->InitializeListeners();

	connect(m_Controls->m_btAddLayer, &QPushButton::clicked, this, &QmitkMultiLabelSegmentationView::OnAddLayer);
	connect(m_Controls->m_btDeleteLayer, &QPushButton::clicked, this, &QmitkMultiLabelSegmentationView::OnDeleteLayer);
	connect(m_Controls->m_btPreviousLayer, &QPushButton::clicked, this, &QmitkMultiLabelSegmentationView::OnPreviousLayer);
	connect(m_Controls->m_btNextLayer, &QPushButton::clicked, this, &QmitkMultiLabelSegmentationView::OnNextLayer);
	connect(m_Controls->m_btLockExterior, &QPushButton::toggled, this, &QmitkMultiLabelSegmentationView::OnLockExteriorToggled);
	connect(m_Controls->m_cbActiveLayer, SIGNAL(currentIndexChanged(int)),
					this, SLOT(OnChangeLayer(int)));
	
	m_Controls->m_btAddLayer->setEnabled(false);
	m_Controls->m_btDeleteLayer->setEnabled(false);
	m_Controls->m_btNextLayer->setEnabled(false);
	m_Controls->m_btPreviousLayer->setEnabled(false);
	m_Controls->m_cbActiveLayer->setEnabled(false);

	m_Controls->m_pbNewLabel->setEnabled(false);
	m_Controls->m_btLockExterior->setEnabled(false);
	m_Controls->m_pbShowLabelTable->setEnabled(false);

  // Make sure the GUI notices if appropriate data is already present on creation
	this->OnReferenceSelectionChanged(m_Controls->m_cbReferenceNodeSelector->GetSelectedNode());
	this->OnSegmentationSelectionChanged(m_Controls->m_cbWorkingNodeSelector->GetSelectedNode());
}

void QmitkMultiLabelSegmentationView::Activated()
{
	m_ToolManager->SetReferenceData(m_Controls->m_cbReferenceNodeSelector->GetSelectedNode());
	m_ToolManager->SetWorkingData(m_Controls->m_cbWorkingNodeSelector->GetSelectedNode());
}

void QmitkMultiLabelSegmentationView::Deactivated()
{
  // Not yet implemented
}

void QmitkMultiLabelSegmentationView::Visible()
{
  // Not yet implemented
}

void QmitkMultiLabelSegmentationView::Hidden()
{
  // Not yet implemented
}

int QmitkMultiLabelSegmentationView::GetSizeFlags(bool width)
{
  if (!width)
  {
    return berry::Constants::MIN | berry::Constants::MAX | berry::Constants::FILL;
  }
  else
  {
    return 0;
  }
}

int QmitkMultiLabelSegmentationView::ComputePreferredSize(bool width,
                                                          int /*availableParallel*/,
                                                          int /*availablePerpendicular*/,
                                                          int preferredResult)
{
  if (width == false)
  {
    return 100;
  }
  else
  {
    return preferredResult;
  }
}

/************************************************************************/
/* protected slots                                                      */
/************************************************************************/
void QmitkMultiLabelSegmentationView::OnManualTool2DSelected(int id)
{
  this->ResetMouseCursor();
  mitk::StatusBar::GetInstance()->DisplayText("");

  if (id >= 0)
  {
    std::string text = "Active Tool: \"";
    text += m_ToolManager->GetToolById(id)->GetName();
    text += "\"";
    mitk::StatusBar::GetInstance()->DisplayText(text.c_str());

    us::ModuleResource resource = m_ToolManager->GetToolById(id)->GetCursorIconResource();
    this->SetMouseCursor(resource, 0, 0);
  }
}

void QmitkMultiLabelSegmentationView::OnNewLabel()
{
  m_ToolManager->ActivateTool(-1);

  mitk::DataNode* workingNode = m_ToolManager->GetWorkingData(0);
  if (!workingNode)
  {
    QMessageBox::information(
      m_Parent, "New Segmentation Session", "Please load and select a patient image before starting some action.");
    return;
  }

  mitk::LabelSetImage* workingImage = dynamic_cast<mitk::LabelSetImage*>(workingNode->GetData());
  if (!workingImage)
  {
    QMessageBox::information(
      m_Parent, "New Segmentation Session", "Please load and select a patient image before starting some action.");
    return;
  }

  QmitkNewSegmentationDialog* dialog = new QmitkNewSegmentationDialog(m_Parent);
  dialog->SetSuggestionList(mitk::OrganNamesHandling::GetDefaultOrganColorString());
  dialog->setWindowTitle("New Label");

  int dialogReturnValue = dialog->exec();
  if (dialogReturnValue == QDialog::Rejected)
  {
    return;
  }

  QString segName = dialog->GetSegmentationName();
  if (segName.isEmpty())
  {
    segName = "Unnamed";
  }
  workingImage->GetActiveLabelSet()->AddLabel(segName.toStdString(), dialog->GetColor());

  UpdateControls();
	m_Controls->m_LabelSetWidget->ResetAllTableWidgetItems();

  mitk::RenderingManager::GetInstance()->InitializeViews(workingNode->GetData()->GetTimeGeometry(), mitk::RenderingManager::REQUEST_UPDATE_ALL, true);
}

void QmitkMultiLabelSegmentationView::OnShowLabelTable(bool value)
{
  if (value)
		m_Controls->m_LabelSetWidget->show();
  else
		m_Controls->m_LabelSetWidget->hide();
}

void QmitkMultiLabelSegmentationView::OnNewSegmentationSession()
{
	mitk::DataNode *referenceNode = m_Controls->m_cbReferenceNodeSelector->GetSelectedNode();

  if (!referenceNode)
  {
    QMessageBox::information(
      m_Parent, "New Segmentation Session", "Please load and select a patient image before starting some action.");
    return;
  }

  m_ToolManager->ActivateTool(-1);

  mitk::Image* referenceImage = dynamic_cast<mitk::Image*>(referenceNode->GetData());
  assert(referenceImage);

  QString newName = QString::fromStdString(referenceNode->GetName());
  newName.append("-labels");

  bool ok = false;
  newName = QInputDialog::getText(m_Parent, "New Segmentation Session", "New name:", QLineEdit::Normal, newName, &ok);

  if (!ok)
  {
    return;
  }
  this->WaitCursorOn();

  mitk::LabelSetImage::Pointer workingImage = mitk::LabelSetImage::New();
  try
  {
    workingImage->Initialize(referenceImage);
  }
  catch (mitk::Exception& e)
  {
    this->WaitCursorOff();
    MITK_ERROR << "Exception caught: " << e.GetDescription();
    QMessageBox::information(m_Parent, "New Segmentation Session", "Could not create a new segmentation session.\n");
    return;
  }

  this->WaitCursorOff();

  mitk::DataNode::Pointer workingNode = mitk::DataNode::New();
  workingNode->SetData(workingImage);
  workingNode->SetName(newName.toStdString());

  workingImage->GetExteriorLabel()->SetProperty("name.parent", mitk::StringProperty::New(referenceNode->GetName().c_str()));
  workingImage->GetExteriorLabel()->SetProperty("name.image", mitk::StringProperty::New(newName.toStdString().c_str()));
  // Set DICOM SEG properties for segmentation session
	mitk::PropertyList::Pointer dicomSegPropertyList = mitk::GetDICOMSegmentationProperties(referenceImage->GetPropertyList());
  workingImage->GetPropertyList()->ConcatenatePropertyList(dicomSegPropertyList);

	if (!m_DataStorage->Exists(workingNode))
  {
		m_DataStorage->Add(workingNode, referenceNode);
  }

  OnNewLabel();
}

void QmitkMultiLabelSegmentationView::OnGoToLabel(const mitk::Point3D& pos)
{
	if (m_MultiWidget)
		m_MultiWidget->SetSelectedPosition(pos, QString());
}

void QmitkMultiLabelSegmentationView::OnResetView()
{
	if (m_MultiWidget)
		m_MultiWidget->ForceImmediateUpdateAll();
}

void QmitkMultiLabelSegmentationView::OnAddLayer()
{
  m_ToolManager->ActivateTool(-1);

  mitk::DataNode* workingNode = m_ToolManager->GetWorkingData(0);
  assert(workingNode);

  mitk::LabelSetImage* workingImage = dynamic_cast<mitk::LabelSetImage*>(workingNode->GetData());
  assert(workingImage);

  QString question = "Do you really want to add a layer to the current segmentation session?";
  QMessageBox::StandardButton answerButton = QMessageBox::question(
		m_Controls->m_LabelSetWidget, "Add layer", question, QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Yes);

  if (answerButton != QMessageBox::Yes) return;

  try
  {
    WaitCursorOn();
    workingImage->AddLayer();
    WaitCursorOff();
  }
  catch ( mitk::Exception& e )
  {
    WaitCursorOff();
    MITK_ERROR << "Exception caught: " << e.GetDescription();
    QMessageBox::information(
			m_Controls->m_LabelSetWidget, "Add Layer", "Could not add a new layer. See error log for details.\n");
    return;
  }

  OnNewLabel();
}

void QmitkMultiLabelSegmentationView::OnDeleteLayer()
{
  m_ToolManager->ActivateTool(-1);

  mitk::DataNode* workingNode = m_ToolManager->GetWorkingData(0);
  assert(workingNode);

  mitk::LabelSetImage* workingImage = dynamic_cast<mitk::LabelSetImage*>(workingNode->GetData());
  assert(workingImage);

  if (workingImage->GetNumberOfLayers() < 2)
    return;

  QString question = "Do you really want to delete the current layer?";

  QMessageBox::StandardButton answerButton = QMessageBox::question(
		m_Controls->m_LabelSetWidget, "Delete layer", question, QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Yes);

  if (answerButton != QMessageBox::Yes)
  {
    return;
  }

  try
  {
    this->WaitCursorOn();
    workingImage->RemoveLayer();
    this->WaitCursorOff();
  }
  catch (mitk::Exception& e)
  {
    this->WaitCursorOff();
    MITK_ERROR << "Exception caught: " << e.GetDescription();
		QMessageBox::information(m_Controls->m_LabelSetWidget, "Delete Layer",
                             "Could not delete the currently active layer. See error log for details.\n");
    return;
  }

  UpdateControls();
	m_Controls->m_LabelSetWidget->ResetAllTableWidgetItems();
}

void QmitkMultiLabelSegmentationView::OnPreviousLayer()
{
  m_ToolManager->ActivateTool(-1);

  mitk::DataNode *workingNode = m_ToolManager->GetWorkingData(0);
  assert(workingNode);
  mitk::LabelSetImage *workingImage = dynamic_cast<mitk::LabelSetImage *>(workingNode->GetData());
  assert(workingImage);

  OnChangeLayer(workingImage->GetActiveLayer() - 1);
}

void QmitkMultiLabelSegmentationView::OnNextLayer()
{
  m_ToolManager->ActivateTool(-1);

  mitk::DataNode *workingNode = m_ToolManager->GetWorkingData(0);
  assert(workingNode);
  mitk::LabelSetImage *workingImage = dynamic_cast<mitk::LabelSetImage *>(workingNode->GetData());
  assert(workingImage);

  OnChangeLayer(workingImage->GetActiveLayer() + 1);
}

void QmitkMultiLabelSegmentationView::OnChangeLayer(int layer)
{
  m_ToolManager->ActivateTool(-1);

  mitk::DataNode *workingNode = m_ToolManager->GetWorkingData(0);
  assert(workingNode);

  mitk::LabelSetImage *workingImage = dynamic_cast<mitk::LabelSetImage *>(workingNode->GetData());
  assert(workingImage);

  this->WaitCursorOn();
  workingImage->SetActiveLayer(layer);
  this->WaitCursorOff();

  UpdateControls();
	m_Controls->m_LabelSetWidget->ResetAllTableWidgetItems();
}

void QmitkMultiLabelSegmentationView::OnDeactivateActiveTool()
{
  m_ToolManager->ActivateTool(-1);
}

void QmitkMultiLabelSegmentationView::OnLockExteriorToggled(bool checked)
{
  mitk::DataNode* workingNode = m_ToolManager->GetWorkingData(0);
  assert(workingNode);

  mitk::LabelSetImage* workingImage = dynamic_cast<mitk::LabelSetImage*>(workingNode->GetData());
  assert(workingImage);

  workingImage->GetLabel(0)->SetLocked(checked);
}

void QmitkMultiLabelSegmentationView::OnReferenceSelectionChanged(const mitk::DataNode* node)
{
  m_ToolManager->ActivateTool(-1);

  m_ReferenceNode = const_cast<mitk::DataNode*>(node);
  m_ToolManager->SetReferenceData(m_ReferenceNode);

  if (m_ReferenceNode.IsNotNull())
  {
    if (m_AutoSelectionEnabled)
    {
      // if an image is selected find a possible working / segmentation image
			mitk::DataStorage::SetOfObjects::ConstPointer derivations = m_DataStorage->GetDerivations(m_ReferenceNode, m_SegmentationPredicate);
      if (derivations->Size() != 0)
      {
        // use the first segmentation child node
        m_WorkingNode = derivations->ElementAt(0);
        m_ToolManager->SetWorkingData(m_WorkingNode);

				m_Controls->m_cbWorkingNodeSelector->blockSignals(true);
				m_Controls->m_cbWorkingNodeSelector->SetSelectedNode(m_WorkingNode);
				m_Controls->m_cbWorkingNodeSelector->blockSignals(false);
      }
      else if (derivations->size() == 0)
      {
				m_Controls->m_cbWorkingNodeSelector->setCurrentIndex(-1);
      }

      // hide all image and segmentation nodes to later show only the automatically selected ones
			mitk::DataStorage::SetOfObjects::ConstPointer patientNodes = m_DataStorage->GetSubset(m_ReferencePredicate);
      for (mitk::DataStorage::SetOfObjects::const_iterator iter = patientNodes->begin(); iter != patientNodes->end(); ++iter)
      {
        (*iter)->SetVisibility(false);
      }

			mitk::DataStorage::SetOfObjects::ConstPointer segmentationNodes = m_DataStorage->GetSubset(m_SegmentationPredicate);
      for (mitk::DataStorage::SetOfObjects::const_iterator iter = segmentationNodes->begin(); iter != segmentationNodes->end(); ++iter)
      {
        (*iter)->SetVisibility(false);
      }
    }
    m_ReferenceNode->SetVisibility(true);

    // check match of segmentation and reference image geometries
    if (m_WorkingNode.IsNotNull())
    {
      mitk::Image* workingImage = dynamic_cast<mitk::Image*>(m_WorkingNode->GetData());
      assert(workingImage);

      mitk::Image* referenceImage = dynamic_cast<mitk::Image*>(node->GetData());
      assert(referenceImage);

      if (!this->CheckForSameGeometry(referenceImage, workingImage))
      {
        return;
      }
      m_WorkingNode->SetVisibility(true);
    }
  }

  UpdateControls();
  if (m_WorkingNode.IsNotNull())
  {
		m_Controls->m_LabelSetWidget->ResetAllTableWidgetItems();
    mitk::RenderingManager::GetInstance()->InitializeViews(m_WorkingNode->GetData()->GetTimeGeometry(), mitk::RenderingManager::REQUEST_UPDATE_ALL, true);
  }
}

void QmitkMultiLabelSegmentationView::OnSegmentationSelectionChanged(const mitk::DataNode* node)
{
  m_ToolManager->ActivateTool(-1);

  if (m_WorkingNode.IsNotNull())
    OnLooseLabelSetConnection();

  m_WorkingNode = const_cast<mitk::DataNode*>(node);
  m_ToolManager->SetWorkingData(m_WorkingNode);
  if (m_WorkingNode.IsNotNull())
  {
    OnEstablishLabelSetConnection();

    if (m_AutoSelectionEnabled)
    {
      // if a segmentation is selected find a possible reference image
			mitk::DataStorage::SetOfObjects::ConstPointer sources = m_DataStorage->GetSources(m_WorkingNode, m_ReferencePredicate);
      if (sources->Size() != 0)
      {
        m_ReferenceNode = sources->ElementAt(0);
        m_ToolManager->SetReferenceData(m_ReferenceNode);

				m_Controls->m_cbReferenceNodeSelector->blockSignals(true);
				m_Controls->m_cbReferenceNodeSelector->SetSelectedNode(m_ReferenceNode);
				m_Controls->m_cbReferenceNodeSelector->blockSignals(false);
      }
      else if(sources->size() == 0)
      {
				m_Controls->m_cbReferenceNodeSelector->setCurrentIndex(-1);
      }

      // hide all image and segmentation nodes to later show only the automatically selected ones
			mitk::DataStorage::SetOfObjects::ConstPointer patientNodes = m_DataStorage->GetSubset(m_ReferencePredicate);
      for (mitk::DataStorage::SetOfObjects::const_iterator iter = patientNodes->begin(); iter != patientNodes->end(); ++iter)
      {
        (*iter)->SetVisibility(false);
      }

			mitk::DataStorage::SetOfObjects::ConstPointer segmentationNodes = m_DataStorage->GetSubset(m_SegmentationPredicate);
      for (mitk::DataStorage::SetOfObjects::const_iterator iter = segmentationNodes->begin(); iter != segmentationNodes->end(); ++iter)
      {
        (*iter)->SetVisibility(false);
      }
    }
    m_WorkingNode->SetVisibility(true);

    // check match of segmentation and reference image geometries
    if (m_ReferenceNode.IsNotNull())
    {
      mitk::Image* referenceImage = dynamic_cast<mitk::Image*>(m_ReferenceNode->GetData());
      assert(referenceImage);

      mitk::Image* workingImage = dynamic_cast<mitk::Image*>(m_WorkingNode->GetData());
      assert(workingImage);

      if (!this->CheckForSameGeometry(referenceImage, workingImage))
      {
        return;
      }
      m_ReferenceNode->SetVisibility(true);
    }
  }

  UpdateControls();
  if (m_WorkingNode.IsNotNull())
  {
		m_Controls->m_LabelSetWidget->ResetAllTableWidgetItems();
    mitk::RenderingManager::GetInstance()->InitializeViews(m_WorkingNode->GetData()->GetTimeGeometry(), mitk::RenderingManager::REQUEST_UPDATE_ALL, true);
  }
}

void QmitkMultiLabelSegmentationView::OnInterpolationSelectionChanged(int index)
{
  if (index == 1)
  {
		m_Controls->m_SurfaceBasedInterpolatorWidget->m_Controls.m_btStart->setChecked(false);//OnToggleWidgetActivation(false);
		m_Controls->m_swInterpolation->setCurrentIndex(0);
		m_Controls->m_swInterpolation->show();
  }
  else if (index == 2)
  {
		m_Controls->m_SliceBasedInterpolatorWidget->m_Controls.m_btStart->setChecked(false);
		m_Controls->m_swInterpolation->setCurrentIndex(1);
		m_Controls->m_swInterpolation->show();
  }
  else
  {
		m_Controls->m_SurfaceBasedInterpolatorWidget->m_Controls.m_btStart->setChecked(false);
		m_Controls->m_SliceBasedInterpolatorWidget->m_Controls.m_btStart->setChecked(false);
		m_Controls->m_swInterpolation->setCurrentIndex(2);
		m_Controls->m_swInterpolation->hide();
  }
}

/************************************************************************/
/* protected                                                            */
/************************************************************************/
void QmitkMultiLabelSegmentationView::OnSelectionChanged(berry::IWorkbenchPart::Pointer, const QList<mitk::DataNode::Pointer> &nodes)
{
  if (m_AutoSelectionEnabled)
  {
    // automatically set the reference node and the working node of the multi label plugin
    if (1 == nodes.size())
    {
      mitk::DataNode::Pointer selectedNode = nodes.at(0);
      if (selectedNode.IsNull())
      {
        return;
      }

      // check selected node
      mitk::LabelSetImage::Pointer labelSetImage = dynamic_cast<mitk::LabelSetImage*>(selectedNode->GetData());
      if (labelSetImage.IsNotNull())
      {
        // reset the image / reference node selector in case the current selected segmentation has no image parent
				m_Controls->m_cbReferenceNodeSelector->setCurrentIndex(-1);
        // selected a label set image (a segmentation ( working node)
				m_Controls->m_cbWorkingNodeSelector->SetSelectedNode(selectedNode);
        return;
      }

      mitk::Image::Pointer selectedImage = dynamic_cast<mitk::Image*>(selectedNode->GetData());
      if (selectedImage.IsNotNull())
      {
        // reset the segmentation / working node selector in case the current selected image has no segmentation child
				m_Controls->m_cbWorkingNodeSelector->setCurrentIndex(-1);
        // selected an image (a reference node)
				m_Controls->m_cbReferenceNodeSelector->SetSelectedNode(selectedNode);
        return;
      }
    }
  }
}

void QmitkMultiLabelSegmentationView::OnPreferencesChanged(const berry::IBerryPreferences* prefs)
{
  if (m_Parent && m_WorkingNode.IsNotNull())
  {
    m_AutoSelectionEnabled = prefs->GetBool("auto selection", false);

    mitk::BoolProperty::Pointer drawOutline = mitk::BoolProperty::New(prefs->GetBool("draw outline", true));
    mitk::BoolProperty::Pointer volumeRendering = mitk::BoolProperty::New(prefs->GetBool("volume rendering", false));
    mitk::LabelSetImage* labelSetImage;
    mitk::DataNode* segmentation;

    // iterate all segmentations (binary (single label) and LabelSetImages)
    mitk::NodePredicateProperty::Pointer isBinaryPredicate = mitk::NodePredicateProperty::New("binary", mitk::BoolProperty::New(true));
    mitk::NodePredicateOr::Pointer allSegmentationsPredicate = mitk::NodePredicateOr::New(isBinaryPredicate, m_SegmentationPredicate);
		mitk::DataStorage::SetOfObjects::ConstPointer allSegmentations = m_DataStorage->GetSubset(allSegmentationsPredicate);

    for (mitk::DataStorage::SetOfObjects::const_iterator it = allSegmentations->begin(); it != allSegmentations->end(); ++it)
    {
      segmentation = *it;
      labelSetImage = dynamic_cast<mitk::LabelSetImage*>(segmentation->GetData());
      if (nullptr != labelSetImage)
      {
        // segmentation node is a multi label segmentation
        segmentation->SetProperty("labelset.contour.active", drawOutline);
        //segmentation->SetProperty("opacity", mitk::FloatProperty::New(drawOutline->GetValue() ? 1.0f : 0.3f));
        segmentation->SetProperty("volumerendering", volumeRendering);
        // force render window update to show outline
        segmentation->GetData()->Modified();
      }
      else if (nullptr != segmentation->GetData())
      {
        // node is actually a 'single label' segmentation,
        // but its outline property can be set in the 'multi label' segmentation preference page as well
        bool isBinary = false;
        segmentation->GetBoolProperty("binary", isBinary);
        if (isBinary)
        {
          segmentation->SetProperty("outline binary", drawOutline);
          segmentation->SetProperty("outline width", mitk::FloatProperty::New(2.0));
          //segmentation->SetProperty("opacity", mitk::FloatProperty::New(drawOutline->GetValue() ? 1.0f : 0.3f));
          segmentation->SetProperty("volumerendering", volumeRendering);
          // force render window update to show outline
          segmentation->GetData()->Modified();
        }
      }
      else
      {
        // "interpolation feedback" data nodes have binary flag but don't have a data set. So skip them for now.
        MITK_INFO << "DataNode " << segmentation->GetName() << " doesn't contain a base data.";
      }
    }
  }
}

void QmitkMultiLabelSegmentationView::NodeAdded(const mitk::DataNode *)
{
  /*
  bool isHelperObject(false);
  node->GetBoolProperty("helper object", isHelperObject);
  if (isHelperObject) return;

  if (m_ReferenceNode.IsNotNull() && dynamic_cast<mitk::LabelSetImage*>(node->GetData()))
  {
  mitk::LabelSetImage* workingImage = dynamic_cast<mitk::LabelSetImage*>(node->GetData());

  if (workingImage->GetNumberOfLabels() > 2)
	m_Controls->m_LabelSetWidget->show();
  else
	m_Controls->m_LabelSetWidget->hide();
  }
  */
}

//void QmitkMultiLabelSegmentationView::NodeRemoved(const mitk::DataNode *node)
//{
//  bool isHelperObject(false);
//  node->GetBoolProperty("helper object", isHelperObject);
//  if (isHelperObject)
//  {
//    return;
//  }

//  if (m_ReferenceNode.IsNotNull() && dynamic_cast<mitk::LabelSetImage *>(node->GetData()))
//  {
//    // remove all possible contour markers of the segmentation
//		mitk::DataStorage::SetOfObjects::ConstPointer allContourMarkers = m_DataStorage->GetDerivations(
//      node, mitk::NodePredicateProperty::New("isContourMarker", mitk::BoolProperty::New(true)));

//    ctkPluginContext *context = mitk::PluginActivator::getContext();
//    ctkServiceReference ppmRef = context->getServiceReference<mitk::PlanePositionManagerService>();
//    mitk::PlanePositionManagerService *service = context->getService<mitk::PlanePositionManagerService>(ppmRef);

//    for (mitk::DataStorage::SetOfObjects::ConstIterator it = allContourMarkers->Begin(); it != allContourMarkers->End(); ++it)
//    {
//      std::string nodeName = node->GetName();
//      unsigned int t = nodeName.find_last_of(" ");
//      unsigned int id = atof(nodeName.substr(t + 1).c_str()) - 1;

//      service->RemovePlanePosition(id);

//			m_DataStorage->Remove(it->Value());
//    }

//    context->ungetService(ppmRef);
//    service = nullptr;
//  }
//}

void QmitkMultiLabelSegmentationView::OnEstablishLabelSetConnection()
{
  if (m_WorkingNode.IsNull())
  {
    return;
  }
  mitk::LabelSetImage *workingImage = dynamic_cast<mitk::LabelSetImage *>(m_WorkingNode->GetData());
  assert(workingImage);

  workingImage->GetActiveLabelSet()->AddLabelEvent += mitk::MessageDelegate<QmitkLabelSetWidget>(
		m_Controls->m_LabelSetWidget, &QmitkLabelSetWidget::ResetAllTableWidgetItems);
  workingImage->GetActiveLabelSet()->RemoveLabelEvent += mitk::MessageDelegate<QmitkLabelSetWidget>(
		m_Controls->m_LabelSetWidget, &QmitkLabelSetWidget::ResetAllTableWidgetItems);
  workingImage->GetActiveLabelSet()->ModifyLabelEvent += mitk::MessageDelegate<QmitkLabelSetWidget>(
		m_Controls->m_LabelSetWidget, &QmitkLabelSetWidget::UpdateAllTableWidgetItems);
  workingImage->GetActiveLabelSet()->AllLabelsModifiedEvent += mitk::MessageDelegate<QmitkLabelSetWidget>(
		m_Controls->m_LabelSetWidget, &QmitkLabelSetWidget::UpdateAllTableWidgetItems);
  workingImage->GetActiveLabelSet()->ActiveLabelEvent +=
		mitk::MessageDelegate1<QmitkLabelSetWidget, mitk::Label::PixelType>(m_Controls->m_LabelSetWidget,
                                                                        &QmitkLabelSetWidget::SelectLabelByPixelValue);
  workingImage->BeforeChangeLayerEvent += mitk::MessageDelegate<QmitkMultiLabelSegmentationView>(
    this, &QmitkMultiLabelSegmentationView::OnLooseLabelSetConnection);
}

void QmitkMultiLabelSegmentationView::OnLooseLabelSetConnection()
{
  if (m_WorkingNode.IsNull())
  {
    return;
  }
  mitk::LabelSetImage *workingImage = dynamic_cast<mitk::LabelSetImage *>(m_WorkingNode->GetData());
  assert(workingImage);

  // Reset LabelSetWidget Events
  workingImage->GetActiveLabelSet()->AddLabelEvent -= mitk::MessageDelegate<QmitkLabelSetWidget>(
		m_Controls->m_LabelSetWidget, &QmitkLabelSetWidget::ResetAllTableWidgetItems);
  workingImage->GetActiveLabelSet()->RemoveLabelEvent -= mitk::MessageDelegate<QmitkLabelSetWidget>(
		m_Controls->m_LabelSetWidget, &QmitkLabelSetWidget::ResetAllTableWidgetItems);
  workingImage->GetActiveLabelSet()->ModifyLabelEvent -= mitk::MessageDelegate<QmitkLabelSetWidget>(
		m_Controls->m_LabelSetWidget, &QmitkLabelSetWidget::UpdateAllTableWidgetItems);
  workingImage->GetActiveLabelSet()->AllLabelsModifiedEvent -= mitk::MessageDelegate<QmitkLabelSetWidget>(
		m_Controls->m_LabelSetWidget, &QmitkLabelSetWidget::UpdateAllTableWidgetItems);
  workingImage->GetActiveLabelSet()->ActiveLabelEvent -=
		mitk::MessageDelegate1<QmitkLabelSetWidget, mitk::Label::PixelType>(m_Controls->m_LabelSetWidget,
                                                                        &QmitkLabelSetWidget::SelectLabelByPixelValue);
  workingImage->BeforeChangeLayerEvent -= mitk::MessageDelegate<QmitkMultiLabelSegmentationView>(
    this, &QmitkMultiLabelSegmentationView::OnLooseLabelSetConnection);
}

void QmitkMultiLabelSegmentationView::SetFocus()
{
}

void QmitkMultiLabelSegmentationView::UpdateControls()
{
  mitk::DataNode* referenceNode = m_ToolManager->GetReferenceData(0);
  bool hasReferenceNode = referenceNode != nullptr;

  mitk::DataNode* workingNode = m_ToolManager->GetWorkingData(0);
  bool hasValidWorkingNode = workingNode != nullptr;

	m_Controls->m_pbNewLabel->setEnabled(false);
	m_Controls->m_gbInterpolation->setEnabled(false);
	m_Controls->m_SliceBasedInterpolatorWidget->setEnabled(false);
	m_Controls->m_SurfaceBasedInterpolatorWidget->setEnabled(false);
	m_Controls->m_LabelSetWidget->setEnabled(false);
	m_Controls->m_btAddLayer->setEnabled(false);
	m_Controls->m_btDeleteLayer->setEnabled(false);
	m_Controls->m_cbActiveLayer->setEnabled(false);
	m_Controls->m_btPreviousLayer->setEnabled(false);
	m_Controls->m_btNextLayer->setEnabled(false);
	m_Controls->m_btLockExterior->setChecked(false);
	m_Controls->m_btLockExterior->setEnabled(false);
	m_Controls->m_pbShowLabelTable->setChecked(false);
	m_Controls->m_pbShowLabelTable->setEnabled(false);

	m_Controls->m_ManualToolSelectionBox3D->SetEnabledMode(QmitkToolSelectionBox::EnabledWithReferenceAndWorkingDataVisible);
	m_Controls->m_ManualToolSelectionBox2D->SetEnabledMode(QmitkToolSelectionBox::EnabledWithReferenceAndWorkingDataVisible);

  if (hasValidWorkingNode)
  {
    // TODO adapt tool manager so that this check is done there, e.g. convenience function
    mitk::LabelSetImage* workingImage = dynamic_cast<mitk::LabelSetImage*>(workingNode->GetData());
    hasValidWorkingNode = workingImage != nullptr;
    if (hasValidWorkingNode)
    {
			m_Controls->m_pbNewLabel->setEnabled(true);
			m_Controls->m_btLockExterior->setEnabled(true);
			m_Controls->m_pbShowLabelTable->setEnabled(true);
			m_Controls->m_gbInterpolation->setEnabled(true);
			m_Controls->m_SliceBasedInterpolatorWidget->setEnabled(true);
			m_Controls->m_SurfaceBasedInterpolatorWidget->setEnabled(true);
			m_Controls->m_LabelSetWidget->setEnabled(true);
			m_Controls->m_btAddLayer->setEnabled(true);

      int activeLayer = workingImage->GetActiveLayer();
      int numberOfLayers = workingImage->GetNumberOfLayers();

			m_Controls->m_cbActiveLayer->blockSignals(true);
			m_Controls->m_cbActiveLayer->clear();
      for (unsigned int lidx = 0; lidx < workingImage->GetNumberOfLayers(); ++lidx)
      {
				m_Controls->m_cbActiveLayer->addItem(QString::number(lidx));
      }
			m_Controls->m_cbActiveLayer->setCurrentIndex(activeLayer);
			m_Controls->m_cbActiveLayer->blockSignals(false);

			m_Controls->m_cbActiveLayer->setEnabled(numberOfLayers > 1);
			m_Controls->m_btDeleteLayer->setEnabled(numberOfLayers > 1);
			m_Controls->m_btPreviousLayer->setEnabled(activeLayer > 0);
			m_Controls->m_btNextLayer->setEnabled(activeLayer != numberOfLayers - 1);

			m_Controls->m_btLockExterior->setChecked(workingImage->GetLabel(0, activeLayer)->GetLocked());
			m_Controls->m_pbShowLabelTable->setChecked(workingImage->GetNumberOfLabels() > 1 /*1st is exterior*/);

      //MLI TODO
			//m_Controls->m_ManualToolSelectionBox2D->SetEnabledMode(QmitkToolSelectionBox::EnabledWithWorkingDataVisible);
    }
  }

  if (hasValidWorkingNode && hasReferenceNode)
  {
    int layer = -1;
    referenceNode->GetIntProperty("layer", layer);
    workingNode->SetIntProperty("layer", layer + 1);
  }

	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void QmitkMultiLabelSegmentationView::WaitCursorOn()
{
	QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
}

void QmitkMultiLabelSegmentationView::WaitCursorOff()
{
	QApplication::restoreOverrideCursor();
}

//void QmitkMultiLabelSegmentationView::RenderWindowPartActivated(mitk::IRenderWindowPart* renderWindowPart)
//{
//  if (m_IRenderWindowPart != renderWindowPart)
//  {
//    m_IRenderWindowPart = renderWindowPart;
//    m_Parent->setEnabled(true);

//    QList<mitk::SliceNavigationController*> controllers;
//    controllers.push_back(m_IRenderWindowPart->GetQmitkRenderWindow("axial")->GetSliceNavigationController());
//    controllers.push_back(m_IRenderWindowPart->GetQmitkRenderWindow("sagittal")->GetSliceNavigationController());
//    controllers.push_back(m_IRenderWindowPart->GetQmitkRenderWindow("coronal")->GetSliceNavigationController());
//    m_Controls->m_SliceBasedInterpolatorWidget->SetSliceNavigationControllers(controllers);
//  }
//}

//void QmitkMultiLabelSegmentationView::RenderWindowPartDeactivated(mitk::IRenderWindowPart* /*renderWindowPart*/)
//{
//  m_ToolManager->ActivateTool(-1);
//  m_IRenderWindowPart = 0;
//  m_Parent->setEnabled(false);
//}

void QmitkMultiLabelSegmentationView::ResetMouseCursor()
{
  if (m_MouseCursorSet)
  {
    mitk::ApplicationCursor::GetInstance()->PopCursor();
    m_MouseCursorSet = false;
  }
}

void QmitkMultiLabelSegmentationView::SetMouseCursor(const us::ModuleResource resource, int hotspotX, int hotspotY)
{
  // Remove previously set mouse cursor
  if (m_MouseCursorSet)
    this->ResetMouseCursor();

  if (resource)
  {
    us::ModuleResourceStream cursor(resource, std::ios::binary);
    mitk::ApplicationCursor::GetInstance()->PushCursor(cursor, hotspotX, hotspotY);
    m_MouseCursorSet = true;
  }
}

void QmitkMultiLabelSegmentationView::InitializeListeners()
{
  if (m_Interactor.IsNull())
  {
    us::Module* module = us::GetModuleContext()->GetModule();
    std::vector<us::ModuleResource> resources = module->FindResources("/", "*", true);
    for (std::vector<us::ModuleResource>::iterator iter = resources.begin(); iter != resources.end(); ++iter)
    {
      MITK_INFO << iter->GetResourcePath();
    }

    m_Interactor = mitk::SegmentationInteractor::New();
    if (!m_Interactor->LoadStateMachine("SegmentationInteraction.xml", module))
    {
      MITK_WARN << "Error loading state machine";
    }

    if (!m_Interactor->SetEventConfig("ConfigSegmentation.xml", module))
    {
      MITK_WARN << "Error loading state machine configuration";
    }

    // Register as listener via micro services
    us::ServiceProperties props;
    props["name"] = std::string("SegmentationInteraction");
    m_ServiceRegistration =
      us::GetModuleContext()->RegisterService<mitk::InteractionEventObserver>(m_Interactor.GetPointer(), props);
  }
}

bool QmitkMultiLabelSegmentationView::CheckForSameGeometry(const mitk::Image *image1, const mitk::Image *image2) const
{
  bool isSameGeometry(true);

  if (image1 && image2)
  {
    mitk::BaseGeometry::Pointer geo1 = image1->GetGeometry();
    mitk::BaseGeometry::Pointer geo2 = image2->GetGeometry();

    isSameGeometry = isSameGeometry && mitk::Equal(geo1->GetOrigin(), geo2->GetOrigin());
    isSameGeometry = isSameGeometry && mitk::Equal(geo1->GetExtent(0), geo2->GetExtent(0));
    isSameGeometry = isSameGeometry && mitk::Equal(geo1->GetExtent(1), geo2->GetExtent(1));
    isSameGeometry = isSameGeometry && mitk::Equal(geo1->GetExtent(2), geo2->GetExtent(2));
    isSameGeometry = isSameGeometry && mitk::Equal(geo1->GetSpacing(), geo2->GetSpacing());
    isSameGeometry = isSameGeometry && mitk::MatrixEqualElementWise(geo1->GetIndexToWorldTransform()->GetMatrix(),
                                                                    geo2->GetIndexToWorldTransform()->GetMatrix());

    return isSameGeometry;
  }
  else
  {
    return false;
  }
}

//QString QmitkMultiLabelSegmentationView::GetLastFileOpenPath()
//{
//  return this->GetPreferences()->Get("LastFileOpenPath", "");
//}

//void QmitkMultiLabelSegmentationView::SetLastFileOpenPath(const QString &path)
//{
//  this->GetPreferences()->Put("LastFileOpenPath", path);
//  this->GetPreferences()->Flush();
//}
