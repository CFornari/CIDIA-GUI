#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <mitkStandaloneDataStorage.h>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QmitkStdMultiWidget;
class AppDataManager;
class DataManagerView;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

public slots:
	void openFileDialog();
	void onNewDataLoaded();

private:
	void loadImage(const QString filePath);

private:
	Ui::MainWindow *ui;

	AppDataManager *m_AppData;
	DataManagerView *m_DataManagerView;
	QmitkStdMultiWidget *m_MultiWidget;
};
#endif // MAINWINDOW_H
