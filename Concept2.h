#ifndef CONCEPT2_H
#define CONCEPT2_H

#include <QWidget>

#include <mitkStandaloneDataStorage.h>

namespace Ui {
	class Concept2;
}

class QmitkStdMultiWidget;

class Concept2 : public QWidget
{
	Q_OBJECT

public:
	explicit Concept2(QWidget *parent = nullptr,
										mitk::StandaloneDataStorage::Pointer ds = mitk::StandaloneDataStorage::New());
	~Concept2();

	void AddPlanesToDataStorage();
	void RemovePlanesFromDataStorage();

private:
	Ui::Concept2 *ui;
	mitk::StandaloneDataStorage::Pointer m_DataStorage;
	QmitkStdMultiWidget *m_MultiWidget;
};

#endif // CONCEPT2_H
