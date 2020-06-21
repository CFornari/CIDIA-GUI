#ifndef CONCEPT2_H
#define CONCEPT2_H

#include <QWidget>

namespace Ui {
	class Concept2;
}

class QmitkStdMultiWidget;
class AppDataManager;

class Concept2 : public QWidget
{
	Q_OBJECT

public:
	explicit Concept2(QWidget *parent = nullptr);
	~Concept2();

public slots:
	void onVisibilityChanged(bool visible);

private:
	Ui::Concept2 *ui;
	QmitkStdMultiWidget *m_MultiWidget;

	AppDataManager *m_AppData;
};

#endif // CONCEPT2_H
