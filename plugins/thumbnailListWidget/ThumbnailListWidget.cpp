#include "ThumbnailListWidget.h"

#include "ThumbnailListView.h"

#include <QTabBar>
#include <QVBoxLayout>

#include <QDebug>

ThumbnailListWidget::ThumbnailListWidget(QWidget *parent)
	:QWidget(parent)
{
	QFrame *listWidget = new QFrame(parent);
	QVBoxLayout *listLayout = new QVBoxLayout;
	listWidget->setLayout(listLayout);

	m_TabList = new QTabBar(listWidget);
	m_TabList->addTab("Axial");
	m_TabList->addTab("Sagittal");
	m_TabList->addTab("Coronal");
	listLayout->addWidget(m_TabList);

	m_ThumbnailListView = new ThumbnailListView(listWidget);
	m_ThumbnailListView->setActiveAlbum(ThumbnailListView::Axial);
	listLayout->addWidget(m_ThumbnailListView);

	connect(m_TabList, &QTabBar::tabBarClicked, this, &ThumbnailListWidget::setActiveView);
	connect(this, &ThumbnailListWidget::loadPictures,
					m_ThumbnailListView, &ThumbnailListView::loadPictures);
}

void ThumbnailListWidget::setActiveView(int index)
{
	switch (index)
	{
	case 0:
			m_ThumbnailListView->setActiveAlbum(ThumbnailListView::Axial);
		break;
	case 1:
			m_ThumbnailListView->setActiveAlbum(ThumbnailListView::Sagittal);
		break;
	case 2:
			m_ThumbnailListView->setActiveAlbum(ThumbnailListView::Coronal);
		break;
	}
}
