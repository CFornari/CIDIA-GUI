#ifndef CONCEPT_H
#define CONCEPT_H

#include <QMainWindow>

#include <QWidget>
#include <QChartGlobal>

#include <mitkStandaloneDataStorage.h>

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

QT_CHARTS_USE_NAMESPACE

namespace Ui {
	class Concept;
}

class Concept : public QMainWindow
{
	Q_OBJECT

public:
	explicit Concept(QWidget *parent = nullptr);
	~Concept();

	DataTable generateRandomData(int listCount, int valueMax, int valueCount) const;
	QChart *createAreaChart() const;
	QChart *createBarChart(int valueCount) const;
	QChart *createSplineChart() const;
	QChart *createScatterChart() const;

	void loadImage(const QString filePath);

public slots:
	void openFileDialog();

private:
	int m_listCount;
	int m_valueMax;
	int m_valueCount;
	QList<QChartView *> m_charts;
	DataTable m_dataTable;
	Ui::Concept *m_ui;

	mitk::StandaloneDataStorage::Pointer m_DataStorage;
};

#endif // CONCEPT_H
