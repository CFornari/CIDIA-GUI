#include "StatisticsWindow.h"
#include "ui_StatisticsWindow.h"

#include "AppDataManager.h"

#include <QmitkStdMultiWidget.h>
#include <Qmitk3DRenderWidget.h>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCore/QRandomGenerator>
#include <QtCharts/QBarCategoryAxis>
#include <QtWidgets/QApplication>
#include <QtCharts/QValueAxis>

#include <QDir>
#include <QStandardPaths>
#include <QFileDialog>

#include <QmitkRenderingManager.h>
#include <QmitkRenderWindow.h>
#include <QmitkIOUtil.h>

StatisticsWindow::StatisticsWindow(QWidget *parent)
	:	QWidget(parent),
		m_listCount(3),
		m_valueMax(10),
		m_valueCount(7),
		m_dataTable(generateRandomData(m_listCount, m_valueMax, m_valueCount)),
		ui(new Ui::StatisticsWindow)
{
	ui->setupUi(this);

	m_AppData = AppDataManager::GetInstance();
	m_AppData->createDataStorageFromMaster("Statistics");
	auto dataStorage = m_AppData->getDataStorageByName("Statistics");

	m_AppData->createDataStorageFromMaster("3D_Statistics");
	auto dataStorage2 = m_AppData->getDataStorageByName("3D_Statistics");

	m_MultiWidget = new QmitkStdMultiWidget(this);
	m_MultiWidget->SetDataStorage(dataStorage);
	m_MultiWidget->InitializeMultiWidget();
	m_MultiWidget->ResetCrosshair();
	m_MultiWidget->AddPlanesToDataStorage();
	m_MultiWidget->GetMultiWidgetLayoutManager()->SetLayoutDesign(QmitkMultiWidgetLayoutManager::LayoutDesign::ONLY_2D_VERTICAL);
	ui->centerDisplay->layout()->addWidget(m_MultiWidget);

	m_3DView = new Qmitk3DRenderWidget(this, QString("3D_Statistics"));
	m_3DView->SetDataStorage(dataStorage2);
	ui->horizontalLayout_4->addWidget(m_3DView);

	connect(m_AppData, &AppDataManager::newDataLoadedEnd, this, &StatisticsWindow::onNewDataLoadedEnd);

	ui->widget_5->hide();

//	QChartView *chartView;

//	//	chartView = new QChartView(createAreaChart());
//	//	m_ui->chartTab->layout()->addWidget(chartView);
//	//	m_charts << chartView;

//	chartView = new QChartView(createBarChart(m_valueCount));
//	ui->chartTab->layout()->addWidget(chartView);
//	m_charts << chartView;

//	chartView = new QChartView(createSplineChart());
//	ui->chartTab->layout()->addWidget(chartView);
//	m_charts << chartView;

//	chartView = new QChartView(createScatterChart());
//	ui->chartTab->layout()->addWidget(chartView);
//	m_charts << chartView;

//	const auto charts = m_charts;
//	for (QChartView *chartView : charts)
//	{
//		chartView->chart()->setTheme(QChart::ChartThemeDark);
//		chartView->chart()->legend()->hide();
//		chartView->setRenderHint(QPainter::Antialiasing, true);
//	}
}

StatisticsWindow::~StatisticsWindow()
{
	m_MultiWidget->RemovePlanesFromDataStorage();
	delete ui;
}

DataTable StatisticsWindow::generateRandomData(int listCount, int valueMax, int valueCount) const
{
	DataTable dataTable;

	// generate random data
	for (int i(0); i < listCount; i++)
	{
		DataList dataList;
		qreal yValue(0);
		for (int j(0); j < valueCount; j++)
		{
			yValue = yValue + QRandomGenerator::global()->bounded(valueMax / (qreal) valueCount);
			QPointF value((j + QRandomGenerator::global()->generateDouble()) * ((qreal) m_valueMax / (qreal) valueCount),
										yValue);
			QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
			dataList << Data(value, label);
		}
		dataTable << dataList;
	}

	return dataTable;
}

