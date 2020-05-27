#ifndef UTILS_H
#define UTILS_H

#include <mitkImage.h>
#include <mitkSliceNavigationController.h>

class QFileInfo;
class QDir;

void SaveSliceOrImageAsPNG(mitk::Image::Pointer image,
													 mitk::SliceNavigationController::ViewDirection viewDirection,
//													 mitk::Image::Pointer mask,
													 std::string dirPath,
													 bool pngFlag = true);

QDir GenerateDirPath(const QFileInfo& fileInfo);

bool CreateSlicesPngDir(const QDir& dir);

#endif // UTILS_H
