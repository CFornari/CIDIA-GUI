#ifndef THUMBNAILLISTWIDGETINTERFACE_H
#define THUMBNAILLISTWIDGETINTERFACE_H

#include <QWidget>
#include <QtPlugin>

class ThumbnailListWidgetInterface
{
public:
		virtual ~ThumbnailListWidgetInterface() {}

public slots:
	virtual void setActiveView(int index) = 0;

signals:
	void loadPictures(const QString& dirAbsolutePath);
};

QT_BEGIN_NAMESPACE

#define ThumbnailListWidgetPlugin_iid "ufrgs.covid-gui.ThumbnailListWidgetPlugin"

Q_DECLARE_INTERFACE(ThumbnailListWidgetInterface, ThumbnailListWidgetPlugin_iid)
QT_END_NAMESPACE

#endif // THUMBNAILLISTWIDGETINTERFACE_H
