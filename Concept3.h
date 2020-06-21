#ifndef CONCEPT3_H
#define CONCEPT3_H

#include "Utils.h"

#include <QWidget>

namespace Ui {
	class Concept3;
}

class AppDataManager;
class QmitkRenderWindowWidget;
class VolumeVisualizationView;

class Concept3 : public QWidget
{
	Q_OBJECT

public:
	explicit Concept3(QWidget *parent = nullptr);
	~Concept3();

public slots:
	void onDataLoaded();
	void onVisibilityChanged(bool visible);

private:
	Ui::Concept3 *ui;

	AppDataManager *m_AppData;

	QmitkRenderWindowWidget *renderWidget;

	VolumeVisualizationView *volumeVisualizationView;
};

#endif // CONCEPT3_H
