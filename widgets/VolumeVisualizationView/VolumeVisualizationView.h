#ifndef VOLUMEVISUALIZATIONVIEWCONTROLS_H
#define VOLUMEVISUALIZATIONVIEWCONTROLS_H

#include <QWidget>

#include <mitkWeakPointer.h>

#include <mitkImage.h>

#include "mitkDataStorage.h"

#include <QmitkDataStorageListModel.h>
#include <QmitkDataStorageComboBox.h>
#include <QmitkTransferFunctionWidget.h>

namespace Ui {
	class VolumeVisualizationView;
}

class VolumeVisualizationView : public QWidget
{
	Q_OBJECT

public:
	VolumeVisualizationView(QWidget* parent);

	~VolumeVisualizationView() override;

	void SetDataNode(const mitk::DataNode::Pointer node);

protected slots:

	void OnMitkInternalPreset( int mode );

	void OnEnableRendering( bool state );
	void OnRenderMode( int mode );
	void OnBlendMode(int mode);

private:

	mitk::WeakPointer<mitk::DataNode> m_SelectedNode;

	void UpdateInterface();
//	void NodeRemoved(const mitk::DataNode* node) override;

	Ui::VolumeVisualizationView *ui;
};

#endif // VOLUMEVISUALIZATIONVIEWCONTROLS_H
