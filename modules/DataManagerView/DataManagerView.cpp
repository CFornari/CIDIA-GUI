#include "DataManagerView.h"
#include "ui_DataManagerView.h"

#include "AppDataManager.h"
#include "QmitkDataManagerItemDelegate.h"

// mitk core
#include <mitkCommon.h>
#include <mitkCoreObjectFactory.h>
#include <mitkEnumerationProperty.h>
#include <mitkImageCast.h>
#include <mitkITKImageImport.h>
#include <mitkLookupTableProperty.h>
#include <mitkNodePredicateAnd.h>
#include <mitkNodePredicateData.h>
#include <mitkNodePredicateDataType.h>
#include <mitkNodePredicateNot.h>
#include <mitkNodePredicateOr.h>
#include <mitkNodePredicateProperty.h>
#include <mitkProperties.h>
#include <mitkRenderingModeProperty.h>

// qt widgets module
#include <QmitkCustomVariants.h>
#include <QmitkDataStorageFilterProxyModel.h>
#include <QmitkDataStorageTreeModel.h>
#include <QmitkIOUtil.h>
#include <QmitkNodeDescriptorManager.h>

// qt
#include <QTreeView>

DataManagerView::DataManagerView(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DataManagerView)
{
	ui->setupUi(this);

	m_CurrentRowCount = 0;
	m_AppData = AppDataManager::GetInstance();

	//# GUI
	m_NodeTreeModel = new QmitkDataStorageTreeModel(m_AppData->getDataStorageMaster(), true, this);
	// Prepare filters
	m_HelperObjectFilterPredicate = mitk::NodePredicateOr::New(
																		mitk::NodePredicateProperty::New("helper object", mitk::BoolProperty::New(true)),
																		mitk::NodePredicateProperty::New("hidden object", mitk::BoolProperty::New(true)));
	m_NodeWithNoDataFilterPredicate = mitk::NodePredicateData::New(nullptr);

	m_FilterModel = new QmitkDataStorageFilterProxyModel();
	m_FilterModel->setSourceModel(m_NodeTreeModel);
	m_FilterModel->AddFilterPredicate(m_HelperObjectFilterPredicate);
	m_FilterModel->AddFilterPredicate(m_NodeWithNoDataFilterPredicate);

	m_NodeTreeView = ui->treeView;
	m_NodeTreeView->setHeaderHidden(true);
	m_NodeTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	m_NodeTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_NodeTreeView->setAlternatingRowColors(true);
	m_NodeTreeView->setDragEnabled(true);
	m_NodeTreeView->setDropIndicatorShown(true);
	m_NodeTreeView->setAcceptDrops(true);
	m_NodeTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
	m_NodeTreeView->setModel(m_FilterModel);
	m_NodeTreeView->setTextElideMode(Qt::ElideMiddle);
//	m_NodeTreeView->installEventFilter(new QmitkNodeTableViewKeyFilter(this, GetDataStorage()));

//	m_ItemDelegate = new QmitkDataManagerItemDelegate(m_NodeTreeView);
//	m_NodeTreeView->setItemDelegate(m_ItemDelegate);

	connect(m_NodeTreeModel, SIGNAL(rowsInserted(const QModelIndex&, int, int)), this, SLOT(NodeTreeViewRowsInserted(const QModelIndex&, int, int)));
	connect(m_NodeTreeModel, SIGNAL(rowsRemoved(const QModelIndex&, int, int)), this, SLOT(NodeTreeViewRowsRemoved(const QModelIndex&, int, int)));
	connect(m_NodeTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(NodeSelectionChanged(const QItemSelection &, const QItemSelection &)));
	connect(m_NodeTreeModel, &QmitkDataStorageTreeModel::nodeVisibilityChanged, this, &DataManagerView::OnNodeVisibilityChanged);

//	QGridLayout* dndFrameWidgetLayout = new QGridLayout;
//	dndFrameWidgetLayout->addWidget(m_NodeTreeView, 0, 0);
//	dndFrameWidgetLayout->setContentsMargins(0, 0, 0, 0);

//	m_DnDFrameWidget = new QmitkDnDFrameWidget(m_Parent);
//	m_DnDFrameWidget->setLayout(dndFrameWidgetLayout);

//	QVBoxLayout* layout = new QVBoxLayout(parent);
//	layout->addWidget(m_DnDFrameWidget);
//	layout->setContentsMargins(0, 0, 0, 0);
}

DataManagerView::~DataManagerView()
{
	delete ui;
}

void DataManagerView::OnNodeVisibilityChanged()
{
	auto selectedNodes = m_AppData->getDataStorageMaster()->GetSubset(mitk::NodePredicateProperty::New("selected", mitk::BoolProperty::New(true)));
	for (auto node : *selectedNodes)
	{
		if (node.IsNotNull())
		{
			std::cout << node->GetName() << endl;
//			node->SetVisibility(true);
//			node->SetSelected(false);
		}
	}

	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void DataManagerView::NodeTreeViewRowsInserted(const QModelIndex& parent, int /*start*/, int /*end*/)
{
	QModelIndex viewIndex = m_FilterModel->mapFromSource(parent);
	m_NodeTreeView->setExpanded(viewIndex, true);

	// a new row was inserted
	if (m_CurrentRowCount == 0 && m_NodeTreeModel->rowCount() == 1)
	{
		m_CurrentRowCount = m_NodeTreeModel->rowCount();
	}
}

void DataManagerView::NodeTreeViewRowsRemoved(const QModelIndex& /*parent*/, int /*start*/, int /*end*/)
{
	m_CurrentRowCount = m_NodeTreeModel->rowCount();
}

void DataManagerView::NodeSelectionChanged(const QItemSelection& selection, const QItemSelection& /*deselected*/)
{
//  auto nodeSet = m_NodeTreeModel->GetNodeSet();

//  for (auto node : nodeSet)
//  {
//    if (node.IsNotNull())
//    {
//			for (auto selected : selectedIndex)
//			{
//			m_NodeTreeModel->data(selected);
//      node->SetSelected(selectedNodes.contains(node));
//			}
//    }
//  }
//	std::cout <<"####\nSelection\n####"<< endl;

	QModelIndexList indexList = selection.indexes();
	QList<mitk::DataNode::Pointer> nodeSet = m_NodeTreeModel->GetNodeSet();

	for (auto index : indexList)
	{
		if(index.isValid())
		{
			for (auto node : nodeSet)
			{
				if (node.IsNotNull())
				{
					QModelIndex treeIndex = m_NodeTreeModel->GetIndex(node);
					if(treeIndex.row() == index.row())
					{
//						std::cout << "Index -->\trow: " << index.row() << "\tcol: " << index.column() << endl;
						std::cout << "Selected: " << node->GetName() << endl;
//						std::cout << "Name from index: " << m_NodeTreeModel->GetNode(treeIndex)->GetName() << endl;
						node->SetSelected(true);
					}
					else
					{
						node->SetSelected(false);
					}
				}
			}
		}
	}
}
