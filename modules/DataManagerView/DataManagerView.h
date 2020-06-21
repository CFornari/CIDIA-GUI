#ifndef DATAMANAGERVIEW_H
#define DATAMANAGERVIEW_H

// mitk core
#include <mitkNodePredicateBase.h>

// qt
#include <QItemSelection>
#include <QWidget>

// forward declarations
class AppDataManager;

class QAction;
class QModelIndex;
class QTreeView;
class QSignalMapper;

class QmitkDnDFrameWidget;
class QmitkDataStorageTreeModel;
class QmitkDataManagerItemDelegate;
class QmitkDataStorageFilterProxyModel;

namespace Ui {
	class DataManagerView;
}


class DataManagerView : public QWidget
{
	Q_OBJECT

public:
	explicit DataManagerView(QWidget *parent = nullptr);
	~DataManagerView();

public slots:
	void OnNodeVisibilityChanged();
	void NodeTreeViewRowsInserted(const QModelIndex& parent, int /*start*/, int /*end*/);
	void NodeTreeViewRowsRemoved(const QModelIndex& /*parent*/, int /*start*/, int /*end*/);
	void NodeSelectionChanged(const QItemSelection& /*selected*/, const QItemSelection& /*deselected*/);

protected:
	QmitkDataStorageTreeModel* m_NodeTreeModel;
	QmitkDataStorageFilterProxyModel* m_FilterModel;
	mitk::NodePredicateBase::Pointer m_HelperObjectFilterPredicate;
	mitk::NodePredicateBase::Pointer m_NodeWithNoDataFilterPredicate;

	///
	/// \brief The Table view to show the selected nodes.
	///
	QTreeView* m_NodeTreeView;
	///
	/// \brief The context menu that shows up when right clicking on a node.
	///
//	QmitkDataNodeContextMenu* m_DataNodeContextMenu;

	/// saves the current amount of rows shown in the data manager
	size_t m_CurrentRowCount;

	QmitkDataManagerItemDelegate* m_ItemDelegate;

private:
	Ui::DataManagerView *ui;
	AppDataManager *m_AppData;
};

#endif // DATAMANAGERVIEW_H