QChart* StatisticsWindow::createAreaChart() const
{
	QChart *chart = new QChart();
	chart->setTitle("Area chart");

	// The lower series initialized to zero values
	QLineSeries *lowerSeries = 0;
	QString name("Series ");
	int nameIndex = 0;
	for (int i(0); i < m_dataTable.count(); i++) {
		QLineSeries *upperSeries = new QLineSeries(chart);
		for (int j(0); j < m_dataTable[i].count(); j++) {
			Data data = m_dataTable[i].at(j);
			if (lowerSeries) {
				const QVector<QPointF>& points = lowerSeries->pointsVector();
				upperSeries->append(QPointF(j, points[i].y() + data.first.y()));
			} else {
				upperSeries->append(QPointF(j, data.first.y()));
			}
		}
		QAreaSeries *area = new QAreaSeries(upperSeries, lowerSeries);
		area->setName(name + QString::number(nameIndex));
		nameIndex++;
		chart->addSeries(area);
		lowerSeries = upperSeries;
	}

	chart->createDefaultAxes();
	chart->axes(Qt::Horizontal).first()->setRange(0, m_valueCount - 1);
	chart->axes(Qt::Vertical).first()->setRange(0, m_valueMax);
	// Add space to label to add space between labels and axis
	QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
	Q_ASSERT(axisY);
	axisY->setLabelFormat("%.1f  ");

	return chart;
}

QChart* StatisticsWindow::createBarChart(int valueCount) const
{
	Q_UNUSED(valueCount);
	QChart *chart = new QChart();
	chart->setTitle("Bar chart");

	QStackedBarSeries *series = new QStackedBarSeries(chart);
	for (int i(0); i < m_dataTable.count(); i++) {
		QBarSet *set = new QBarSet("Bar set " + QString::number(i));
		for (const Data &data : m_dataTable[i])
			*set << data.first.y();
		series->append(set);
	}
	chart->addSeries(series);

	chart->createDefaultAxes();
	chart->axes(Qt::Vertical).first()->setRange(0, m_valueMax * 2);
	// Add space to label to add space between labels and axis
	QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
	Q_ASSERT(axisY);
	axisY->setLabelFormat("%.1f  ");

	return chart;
}

QChart* StatisticsWindow::createSplineChart() const
{
	QChart *chart = new QChart();
	chart->setTitle("Spline chart");
	QString name("Series ");
	int nameIndex = 0;
	for (const DataList &list : m_dataTable) {
		QSplineSeries *series = new QSplineSeries(chart);
		for (const Data &data : list)
			series->append(data.first);
		series->setName(name + QString::number(nameIndex));
		nameIndex++;
		chart->addSeries(series);
	}

	chart->createDefaultAxes();
	chart->axes(Qt::Horizontal).first()->setRange(0, m_valueMax);
	chart->axes(Qt::Vertical).first()->setRange(0, m_valueCount);

	// Add space to label to add space between labels and axis
	QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
	Q_ASSERT(axisY);
	axisY->setLabelFormat("%.1f  ");
	return chart;
}

QChart* StatisticsWindow::createScatterChart() const
{
	// scatter chart
	QChart *chart = new QChart();
	chart->setTitle("Scatter chart");
	QString name("Series ");
	int nameIndex = 0;
	for (const DataList &list : m_dataTable) {
		QScatterSeries *series = new QScatterSeries(chart);
		for (const Data &data : list)
			series->append(data.first);
		series->setName(name + QString::number(nameIndex));
		nameIndex++;
		chart->addSeries(series);
	}

	chart->createDefaultAxes();
	chart->axes(Qt::Horizontal).first()->setRange(0, m_valueMax);
	chart->axes(Qt::Vertical).first()->setRange(0, m_valueCount);
	// Add space to label to add space between labels and axis
	QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
	Q_ASSERT(axisY);
	axisY->setLabelFormat("%.1f  ");
	return chart;
}

void StatisticsWindow::onNewDataLoadedEnd()
{
	resetViews();
}

void StatisticsWindow::showEvent(QShowEvent* e)
{
	Q_UNUSED(e)

}

void StatisticsWindow::hideEvent(QHideEvent* e)
{
	Q_UNUSED(e)
}

void StatisticsWindow::resetViews()
{
	m_MultiWidget->ResetCrosshair();
	m_3DView->ResetView();
}

