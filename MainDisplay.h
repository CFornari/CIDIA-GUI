#ifndef MAINDISPLAY_H
#define MAINDISPLAY_H

#include <mitkStandaloneDataStorage.h>

#include <QWidget>

class QmitkStdMultiWidget;
class QmitkRenderWindow;

class MainDisplay : public QWidget
{
	Q_OBJECT
public:
	explicit MainDisplay(QWidget *parent);

	void setDataStorage(mitk::StandaloneDataStorage::Pointer dataStorage);
	QmitkRenderWindow* get3DRenderWindow() const;
protected:
	QmitkStdMultiWidget *m_MultiWidget ;
	mitk::StandaloneDataStorage::Pointer m_DataStorage;
};

#endif // MAINDISPLAY_H
