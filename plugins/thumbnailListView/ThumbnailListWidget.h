#ifndef THUMBNAILLISTWIDGET_H
#define THUMBNAILLISTWIDGET_H

#include <QFrame>

class QTabBar;
class ThumbnailListView;

class ThumbnailListWidget : public QWidget
{
	Q_OBJECT
public:
	ThumbnailListWidget(QWidget *parent);

signals:
	void loadPictures(const QString& dirAbsolutePath);

public slots:
	void setActiveView(int index);

private:
	QTabBar *m_TabList;
	ThumbnailListView *m_ThumbnailListView ;
};

#endif // THUMBNAILLISTWIDGET_H
