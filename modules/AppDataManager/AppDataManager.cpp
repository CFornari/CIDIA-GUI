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
		m_SetOfObjects = mitk::IOUtil::Load(filePath.toStdString(), *m_Storage);
	}
	catch (const mitk::Exception& e)
	{
		MITK_INFO << e;
		return;
	}

	onNewNodeAdded();

	mitk::RenderingManager::GetInstance()->InitializeViewsByBoundingObjects(m_Storage);
	mitk::RenderingManager::GetInstance()->RequestUpdateAll();

	emit newDataLoadedEnd();
}

AppDataManager::AppDataManager()
	: QObject(qApp)
{
	m_Storage = mitk::StandaloneDataStorage::New();
	m_DummyStorage = mitk::StandaloneDataStorage::New();
	m_SetOfObjects = mitk::StandaloneDataStorage::SetOfObjects::New();

	m_Tree = new QmitkDataStorageTreeModel(m_Storage, true, this);
}

AppDataManager::~AppDataManager()
{
}

void AppDataManager::onNewNodeAdded()
{
	QList<mitk::DataNode::Pointer> nodeSet = m_Tree->GetNodeSet();
	std::string name = nodeSet.first()->GetName();
	m_NodeList.insert(QString::fromUtf8(name.data(),name.size()),nodeSet.first());
}

mitk::StandaloneDataStorage::Pointer AppDataManager::getDummyStorage() const
{
	return m_DummyStorage;
}

mitk::StandaloneDataStorage::SetOfObjects::Pointer AppDataManager::getSetOfObjects() const
{
	return m_SetOfObjects;
}

void AppDataManager::setSetOfObjects(const mitk::StandaloneDataStorage::SetOfObjects::Pointer* setOfObjects)
{
	//	m_SetOfObjects = setOfObjects;
}

void AppDataManager::debugListNodesNamesFromTree()
{
	QList<mitk::DataNode::Pointer> nodeList = m_Tree->GetNodeSet();
	unsigned int i = 0;
	for(auto node : nodeList)
	{
		std::cout << "Node(" << i << "): " << node->GetName() <<endl;
		i++;
	}
}

mitk::StandaloneDataStorage::Pointer AppDataManager::getDataStorage() const
{
	return m_Storage;
}

void AppDataManager::setStorage(const mitk::StandaloneDataStorage::Pointer* storage)
{
	m_Storage = *storage;
}
