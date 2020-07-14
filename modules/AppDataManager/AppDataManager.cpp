#include "AppDataManager.h"

#include <QmitkStdMultiWidget.h>
#include <QmitkRenderingManager.h>
#include <QmitkRenderWindow.h>
#include <QmitkIOUtil.h>

#include <QmitkCustomVariants.h>
#include <QmitkDataStorageFilterProxyModel.h>
#include <QmitkDataStorageTreeModel.h>
#include <QmitkNodeDescriptorManager.h>

#include <mitkNodePredicateProperty.h>
#include <mitkTransferFunctionProperty.h>
#include <mitkIOUtil.h>

#include <QApplication>

AppDataManager* AppDataManager::m_Instance = nullptr;

AppDataManager* AppDataManager::GetInstance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new AppDataManager;
	}
	return m_Instance;
}

void AppDataManager::loadFile(const QString& filePath)
{
	emit newDataLoadedStart();
	try
	{
		m_SetOfObjects = mitk::IOUtil::Load(filePath.toStdString(), *m_StorageMaster);
	}
	catch (const mitk::Exception& e)
	{
		MITK_INFO << e;
		return;
	}

	mitk::RenderingManager::GetInstance()->RequestUpdateAll();

	onNewNodeAdded();

	emit newDataLoadedEnd();

	debugListNodesNamesFromFiles();
	debugListNodesNamesFromTree();
}

bool AppDataManager::createDataStorageFromMaster(const QString& name)
{
	if(m_StorageHash.contains(name))
	{
		std::cout << "AppDataManager::createDataStorageFromMaster(): Name already exists." << endl;
		return false; // fail
	}

	mitk::StandaloneDataStorage::Pointer storage = mitk::StandaloneDataStorage::New();

	for(auto node : m_FileNodeList)
	{
		storage->Add(node);
	}

	m_StorageHash.insert(name, storage);

	return true;	// success
}


mitk::StandaloneDataStorage::Pointer AppDataManager::getDataStorageMaster() const
{
	return m_StorageMaster;
}

mitk::StandaloneDataStorage::Pointer AppDataManager::getDataStorageByName(const QString& name) const
{
	if(m_StorageHash.contains(name))
	{
		return m_StorageHash[name];
	}
	else
	{
		std::cout << "AppDataManager::getDataStorageByName(): name doesn't exists in hash, "
								 "returning mitk::StandaloneDataStorage::New() to avoid program abortion." << endl;
		return mitk::StandaloneDataStorage::New();
	}
}

QHash<QString, mitk::DataNode::Pointer> AppDataManager::getFileNodeList() const
{
	return m_FileNodeHash;
}

unsigned int AppDataManager::getNumberOfFiles()
{
	return m_FileNodeHash.size();
}

bool AppDataManager::FilesExist()
{
	if(m_FileNodeHash.isEmpty())
		return false;
	else
		return true;
}

void AppDataManager::debugListNodesNamesFromFiles()
{
	unsigned int i = 0;
	std::cout << "debugListNodesNamesFromFiles: " << endl;
	for(auto node : m_FileNodeHash)
	{
		std::cout << "Node(" << i << "): " << node->GetName() <<endl;
		i++;
	}
}

void AppDataManager::debugListNodesNamesFromTree()
{
	QList<mitk::DataNode::Pointer> nodeList = m_Tree->GetNodeSet();
	unsigned int i = 0;
	std::cout << "debugListNodesNamesFromTree: " << endl;
	for(auto node : nodeList)
	{
		std::cout << "Node(" << i << "): " << node->GetName() <<endl;
		i++;
	}
}

AppDataManager::AppDataManager()
	: QObject(qApp)
{
	m_StorageMaster = mitk::StandaloneDataStorage::New();
	m_SetOfObjects = mitk::StandaloneDataStorage::SetOfObjects::New();

	m_Tree = new QmitkDataStorageTreeModel(m_StorageMaster, true, this);
}

AppDataManager::~AppDataManager()
{
}

void AppDataManager::onNewNodeAdded()
{
	mitk::DataNode::Pointer node = m_Tree->GetNodeSet().first();

	std::string name = node->GetName();

	m_FileNodeHash.insert(QString::fromUtf8(name.data(),name.size()), node);
	m_FileNodeList.append(node);

	for(auto storage : m_StorageHash)
	{
		storage->Add(node);
	}
}

