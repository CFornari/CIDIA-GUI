#ifndef THUMBNAILLISTWIDGET_H
#define THUMBNAILLISTWIDGET_H

#include "ThumbnailListWidgetInterface.h"

#include <QFrame>

class QTabBar;
class ThumbnailListView;

class ThumbnailListWidget : public QWidget, ThumbnailListWidgetInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID ThumbnailListWidgetPlugin_iid FILE "plug.json")
	Q_INTERFACES(ThumbnailListWidgetInterface)

public:
	explicit ThumbnailListWidget(QWidget *parent = nullptr);

public slots:
	void setActiveView(int index);

signals:
	void loadPictures(const QString& dirAbsolutePath);

private:
	QTabBar *m_TabList;
	ThumbnailListView *m_ThumbnailListView ;
};

#endif // THUMBNAILLISTWIDGET_H
