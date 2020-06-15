#ifndef W3D_H
#define W3D_H

#include <mitkStandaloneDataStorage.h>

#include <QWidget>

class QmitkRenderWindow;
class vtkRenderWindow;

class w3D : public QWidget
{
	Q_OBJECT
public:
	explicit w3D(QWidget *parent = nullptr);

	void setTrasnferFunction(mitk::StandaloneDataStorage::SetOfObjects::Pointer dataNodes);
	void setRenderWindow(QmitkRenderWindow* win);
	void init(mitk::StandaloneDataStorage::Pointer ds);
	void setTest();

private:
	QmitkRenderWindow *m_RenderWindow;
};

#endif // W3D_H
