#include "Utils.h"

#include <vtkRenderLargeImage.h>
#include <vtkPNGWriter.h>

#include <QmitkRenderingManager.h>
#include <QmitkRenderWindow.h>

#include <mitkStandaloneDataStorage.h>
#include <mitkTransferFunction.h>
#include <mitkTransferFunctionProperty.h>
#include <mitkDataInteractor.h>

#include <QDir>

AppDataStorage::AppDataStorage()
{
	storage = mitk::StandaloneDataStorage::New();
	nodes = mitk::StandaloneDataStorage::SetOfObjects::New();
}

AppDataStorage::~AppDataStorage()
{
	storage->Delete();
	nodes->Delete();
}

void SaveSliceOrImageAsPNG(mitk::Image::Pointer image,
													 mitk::SliceNavigationController::ViewDirection viewDirection,
//													 mitk::Image::Pointer mask,
													 std::string dirPath,
													 bool pngFlag)
{
	std::string png;
	if(pngFlag)
		png = ".png";

	// Create a Standalone Datastorage for the single purpose of saving screenshots..
	mitk::StandaloneDataStorage::Pointer ds = mitk::StandaloneDataStorage::New();
	QmitkRenderWindow renderWindow;
	renderWindow.GetRenderer()->SetDataStorage(ds);

	auto nodeI = mitk::DataNode::New();
	nodeI->SetData(image);
//	auto nodeM = mitk::DataNode::New();
//	nodeM->SetData(mask);
	ds->Add(nodeI);
//	ds->Add(nodeM);

	auto geo = ds->ComputeBoundingGeometry3D(ds->GetAll());
	mitk::RenderingManager::GetInstance()->InitializeViews(geo);

	// Init slice variable
	mitk::SliceNavigationController::Pointer sliceNaviController = renderWindow.GetSliceNavigationController();
	sliceNaviController->SetViewDirection(viewDirection);
	sliceNaviController->Update();

	unsigned int numberOfSteps = 1;
	if (sliceNaviController)
	{
		// set slice variable index to the first position
		numberOfSteps = sliceNaviController->GetSlice()->GetSteps();
		sliceNaviController->GetSlice()->SetPos(0);
	}

	// WARNING: Workaround!!
	// renderWindow shows just a black screen, shouldn't be visible, but show() method seems to make some
	// necessary layout calculations.
	// The next line allow us to invoke show() without showing a window on screen.
	// Without this, the function crashes!
	// This approach seems to work here, but some bugs may appear depending on implementation (see source).
	// source: https://stackoverflow.com/a/3996525/5901259
	renderWindow.setAttribute(Qt::WA_DontShowOnScreen);
	renderWindow.show();

	vtkRenderWindow* renderWindow2 = renderWindow.GetVtkRenderWindow();
	mitk::BaseRenderer* baserenderer = mitk::BaseRenderer::GetInstance(renderWindow2);
	auto vtkRender = baserenderer->GetVtkRenderer();

	vtkRenderLargeImage* magnifier;
	vtkPNGWriter *fileWriter;

	for (unsigned int currentStep = 0; currentStep < numberOfSteps; ++currentStep)
	{
		if (sliceNaviController)
		{
			sliceNaviController->GetSlice()->SetPos(currentStep);
		}

		renderWindow.GetRenderer()->PrepareRender();

		vtkRender->GetRenderWindow()->WaitForCompletion();

		magnifier = vtkRenderLargeImage::New();
		magnifier->SetInput(vtkRender);
		magnifier->SetMagnification(3.0);

		std::stringstream ss;
		std::string tmpImageName;
		std::string currentStepStr = std::to_string(currentStep);

		ss << dirPath << std::string(5 - currentStepStr.length(), '0') + currentStepStr << png;
		ss >> tmpImageName;

		fileWriter = vtkPNGWriter::New();
		fileWriter->SetInputConnection(magnifier->GetOutputPort());
		fileWriter->SetFileName(tmpImageName.c_str());
		fileWriter->Write();
		fileWriter->Delete();
	}
}

QDir GenerateDirPath(const QFileInfo& fileInfo)
{
	return 	QDir(fileInfo.absoluteDir().absolutePath() + "/" + fileInfo.completeBaseName() + "-SlicesPng").absolutePath();
}

bool CreateSlicesPngDir(const QDir& dir)
{
	if(!dir.exists())
	{
		QDir dir1(dir.absolutePath() + "/axial");
		if(!dir1.exists())
			QDir().mkpath(dir1.absolutePath());

		QDir dir2(dir.absolutePath() + "/sagittal");
		if(!dir2.exists())
			QDir().mkpath(dir2.absolutePath());

		QDir dir3(dir.absolutePath() + "/coronal");
		if(!dir3.exists())
			QDir().mkpath(dir3.absolutePath());

		return true;
	}
	else
		return false;
}

void setTrasnferFunction(mitk::StandaloneDataStorage::SetOfObjects::Pointer dataNodes)
{
	mitk::DataNode::Pointer node = dataNodes->at(0);

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

