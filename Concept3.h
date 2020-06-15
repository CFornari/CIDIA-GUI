#ifndef CONCEPT3_H
#define CONCEPT3_H

#include "Utils.h"

#include <QWidget>

#include <mitkStandaloneDataStorage.h>


namespace Ui {
	class Concept3;
}

struct AppDataStorage;
class QmitkRenderWindowWidget;
class VolumeVisualizationView;

class Concept3 : public QWidget
{
	Q_OBJECT

public:
	explicit Concept3(QWidget *parent = nullptr,
										AppDataStorage *appData = new AppDataStorage());
	~Concept3();

public slots:
	void dataLoaded(AppDataStorage* appData);

private:
	Ui::Concept3 *ui;

	AppDataStorage *m_AppData;

	QmitkRenderWindowWidget *renderWidget;

	VolumeVisualizationView *volumeVisualizationView;
};

#endif // CONCEPT3_H
