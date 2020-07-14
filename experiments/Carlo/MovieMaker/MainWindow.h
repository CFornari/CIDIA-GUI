#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QmitkStdMultiWidget;
class QmitkMovieMakerView;
class QRenderWidget;
class Qmitk2DRenderWidget;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

public slots:
	void onRenderWindowChanged(int renderWindow);

private:
	virtual void showEvent(QShowEvent *e) override;

	Ui::MainWindow *ui;

	QmitkMovieMakerView *m_View;

	Qmitk2DRenderWidget *viewAxial;
	QRenderWidget *viewSagittal;
	QRenderWidget *viewCoronal;
	QRenderWidget *view3D;
};
#endif // MAINWINDOW_H
