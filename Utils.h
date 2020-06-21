#ifndef UTILS_H
#define UTILS_H

#include <mitkStandaloneDataStorage.h>
#include <mitkImage.h>
#include <mitkSliceNavigationController.h>

class QFileInfo;
class QDir;
class QWidget;

void SaveSliceOrImageAsPNG(mitk::Image::Pointer image,
													 mitk::SliceNavigationController::ViewDirection viewDirection,
//													 mitk::Image::Pointer mask,
													 std::string dirPath,
													 bool pngFlag = true);

QDir GenerateDirPath(const QFileInfo& fileInfo);

bool CreateSlicesPngDir(const QDir& dir);

void SetTrasnferFunction(mitk::StandaloneDataStorage::SetOfObjects::Pointer dataNodes);

namespace Widgets
{
	void MoveCenter(QWidget* widget);

	void MoveTop(QWidget* widget);
	void MoveBaseTop(QWidget *widget);
}


#endif // UTILS_H
