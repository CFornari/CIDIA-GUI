/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef QMITKSEGMENTATIONVIEW_H
#define QMITKSEGMENTATIONVIEW_H

#include <QWidget>

#include <mitkStandaloneDataStorage.h>

#include <mitkToolManager.h>

#include <berryIBerryPreferences.h>

class QmitkRenderWindow;

/**
* @brief
*
*
*/
class QmitkSegmentationView : public QWidget
{
  Q_OBJECT

public:

	QmitkSegmentationView(QWidget* parent);

  ~QmitkSegmentationView() override;

  typedef std::map<mitk::DataNode*, unsigned long> NodeTagMapType;

	void SetDataStorage(mitk::StandaloneDataStorage::Pointer dataStorage);
	mitk::StandaloneDataStorage::Pointer GetDataStorage();

  void NewNodeObjectsGenerated(mitk::ToolManager::DataVectorType*);

	void SetFocus();

	void RenderWindowPartActivated(mitk::IRenderWindowPart* renderWindowPart);

	void RenderWindowPartDeactivated(mitk::IRenderWindowPart* renderWindowPart);

  // BlueBerry's notification about preference changes (e.g. from a dialog)
	void OnPreferencesChanged(const berry::IBerryPreferences* prefs);

  // observer to mitk::RenderingManager's RenderingManagerViewsInitializedEvent event
  void CheckRenderingState();

  static const std::string VIEW_ID;

  protected slots:

  void OnPatientSelectionChanged(QList<mitk::DataNode::Pointer> nodes);
  void OnSegmentationSelectionChanged(QList<mitk::DataNode::Pointer> nodes);

  // reaction to the button "New segmentation"
  void CreateNewSegmentation();

  void OnManualTool2DSelected(int id);

  void OnVisiblePropertyChanged();

  void OnShowMarkerNodes(bool);

  void OnTabWidgetChanged(int);

protected:

  // a type for handling lists of DataNodes
  typedef std::vector<mitk::DataNode*> NodeList;

  // GUI setup
	void setupGUI(QWidget* parent);

  // propagate BlueBerry selection to ToolManager for manual segmentation
  void SetToolManagerSelection(mitk::DataNode* referenceData, mitk::DataNode* workingData);

  // make sure all images/segmentations look as selected by the users in this view's preferences
  void ForceDisplayPreferencesUponAllImages();

  // decorates a DataNode according to the user preference settings
  void ApplyDisplayOptions(mitk::DataNode* node);

  void ResetMouseCursor();

  void SetMouseCursor(const us::ModuleResource&, int hotspotX, int hotspotY);

  void SetToolSelectionBoxesEnabled(bool);

  // If a contourmarker is selected, the plane in the related widget will be reoriented according to the marker`s geometry
  void OnContourMarkerSelected(const mitk::DataNode* node);

	void OnSelectionChanged(berry::IWorkbenchPart::Pointer part, const QList<mitk::DataNode::Pointer> &nodes);

	void NodeRemoved(const mitk::DataNode* node);

	void NodeAdded(const mitk::DataNode *node);

  static bool CheckForSameGeometry(const mitk::DataNode*, const mitk::DataNode*);

  void UpdateWarningLabel(QString text/*, bool overwriteExistingText = true*/);

  // the Qt parent of our GUI (NOT of this object)
  QWidget* m_Parent;

  // our GUI
  Ui::QmitkSegmentationControls* m_Controls;

  mitk::IRenderWindowPart* m_RenderWindowPart;

  unsigned long m_VisibilityChangedObserverTag;

  bool m_MouseCursorSet;

  bool m_DataSelectionChanged;

  NodeTagMapType  m_WorkingDataObserverTags;

  unsigned int m_RenderingManagerObserverTag;

  mitk::NodePredicateNot::Pointer m_IsNotAHelperObject;
  mitk::NodePredicateAnd::Pointer m_IsOfTypeImagePredicate;
  mitk::NodePredicateOr::Pointer m_IsASegmentationImagePredicate;
  mitk::NodePredicateAnd::Pointer m_IsAPatientImagePredicate;

	mitk::StandaloneDataStorage::Pointer m_DataStorage;
};

#endif // QMITKSEGMENTATIONVIEW_H
