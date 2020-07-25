/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/


#ifndef QMITK_NODE_SELECTION_BUTTON_H
#define QMITK_NODE_SELECTION_BUTTON_H

#include <mitkWeakPointer.h>
#include <mitkDataNode.h>

#include "QPushButton"
#include "QPixmap"


/** Button class that can be used to display informations about a passed node.
 * If the passed node is a null ptr the node info text will be shown.
 * In difference to the normal push button text property. The node info can
 * be formated text (e.g. HTML code; like the tooltip text).*/
class QmitkNodeSelectionButton : public QPushButton
{
  Q_OBJECT

public:
  explicit QmitkNodeSelectionButton(QWidget *parent = nullptr);
  ~QmitkNodeSelectionButton() override;

  const mitk::DataNode* GetSelectedNode() const;
  bool GetSelectionIsOptional() const;

public Q_SLOTS :
  virtual void SetSelectedNode(const mitk::DataNode* node);
  virtual void SetNodeInfo(QString info);

  /** Set the widget into an optional mode. Optional means that the selection of no valid
  node does not mean an invalid state. Thus no node is a valid "node" selection too.
  The state influences if the info text is handled as an information (optional) or a
  warning (optiona==false).*/
  void SetSelectionIsOptional(bool isOptional);

protected:
  void paintEvent(QPaintEvent *p) override;
  void changeEvent(QEvent *event) override;

  void AddNodeObserver();
  void RemoveNodeObserver();
  void OnNodeModified(const itk::Object * /*caller*/, const itk::EventObject &);

  mitk::DataNode::ConstPointer m_SelectedNode;
  QString m_Info;
  bool m_OutDatedThumbNail;
  QPixmap m_ThumbNail;
  itk::ModifiedTimeType m_DataMTime;
  itk::ModifiedTimeType m_SelectionPropMTime;

  bool m_IsOptional;

  unsigned long m_NodeModifiedObserverTag;
  bool m_NodeObserved;
};


#endif // QmitkSingleNodeSelectionWidget_H
