#include "Concept.h"
#include "ui_Concept.h"

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
#include <QmitkIOUtil.h>

Concept::Concept(QWidget *parent) :
	QMainWindow(parent),
	m_listCount(3),
	m_valueMax(10),
	m_valueCount(7),
	m_dataTable(generateRandomData(m_listCount, m_valueMax, m_valueCount)),
	m_DataStorage(mitk::StandaloneDataStorage::New()),
	m_ui(new Ui::Concept)
{
	m_ui->setupUi(this);

	m_ui->leftDisplay->setDataStorage(m_DataStorage);
	m_ui->centerDisplay->setDataStorage(m_DataStorage);

	QChartView *chartView;

	//	chartView = new QChartView(createAreaChart());
	//	m_ui->chartTab->layout()->addWidget(chartView);
	//	m_charts << chartView;

	chartView = new QChartView(createBarChart(m_valueCount));
	m_ui->chartTab->layout()->addWidget(chartView);
	m_charts << chartView;

	chartView = new QChartView(createSplineChart());
	m_ui->chartTab->layout()->addWidget(chartView);
	m_charts << chartView;

	chartView = new QChartView(createScatterChart());
	m_ui->chartTab->layout()->addWidget(chartView);
	m_charts << chartView;

	const auto charts = m_charts;
	for (QChartView *chartView : charts)
	{
		chartView->chart()->setTheme(QChart::ChartThemeDark);
		chartView->chart()->legend()->hide();
		chartView->setRenderHint(QPainter::Antialiasing, true);
	}
}

Concept::~Concept()
{
	delete m_ui;
}

DataTable Concept::generateRandomData(int listCount, int valueMax, int valueCount) const
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

QChart* Concept::createAreaChart() const
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

QChart* Concept::createBarChart(int valueCount) const
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

QChart* Concept::createSplineChart() const
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

QChart* Concept::createScatterChart() const
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

void Concept::loadImage(const QString filePath)
{
	// Do the actual work of loading the data into the data storage
	mitk::DataStorage::SetOfObjects::Pointer dataNodes;
	try
	{
		dataNodes = QmitkIOUtil::Load(filePath, *m_DataStorage);
	}
	catch (const mitk::Exception& e)
	{
		MITK_INFO << e;
		return;
	}

	// If the file is loaded refresh display's views.
	if(!dataNodes->empty())
		// global reinit
		QmitkRenderingManager::GetInstance()->InitializeViewsByBoundingObjects(m_DataStorage);
	else
		exit(2);
}

void Concept::openFileDialog()
{
	const QString directory =
			QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).value(0, QDir::homePath());
	QFileDialog dialog(this, tr("Open Image"), directory);
	dialog.setAcceptMode(QFileDialog::AcceptOpen);
	dialog.setFileMode(QFileDialog::ExistingFile);

	if (dialog.exec() == QDialog::Accepted)
		loadImage(dialog.selectedFiles().constFirst());
}
