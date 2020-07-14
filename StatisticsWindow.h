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
	class StatisticsWindow;
}

class QmitkStdMultiWidget;
class Qmitk3DRenderWidget;
class AppDataManager;

class StatisticsWindow : public QWidget
{
	Q_OBJECT

public:
	explicit StatisticsWindow(QWidget *parent = nullptr);
	~StatisticsWindow();

	DataTable generateRandomData(int listCount, int valueMax, int valueCount) const;
	QChart *createAreaChart() const;
	QChart *createBarChart(int valueCount) const;
	QChart *createSplineChart() const;
	QChart *createScatterChart() const;

public slots:
	void onNewDataLoadedEnd();

private:
	virtual void showEvent(QShowEvent *e) override;
	virtual void hideEvent(QHideEvent *e) override;
	void resetViews();

	int m_listCount;
	int m_valueMax;
	int m_valueCount;
	QList<QChartView *> m_charts;
	DataTable m_dataTable;
	Ui::StatisticsWindow *ui;

	QmitkStdMultiWidget *m_MultiWidget;
	Qmitk3DRenderWidget *m_3DView;

	AppDataManager *m_AppData;
};

#endif // CONCEPT_H
