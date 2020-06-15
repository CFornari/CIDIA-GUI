#ifndef UTILS_H
#define UTILS_H

#include <mitkStandaloneDataStorage.h>
#include <mitkImage.h>
#include <mitkSliceNavigationController.h>

class QFileInfo;
class QDir;

struct AppDataStorage
{
	AppDataStorage();
	~AppDataStorage();

	mitk::StandaloneDataStorage::Pointer storage;
	mitk::StandaloneDataStorage::SetOfObjects::Pointer nodes;
};


void SaveSliceOrImageAsPNG(mitk::Image::Pointer image,
													 mitk::SliceNavigationController::ViewDirection viewDirection,
//													 mitk::Image::Pointer mask,
													 std::string dirPath,
													 bool pngFlag = true);

QDir GenerateDirPath(const QFileInfo& fileInfo);

bool CreateSlicesPngDir(const QDir& dir);

void setTrasnferFunction(mitk::StandaloneDataStorage::SetOfObjects::Pointer dataNodes);

#endif // UTILS_H
