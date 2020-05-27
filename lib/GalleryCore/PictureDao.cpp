#include "PictureDao.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "Picture.h"
#include "DatabaseManager.h"

#include <QDebug>
#include <QSqlError>

using namespace std;

PictureDao::PictureDao(QSqlDatabase& database) :
    mDatabase(database)
{
}

void PictureDao::init() const
{
    if (!mDatabase.tables().contains("pictures")) {
        QSqlQuery query(mDatabase);
        query.exec(QString("CREATE TABLE pictures")
        + " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        + "album_id INTEGER, "
        + "url TEXT)");
        DatabaseManager::debugQuery(query);
    }
}

void PictureDao::addPictureInAlbum(int albumId, Picture& picture) const
{
    QSqlQuery query(mDatabase);
    query.prepare(QString("INSERT INTO pictures")
        + " (album_id, url)"
        + " VALUES ("
        + ":album_id, "
        + ":url"
        + ")");
    query.bindValue(":album_id", albumId);
    query.bindValue(":url", picture.fileUrl());
    query.exec();
    DatabaseManager::debugQuery(query);
    picture.setId(query.lastInsertId().toInt());
    picture.setAlbumId(albumId);
}

void PictureDao::addPicturesInAlbum(int albumId, QVector<Picture> pictures) const
{
	QSqlQuery query(mDatabase);
	query.prepare(QString("INSERT INTO pictures")
			+ " (album_id, url)"
			+ " VALUES ("
			+ ":album_id, "
			+ ":url"
			+ ")");

	QVariantList album_ids;
	QVariantList urls;
	for (int i=0; i < pictures.size() ; i++)
	{
		album_ids.append(albumId);
		urls.append(pictures.at(i).fileUrl());
	}

	query.bindValue(":album_id", album_ids);
	query.bindValue(":url", urls);

	if (!query.execBatch())
			qDebug() << query.lastError();
}

void PictureDao::removePicture(int id) const
{
    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM pictures WHERE id = (:id)");
    query.bindValue(":id", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

void PictureDao::removePicturesForAlbum(int albumId) const
{
    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM pictures WHERE album_id = (:album_id)");
    query.bindValue(":album_id", albumId);
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<unique_ptr<Picture>>> PictureDao::picturesForAlbum(int albumId) const
{
    QSqlQuery query(mDatabase);
    query.prepare("SELECT * FROM pictures WHERE album_id = (:album_id)");
    query.bindValue(":album_id", albumId);
    query.exec();
    DatabaseManager::debugQuery(query);
    unique_ptr<vector<unique_ptr<Picture>>> list(new vector<unique_ptr<Picture>>());
    while(query.next()) {
        unique_ptr<Picture> picture(new Picture());
        picture->setId(query.value("id").toInt());
        picture->setAlbumId(query.value("album_id").toInt());
        picture->setFileUrl(query.value("url").toString());
        list->push_back(move(picture));
    }
    return list;
}
