#ifndef THUMBNAILLISTVIEW_H
#define THUMBNAILLISTVIEW_H

#include <QListView>


class ThumbnailProxyModel;
class ThumbnailListLoader;

class ThumbnailListView : public QListView
{
	Q_OBJECT
public:	
	typedef enum ViewPerspective{
		Axial,
		Coronal,
		Sagittal
	}ViewPerspective;

	explicit ThumbnailListView(QWidget *parent = nullptr);
	void setActiveAlbum(ViewPerspective perspective);

private:
	unsigned char m_AlbumID;
	ThumbnailProxyModel *m_ThumbnailProxyModel;
	ThumbnailListLoader *m_Loader;
};

#endif // THUMBNAILLISTVIEW_H
