#include "ThumbnailListView.h"

#include "ThumbnailProxyModel.h"
#include "PictureDelegate.h"

#include <AlbumModel.h>
#include <PictureModel.h>

#include <QDir>

ThumbnailListView::ThumbnailListView(QWidget *parent)
	: QListView(parent)
{
	setSpacing(5);
	setResizeMode(QListView::Adjust);
	setFlow(QListView::LeftToRight);
	setWrapping(true);
	setItemDelegate(new PictureDelegate(this));

	m_AlbumModel = new AlbumModel(this);
	m_AlbumModel->addAlbum(Album("Axial"));
	m_AlbumModel->addAlbum(Album("Sagittal"));
	m_AlbumModel->addAlbum(Album("Coronal"));

	m_PictureModel = new PictureModel(this);
	m_ThumbnailProxyModel = new ThumbnailProxyModel(this);
	m_ThumbnailProxyModel->setSourceModel(m_PictureModel);
	setModel(m_ThumbnailProxyModel);
}

void ThumbnailListView::setActiveAlbum(ThumbnailListView::ViewPerspective perspective)
{
	switch (perspective)
	{
	case Axial:
		m_PictureModel->setAlbumId(1);
		break;
	case Sagittal:
		m_PictureModel->setAlbumId(2);
		break;
	case Coronal:
		m_PictureModel->setAlbumId(3);
		break;
	}
}

void ThumbnailListView::loadPictures(const QString& dirAbsolutePath)
{
	//assume the directory exists and contains some files and you want all jpg and JPG files
	QStringList images;

	// Add axial slices
	QDir axialDir(QString(dirAbsolutePath+"/axial"));
	images = axialDir.entryList(QStringList() << "*",QDir::Files);
	addPictures(QString(dirAbsolutePath+"/axial/"),images, 1);

	// Add sagittal slices
	QDir sagittalDir(QString(dirAbsolutePath+"/sagittal"));
	images = sagittalDir.entryList(QStringList() << "*",QDir::Files);
	addPictures(QString(dirAbsolutePath+"/sagittal/"),images, 2);

	// Add coronal slices
	QDir coronalDir(QString(dirAbsolutePath+"/coronal"));
	images = coronalDir.entryList(QStringList() << "*",QDir::Files);
	addPictures(QString(dirAbsolutePath+"/coronal/"),images, 3);
}

void ThumbnailListView::addPictures(const QString& filePath, const QStringList& fileNames, int albumID)
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

