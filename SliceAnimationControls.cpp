//#include "SliceAnimationControls.h"

//#include <QVBoxLayout>
//#include <QTimer>
//#include <QPushButton>

//SliceAnimationControls::SliceAnimationControls(QWidget *parent,
//																							 mitk::StandaloneDataStorage::Pointer dataStorage,
//																							 mitk::Image::Pointer image)
//	: QWidget(parent),
//{
//	m_Slices.axial = 0;
//	m_Slices.sagittal = 0;
//	m_Slices.coronal = 0;

//	m_DataStorage = dataStorage;
//	m_SliceNavigator->SetViewDirection(mitk::SliceNavigationController::Axial);
//	m_Slices.axial = m_Image->GetSlicedGeometry()->GetSlices();
//	m_SliceNavigator->Update();

//	QVBoxLayout *vlayout = new QVBoxLayout;
//	setLayout(vlayout);

//	QPushButton *btnStart = new QPushButton;
//	btnStart->setText("Start");
//	vlayout->addWidget(btnStart);

//	m_Timer = new QTimer(this);
//	connect(m_Timer, &QTimer::timeout, this, &SliceAnimationControls::incrementStep);
//	m_Timer->start(5);
//}

//void SliceAnimationControls::incrementStep()
//{

//}

