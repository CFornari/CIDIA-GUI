#ifndef APPDATAMANAGER_H
#define APPDATAMANAGER_H

#include <mitkStandaloneDataStorage.h>

#include <QObject>
#include <QHash>

class QmitkDataStorageTreeModel;
class QmitkStdMultiWidget;

class AppDataManager : public QObject
{
	Q_OBJECT
public:
	static AppDataManager* GetInstance();

	void loadFile(const QString& filePath);

	mitk::StandaloneDataStorage::Pointer getDataStorage() const;
	void setStorage(const mitk::StandaloneDataStorage::Pointer* Storage);

	mitk::StandaloneDataStorage::SetOfObjects::Pointer getSetOfObjects() const;
	void setSetOfObjects(const mitk::StandaloneDataStorage::SetOfObjects::Pointer* setOfObjects);

	void debugListNodesNamesFromTree();

signals:
	void newDataLoadedStart();
	void newDataLoadedEnd();

protected:
	AppDataManager();
	~AppDataManager();

	void onNewNodeAdded();

private:
	static AppDataManager* m_Instance;

	mitk::StandaloneDataStorage::Pointer m_Storage;

	mitk::StandaloneDataStorage::SetOfObjects::Pointer m_SetOfObjects;

	QmitkDataStorageTreeModel *m_Tree;
	QHash<QString, mitk::DataNode::Pointer> m_NodeList;
};

#endif // APPDATAMANAGER_H
