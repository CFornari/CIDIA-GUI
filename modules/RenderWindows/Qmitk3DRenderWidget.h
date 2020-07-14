#ifndef QMITK3DRENDERWIDGET_H
#define QMITK3DRENDERWIDGET_H

#include "QmitkAbstractRenderWidget.h"

class Qmitk3DRenderWidget : public QmitkAbstractRenderWidget
{
public:
	explicit Qmitk3DRenderWidget(QWidget *parent,
															 const QString& widgetName);


	void AppendChildPlaneNodes(mitk::DataNode::Pointer childPlaneNode);
	void ClearChildPlaneNodes();

	void AddPlanesToDataStorage();
	void RemovePlanesFromDataStorage();

private:
	void CreatePlaneNode();
	void Set3DView();

	virtual void showEvent(QShowEvent *e) override;
	virtual void hideEvent(QHideEvent *e) override;

	QVector<mitk::DataNode::Pointer> m_ChildPlaneNodes;
};

#endif // QMITK3DRENDERWIDGET_H
