#include "MainWindow.h"

//#include "SliceAnimationControls.h"
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
		m_DataStorage(mitk::StandaloneDataStorage::New())
//		m_FirstImage(mitk::Image::New())
{
//	loadPlugin();
	setupMenus();
	setupLayout();

//	connect(this, &MainWindow::fileLoaded, m_ThumbnailListWidget, &ThumbnailListWidgetInterface::loadPictures);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupLayout()
{
	QFrame *mainWidget = new QFrame(this);
	this->setCentralWidget(mainWidget);

	QHBoxLayout *hLayout = new QHBoxLayout(mainWidget);
	mainWidget->setLayout(hLayout);

	QSplitter *splitter = new QSplitter(this);
	hLayout->addWidget(splitter);

//	m_ThumbnailListWidget = new ThumbnailListWidgetInterface(splitter);
	m_MainDisplay = new MainDisplay(splitter);
//	SliceAnimationControls *sliceAnimationWidget = new SliceAnimationControls(splitter, m_DataStorage, m_FirstImage);
//	splitter->addWidget(m_ThumbnailListWidget);
	splitter->addWidget(m_MainDisplay);

//	m_FirstImage->GetSlicedGeometry()->GetSlices();
}

void MainWindow::setupMenus()
{
	m_FileMenu = new QMenu(this);
	m_FileMenu = menuBar()->addMenu(tr("&File"));

	QAction *openAction = m_FileMenu->addAction(tr("&Open..."), this, &MainWindow::openFileDialog);
	openAction->setShortcuts(QKeySequence::Open);
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

	mitk::Image::Pointer image = dynamic_cast<mitk::Image *>(dataNodes->at(0)->GetData());

	if ((m_FirstImage.IsNull()) && (image.IsNotNull()))
		m_FirstImage = image;
	else
		exit(2);

	QFileInfo fileInfo(filePath);

	QDir dir(GenerateDirPath(fileInfo));

	if(CreateSlicesPngDir(dir))
	{
		// axial
		SaveSliceOrImageAsPNG(m_FirstImage, mitk::SliceNavigationController::Axial, dir.absolutePath().toStdString()+ "/axial/", false);
		// sagital
		SaveSliceOrImageAsPNG(m_FirstImage, mitk::SliceNavigationController::Sagittal, dir.absolutePath().toStdString() +"/sagittal/", false);
		// coronal
		SaveSliceOrImageAsPNG(m_FirstImage, mitk::SliceNavigationController::Frontal, dir.absolutePath().toStdString() + "/coronal/", false);
	}

	emit fileLoaded(dir.absolutePath());

//	qDebug() << "SIZE: " << m_FirstImage->GetSlicedGeometry()->GetSlices();

	//	m_FirstImage->GetSlicedGeometry()->GetSliceNavigationController()->GetSlice()->SetPos(0);
	//	qDebug() << QmitkIOUtil::Save(m_FirstImage, QString("image.png"), QString("~/"), this, false);
}

//bool MainWindow::loadPlugin()
//{
//		QDir pluginsDir(qApp->applicationDirPath());
//#if defined(Q_OS_WIN)
//		if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
//				pluginsDir.cdUp();
//#elif defined(Q_OS_MAC)
//		if (pluginsDir.dirName() == "MacOS") {
//				pluginsDir.cdUp();
//				pluginsDir.cdUp();
//				pluginsDir.cdUp();
//		}
//#endif
//		pluginsDir.cd("plugins");
//		foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
//				QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
//				QObject *plugin = pluginLoader.instance();
//				if (plugin) {
//						m_ThumbnailListWidget = qobject_cast<ThumbnailListWidgetInterface *>(plugin);
//						if (m_ThumbnailListWidget)
//								return true;
//				}
//		}

//		return false;
//}
