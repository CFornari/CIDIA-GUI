#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QmitkStdMultiWidget;
class QmitkMovieMakerView;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	Ui::MainWindow *ui;
	QmitkStdMultiWidget *m_MultiWidget;
	QmitkMovieMakerView *m_View;
};
#endif // MAINWINDOW_H
