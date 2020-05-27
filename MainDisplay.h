#ifndef MAINDISPLAY_H
#define MAINDISPLAY_H

#include "QmitkStdMultiWidget.h"

#include <mitkStandaloneDataStorage.h>

#include <QWidget>

class MainDisplay : public QWidget
{
	Q_OBJECT
public:
	explicit MainDisplay(QWidget *parent,
											 mitk::StandaloneDataStorage::Pointer dataStorage);

protected:
	QmitkStdMultiWidget *m_MultiWidget ;
	mitk::StandaloneDataStorage::Pointer m_DataStorage;
};

#endif // MAINDISPLAY_H
