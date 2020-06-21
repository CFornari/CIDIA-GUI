#include "Concept1.h"
#include "ui_Concept1.h"

#include "AppDataManager.h"

#include <QmitkStdMultiWidget.h>
#include <QmitkRenderWindowWidget.h>

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

Concept1::Concept1(QWidget *parent)
	:	QWidget(parent),
		m_listCount(3),
		m_valueMax(10),
		m_valueCount(7),
		m_dataTable(generateRandomData(m_listCount, m_valueMax, m_valueCount)),
		ui(new Ui::Concept1)
{
	ui->setupUi(this);

	m_AppData = AppDataManager::GetInstance();

	m_MultiWidget = new QmitkStdMultiWidget(this);
	m_MultiWidget->SetDataStorage(m_AppData->getDataStorage());
	m_MultiWidget->InitializeMultiWidget();
	m_MultiWidget->ResetCrosshair();
	m_MultiWidget->GetMultiWidgetLayoutManager()->SetLayoutDesign(QmitkMultiWidgetLayoutManager::LayoutDesign::ONLY_2D_VERTICAL);
	ui->centerDisplay->layout()->addWidget(m_MultiWidget);

	m_3DView = new QmitkRenderWindowWidget(this, QString("3D_Statistics"), m_AppData->getDataStorage());
	m_3DView->setStyleSheet("border: 0px");
	m_3DView->SetCornerAnnotationText("3D");
//	m_3DView->GetRenderWindow()->SetLayoutIndex(mitk::BaseRenderer::ViewDirection::THREE_D);
	m_3DView->GetRenderWindow()->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);
	m_3DView->GetRenderWindow()->GetRenderer()->GetVtkRenderer()->ResetCamera();
	ui->leftDisplay->layout()->addWidget(m_3DView);

	ui->widget_5->setVisible(false);

//  m_3DView->SetDecorationColor();

//  mitk::BaseRenderer::GetInstance(renderWindowWidget4->GetRenderWindow()->GetRenderWindow())->SetMapperID(mitk::BaseRenderer::Standard3D);

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

Concept1::~Concept1()
{
	m_MultiWidget->RemovePlanesFromDataStorage();
	delete ui;
}

DataTable Concept1::generateRandomData(int listCount, int valueMax, int valueCount) const
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

QChart* Concept1::createAreaChart() const
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

QChart* Concept1::createBarChart(int valueCount) const
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

QChart* Concept1::createSplineChart() const
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

QChart* Concept1::createScatterChart() const
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

void Concept1::onVisibilityChanged(bool visible)
{
	this->setVisible(visible);
}

