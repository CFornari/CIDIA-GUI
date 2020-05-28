#ifndef THUMBNAILLISTVIEW_H
#define THUMBNAILLISTVIEW_H

#include <QListView>

class AlbumModel;
class PictureModel;
class ThumbnailProxyModel;

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

public slots:
	void loadPictures(const QString& dirAbsolutePath);

signals:

protected:
	void addPictures(const QString& filePath, const QStringList& fileNames, int albumID);

protected:
	AlbumModel *m_AlbumModel;
	PictureModel *m_PictureModel;
	ThumbnailProxyModel *m_ThumbnailProxyModel;

};

#endif // THUMBNAILLISTVIEW_H
