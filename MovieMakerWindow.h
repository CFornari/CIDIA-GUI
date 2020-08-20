#ifndef MOVIEMAKER_WINDOW_H
#define MOVIEMAKER_WINDOW_H

#include <QWidget>

class AppDataManager;
class Qmitk2DRenderWidget;
class Qmitk3DRenderWidget;
class QmitkMovieMakerView;

namespace Ui {
	class MovieMakerWindow;
}

class MovieMakerWindow : public QWidget
{
	Q_OBJECT

public:
	explicit MovieMakerWindow(QWidget *parent = nullptr);
	~MovieMakerWindow();

public slots:
	void onNewDataLoadedEnd();
	void onRenderWindowChanged(int renderWindow);
	void onAddAnimationButtonClicked();
	void onRemoveAnimationButtonClicked();

private:
	void showEvent(QShowEvent *e) override;
	void hideEvent(QHideEvent *event) override;

	void resetViews();

	Ui::MovieMakerWindow *ui;

	AppDataManager *m_AppData;

	Qmitk2DRenderWidget *m_ViewAxial;
	Qmitk2DRenderWidget *m_ViewSagittal;
	Qmitk2DRenderWidget *m_ViewCoronal;
	Qmitk3DRenderWidget *m_View3D;

	QmitkMovieMakerView *m_MovieView;
};

#endif // MOVIEMAKER_WINDOW_H
