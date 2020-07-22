#include "Settings.h"
#include "ui_Settings.h"

Settings::Settings(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Settings)
{
	ui->setupUi(this);
}

Settings::~Settings()
{
	delete ui;
}
