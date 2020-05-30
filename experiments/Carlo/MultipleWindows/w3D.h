#ifndef W3D_H
#define W3D_H

#include <mitkStandaloneDataStorage.h>

#include <QWidget>

class QmitkRenderWindow;

class w3D : public QWidget
{
	Q_OBJECT
public:
	explicit w3D(QWidget *parent = nullptr);

	void setData(mitk::StandaloneDataStorage::Pointer ds,
							 mitk::StandaloneDataStorage::SetOfObjects::Pointer dataNodes);

private:
	QmitkRenderWindow *m_RenderWindow;
};

#endif // W3D_H
