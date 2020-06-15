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

static void setTrasnferFunction(mitk::DataNode::Pointer node)
{
	mitk::Image::Pointer image = dynamic_cast<mitk::Image *>(node->GetData());
	if (image.IsNotNull())
	{
		// Set the property "volumerendering" to the Boolean value "true"
		node->SetProperty("volumerendering", mitk::BoolProperty::New(true));

		// Create a transfer function to assign optical properties (color and opacity) to grey-values of the data
		mitk::TransferFunction::Pointer tf = mitk::TransferFunction::New();
		tf->InitializeByMitkImage(image);

		// Set the color transfer function AddRGBPoint(double x, double r, double g, double b)
		tf->GetColorTransferFunction()->AddRGBPoint(tf->GetColorTransferFunction()->GetRange()[0], 1.0, 0.0, 0.0);
		tf->GetColorTransferFunction()->AddRGBPoint(tf->GetColorTransferFunction()->GetRange()[1], 1.0, 1.0, 0.0);

		// Set the piecewise opacity transfer function AddPoint(double x, double y)
		tf->GetScalarOpacityFunction()->AddPoint(0, 0);
		tf->GetScalarOpacityFunction()->AddPoint(tf->GetColorTransferFunction()->GetRange()[1], 1);

		node->SetProperty("TransferFunction", mitk::TransferFunctionProperty::New(tf.GetPointer()));
	}
}

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

//	auto nodeSet = m_Tree->GetNodeSet();

//	std::cout <<"####\nLoading\n####"<< endl;
//	for (auto node : nodeSet)
//	{
//		if (node.IsNotNull())
//		{
//			QModelIndex index = m_Tree->GetIndex(node);
//			std::cout << "Index -->\trow: " << index.row() << "\tcol: " << index.column() << endl;
//			std::cout << "Name from node: " << node->GetName() << endl;
//			std::cout << "Name from index: " << m_Tree->GetNode(index)->GetName() << endl;
//		}
//	}

//	onNewNodeAdded();
	debugListNodesNamesFromTree();

	setTrasnferFunction(m_SetOfObjects->at(0));

	mitk::RenderingManager::GetInstance()->InitializeViewsByBoundingObjects(m_Storage);
	mitk::RenderingManager::GetInstance()->RequestUpdateAll();

	emit newDataLoadedEnd();
}

AppDataManager::AppDataManager()
	: QObject(qApp)
{
	m_Storage = mitk::StandaloneDataStorage::New();
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
