#ifndef WMULTI_H
#define WMULTI_H

#include <QWidget>

#include <mitkStandaloneDataStorage.h>

class QmitkStdMultiWidget;
class QmitkRenderWindow;

class wMulti : public QWidget
{
	Q_OBJECT
public:
	explicit wMulti(QWidget *parent = nullptr);

	void setData(mitk::StandaloneDataStorage::Pointer ds);
	QmitkStdMultiWidget* getRenderWindow();
protected:
	QmitkStdMultiWidget *m_MultiWidget ;
	mitk::StandaloneDataStorage::Pointer m_DataStorage;

};

#endif // WMULTI_H
