#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void initDataManagerView();
	void addBorderShadowGloomEffect(QWidget* widget);

public slots:
	void openFileDialog();
	void onDataManagerButtonToggled(bool state);
	void showDataManager();
	void hideDataManager();

private:
	Ui::MainWindow *ui;

	QWidget *m_DataManagerView;
	QRect m_DataManagerShowPosition;
	QRect m_DataManagerHidePosition;
};
#endif // MAINWINDOW_H
