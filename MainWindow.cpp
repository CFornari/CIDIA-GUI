#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Concept1.h"
#include "Concept2.h"
#include "Concept3.h"
#include "Utils.h"

// Qmitk
#include <QmitkRenderingManager.h>
#include <QmitkRenderWindow.h>
#include <QmitkIOUtil.h>

// MITK
//#include <mitkDataStorage.h>

// Qt GUI
#include <QSplitter>
#include <QTabWidget>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QtWidgets>
#include <stdlib.h>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
		m_AppData(new AppDataStorage),
		ui(new Ui::MainWindow)
//		m_FirstImage(mitk::Image::New())
{
	ui->setupUi(this);

	QGraphicsDropShadowEffect *windowShadow = new QGraphicsDropShadowEffect(this);
	windowShadow->setBlurRadius(12.0);
	windowShadow->setColor(palette().color(QPalette::Highlight));
	windowShadow->setOffset(0.0);
	ui->widget_3->setGraphicsEffect(windowShadow);

	m_Concept1 = new Concept1(ui->windowView,  m_AppData->storage);
	ui->windowViewLayout->addWidget(m_Concept1);

	m_Concept2 = new Concept2(ui->windowView, m_AppData->storage);
	ui->windowViewLayout->addWidget(m_Concept2);

	m_Concept3 = new Concept3(ui->windowView, m_AppData);
	ui->windowViewLayout->addWidget(m_Concept3);

	showConcept1();

	connect(this, &MainWindow::dataLoaded, m_Concept3, &Concept3::dataLoaded);
//	connect(this, &MainWindow::dataStorageUpdated, m_Concept1, &Concept::resetView);
//	connect(this, &MainWindow::fileLoaded, m_ThumbnailListWidget, &ThumbnailListWidgetInterface::loadPictures);
}

MainWindow::~MainWindow()
{
	delete m_AppData;
}

void MainWindow::showConcept1()
{
	m_Concept1->RemovePlanesFromDataStorage();
	m_Concept2->RemovePlanesFromDataStorage();

	m_Concept1->setVisible(true);
	m_Concept2->setVisible(false);
	m_Concept3->setVisible(false);
}

void MainWindow::showConcept2()
{
	m_Concept1->RemovePlanesFromDataStorage();
	m_Concept2->RemovePlanesFromDataStorage();

	m_Concept1->setVisible(false);
	m_Concept2->setVisible(true);
	m_Concept3->setVisible(false);
}

void MainWindow::showConcept3()
{
	m_Concept1->RemovePlanesFromDataStorage();
	m_Concept2->RemovePlanesFromDataStorage();

	m_Concept1->setVisible(false);
	m_Concept2->setVisible(false);
	m_Concept3->setVisible(true);
}

void MainWindow::setupLayout()
{
//	QFrame *mainWidget = new QFrame(this);
//	this->setCentralWidget(mainWidget);

//	QHBoxLayout *hLayout = new QHBoxLayout(mainWidget);
//	mainWidget->setLayout(hLayout);

//	QSplitter *splitter = new QSplitter(this);
//	hLayout->addWidget(splitter);

//	m_ThumbnailListWidget = new ThumbnailListWidgetInterface(splitter);
//	m_MainDisplay = new MainDisplay(splitter);
//	SliceAnimationControls *sliceAnimationWidget = new SliceAnimationControls(splitter, m_DataStorage, m_FirstImage);
//	splitter->addWidget(m_ThumbnailListWidget);
//	splitter->addWidget(m_MainDisplay);

//	m_FirstImage->GetSlicedGeometry()->GetSlices();
}

void MainWindow::openFileDialog()
{
		const QString directory =
				QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).value(0, QDir::homePath());
		QFileDialog dialog(this, tr("Open Image"), directory);
		dialog.setAcceptMode(QFileDialog::AcceptOpen);
		dialog.setFileMode(QFileDialog::ExistingFile);

		if (dialog.exec() == QDialog::Accepted)
				loadImage(dialog.selectedFiles().constFirst());
}

void MainWindow::loadImage(const QString filePath)
{
	try
	{
		m_AppData->nodes = QmitkIOUtil::Load(filePath, *m_AppData->storage);
	}
	catch (const mitk::Exception& e)
	{
		MITK_INFO << e;
		return;
	}
//	setTrasnferFunction(m_AppData->nodes);

	mitk::RenderingManager::GetInstance()->InitializeViewsByBoundingObjects(m_AppData->storage);
	emit dataLoaded(m_AppData);
//	mitk::Image::Pointer image = dynamic_cast<mitk::Image *>(dataNodes->at(0)->GetData());

//	if ((m_FirstImage.IsNull()) && (image.IsNotNull()))
//		m_FirstImage = image;
//	else
//		exit(2);

//	QFileInfo fileInfo(filePath);

//	QDir dir(GenerateDirPath(fileInfo));

//	if(CreateSlicesPngDir(dir))
//	{
//		// axial
//		SaveSliceOrImageAsPNG(m_FirstImage, mitk::SliceNavigationController::Axial, dir.absolutePath().toStdString()+ "/axial/", false);
//		// sagital
//		SaveSliceOrImageAsPNG(m_FirstImage, mitk::SliceNavigationController::Sagittal, dir.absolutePath().toStdString() +"/sagittal/", false);
//		// coronal
//		SaveSliceOrImageAsPNG(m_FirstImage, mitk::SliceNavigationController::Frontal, dir.absolutePath().toStdString() + "/coronal/", false);
//	}

//	emit fileLoaded(dir.absolutePath());
	// If the file is loaded refresh display's views.
//	if(!dataNodes->empty())
//		// global reinit
//		QmitkRenderingManager::GetInstance()->InitializeViewsByBoundingObjects(m_DataStorage);
//	else
//		exit(2);

//	qDebug() << "SIZE: " << m_FirstImage->GetSlicedGeometry()->GetSlices();

	//	m_FirstImage->GetSlicedGeometry()->GetSliceNavigationController()->GetSlice()->SetPos(0);
	//	qDebug() << QmitkIOUtil::Save(m_FirstImage, QString("image.png"), QString("~/"), this, false);
}
