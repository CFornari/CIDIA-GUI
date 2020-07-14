#ifndef APPDATAMANAGER_H
#define APPDATAMANAGER_H

#include <mitkStandaloneDataStorage.h>

#include <QObject>
#include <QHash>
#include <QVector>

class QmitkDataStorageTreeModel;
class QmitkStdMultiWidget;

class AppDataManager : public QObject
{
	Q_OBJECT
public:
	static AppDataManager* GetInstance();

	void loadFile(const QString& filePath);

	bool createDataStorageFromMaster(const QString& name);

	mitk::StandaloneDataStorage::Pointer getDataStorageMaster() const;
	mitk::StandaloneDataStorage::Pointer getDataStorageByName(const QString& name) const;

	unsigned int getNumberOfFiles();
	bool FilesExist();

	void debugListNodesNamesFromTree();
	void debugListNodesNamesFromFiles();

	QHash<QString, mitk::DataNode::Pointer> getFileNodeList() const;

signals:
	void newDataLoadedStart();
	void newDataLoadedEnd();

protected:
	AppDataManager();
	~AppDataManager();

	void onNewNodeAdded();

private:
	static AppDataManager* m_Instance;

	mitk::StandaloneDataStorage::Pointer m_StorageMaster;
	QHash<QString, mitk::StandaloneDataStorage::Pointer> m_StorageHash;

	mitk::StandaloneDataStorage::SetOfObjects::Pointer m_SetOfObjects;

	QmitkDataStorageTreeModel *m_Tree;
	QHash<QString, mitk::DataNode::Pointer> m_FileNodeHash;
	QList<mitk::DataNode::Pointer> m_FileNodeList;
};

#endif // APPDATAMANAGER_H
