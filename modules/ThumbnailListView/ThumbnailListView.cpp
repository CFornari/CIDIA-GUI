#include "ThumbnailListView.h"

#include "ThumbnailListLoader.h"
#include "ThumbnailProxyModel.h"
#include "PictureDelegate.h"

#include <AlbumModel.h>
#include <PictureModel.h>

ThumbnailListView::ThumbnailListView(QWidget *parent)
	: QListView(parent)
{
	setSpacing(5);
	setResizeMode(QListView::Adjust);
	setFlow(QListView::LeftToRight);
	setWrapping(true);
	setItemDelegate(new PictureDelegate(this));

	m_Loader = ThumbnailListLoader::GetInstance();

	m_ThumbnailProxyModel = new ThumbnailProxyModel(this);
	m_ThumbnailProxyModel->setSourceModel(m_Loader->getPictureModel());
	setModel(m_ThumbnailProxyModel);

	setActiveAlbum(ThumbnailListView::Axial);
}

void ThumbnailListView::setActiveAlbum(ThumbnailListView::ViewPerspective perspective)
{
	switch (perspective)
	{
	case Axial:		
		m_AlbumID = 1;
		break;
	case Sagittal:
		m_AlbumID = 2;
		break;
	case Coronal:
		m_AlbumID = 3;
		break;
	}

	m_Loader->getPictureModel()->setAlbumId(m_AlbumID);
}
