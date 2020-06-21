#include "ThumbnailListLoader.h"

#include <QtConcurrent>
#include <QDir>

#include <AlbumModel.h>
#include <PictureModel.h>

ThumbnailListLoader* ThumbnailListLoader::m_Instance = nullptr;

ThumbnailListLoader* ThumbnailListLoader::GetInstance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new ThumbnailListLoader;
	}
	return m_Instance;
}

ThumbnailListLoader::ThumbnailListLoader()
	: QObject(qApp)
{
	m_AlbumModel = new AlbumModel(this);
	m_AlbumModel->addAlbum(Album("Axial")); // ID = 1
	m_AlbumModel->addAlbum(Album("Sagittal")); // ID = 2
	m_AlbumModel->addAlbum(Album("Coronal")); // ID = 3

	m_PictureModel = new PictureModel(this);
}

PictureModel* ThumbnailListLoader::getPictureModel() const
{
	return m_PictureModel;
}

void ThumbnailListLoader::addPictures(const QString& filePath,
																			const QStringList& fileNames,
																			int albumID)
{
	if (!fileNames.isEmpty())
	{
		m_PictureModel->setAlbumId(albumID);
		QVector<Picture> pictures;

		for (auto filename : fileNames)
		{
			Picture picture(QString(filePath+filename));
			pictures.append(picture);
		}
		m_PictureModel->addPictures(pictures);
	}
}

void ThumbnailListLoader::loadPictures(const QString& dirAbsolutePath)
{
	//assume the directory exists and contains some files and you want all jpg and JPG files
	QStringList images;

	// Add axial slices
	QDir axialDir(QString(dirAbsolutePath+"/axial"));
	images = axialDir.entryList(QStringList() << "*",QDir::Files);
	QFuture<void> m_F1 = QtConcurrent::run(this, &ThumbnailListLoader::addPictures,
													 QString(dirAbsolutePath+"/axial/"),images, 1);

	// Add sagittal slices
	QDir sagittalDir(QString(dirAbsolutePath+"/sagittal"));
	images = sagittalDir.entryList(QStringList() << "*",QDir::Files);
	QFuture<void> m_F2 = QtConcurrent::run(this, &ThumbnailListLoader::addPictures,
													 QString(dirAbsolutePath+"/sagittal/"),images, 2);


	// Add coronal slices
	QDir coronalDir(QString(dirAbsolutePath+"/coronal"));
	images = coronalDir.entryList(QStringList() << "*",QDir::Files);
	QFuture<void> m_F3 = QtConcurrent::run(this, &ThumbnailListLoader::addPictures,
													 QString(dirAbsolutePath+"/coronal/"),images, 3);

	m_F1.waitForFinished();
	m_F2.waitForFinished();
	m_F3.waitForFinished();
}
