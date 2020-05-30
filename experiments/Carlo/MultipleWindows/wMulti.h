#ifndef WMULTI_H
#define WMULTI_H

#include "QmitkStdMultiWidget.h"

#include <mitkStandaloneDataStorage.h>

#include <QWidget>

class wMulti : public QWidget
{
	Q_OBJECT
public:
	explicit wMulti(QWidget *parent = nullptr);

	void setData(mitk::StandaloneDataStorage::Pointer ds);
	QmitkRenderWindow* get3DRenderWindow();

protected:
	QmitkStdMultiWidget *m_MultiWidget ;
	mitk::StandaloneDataStorage::Pointer m_DataStorage;

};

#endif // WMULTI_H
