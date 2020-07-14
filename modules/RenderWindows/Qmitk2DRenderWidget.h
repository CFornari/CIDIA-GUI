#ifndef QMITK2DRENDERWIDGET_H
#define QMITK2DRENDERWIDGET_H

#include "QmitkAbstractRenderWidget.h"

class Qmitk2DRenderWidget : public QmitkAbstractRenderWidget
{
public:
	explicit Qmitk2DRenderWidget(QWidget *parent,
															 const QString& widgetName);

	void AddPlaneToDataStorage();
	void RemovePlaneFromDataStorage();

	void setAxialView();
	void setSagittalView();
	void setCoronalView();

private:
	void CreatePlaneNode();
};

#endif // QMITK2DRENDERWIDGET_H
