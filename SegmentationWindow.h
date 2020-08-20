#ifndef SEGMENTATION_WINDOW_H
#define SEGMENTATION_WINDOW_H

#include <QWidget>

namespace Ui {
	class SegmentationWindow;
}

class QmitkStdMultiWidget;
class AppDataManager;
class QmitkMultiLabelSegmentationView;

class SegmentationWindow : public QWidget
{
	Q_OBJECT

public:
	explicit SegmentationWindow(QWidget *parent = nullptr);
	~SegmentationWindow();

public slots:

private:
	virtual void showEvent(QShowEvent *e) override;
	virtual void hideEvent(QHideEvent *e) override;

	Ui::SegmentationWindow *ui;
	QmitkStdMultiWidget *m_MultiWidget;

	AppDataManager *m_AppData;
	QmitkMultiLabelSegmentationView *m_SegView;
};

#endif // SEGMENTATION_WINDOW_H
