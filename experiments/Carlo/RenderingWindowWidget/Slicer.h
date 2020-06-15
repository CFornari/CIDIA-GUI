#ifndef SLICER_H
#define SLICER_H

#include <mitkStandaloneDataStorage.h>

#include <QWidget>

class QmitkSliceWidget;

class Slicer : public QWidget
{
	Q_OBJECT
public:
	explicit Slicer(QWidget *parent = nullptr);

	void setData(mitk::StandaloneDataStorage::Pointer ds);

private:
	QmitkSliceWidget *m_SliceRenderWindow;

};

#endif // SLICER_H
