#ifndef THUMBNAILLISTLOADER_H
#define THUMBNAILLISTLOADER_H

#include <QObject>

class AlbumModel;
class PictureModel;

class ThumbnailListLoader : public QObject
{
public:
	static ThumbnailListLoader* GetInstance();

	void addPictures(const QString& filePath,
									 const QStringList& fileNames,
									 int albumID);

	PictureModel* getPictureModel() const;

public slots:
		void loadPictures(const QString& dirAbsolutePath);

protected:
	ThumbnailListLoader();

private:
	static ThumbnailListLoader* m_Instance;

	AlbumModel *m_AlbumModel;
	PictureModel *m_PictureModel;
};

#endif // THUMBNAILLISTLOADER_H
