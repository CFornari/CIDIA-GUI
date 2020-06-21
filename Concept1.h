#ifndef CONCEPT_H
#define CONCEPT_H

#include <QWidget>
#include <QChartGlobal>

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

QT_CHARTS_USE_NAMESPACE

namespace Ui {
	class Concept1;
}

class QmitkStdMultiWidget;
class QmitkRenderWindowWidget;
class AppDataManager;

class Concept1 : public QWidget
{
	Q_OBJECT

public:
	explicit Concept1(QWidget *parent = nullptr);
	~Concept1();

	DataTable generateRandomData(int listCount, int valueMax, int valueCount) const;
	QChart *createAreaChart() const;
	QChart *createBarChart(int valueCount) const;
	QChart *createSplineChart() const;
	QChart *createScatterChart() const;

public slots:
	void onVisibilityChanged(bool visible);

private:
	int m_listCount;
	int m_valueMax;
	int m_valueCount;
	QList<QChartView *> m_charts;
	DataTable m_dataTable;
	Ui::Concept1 *ui;

	QmitkStdMultiWidget *m_MultiWidget;
	QmitkRenderWindowWidget *m_3DView;

	AppDataManager *m_AppData;
};

#endif // CONCEPT_H
