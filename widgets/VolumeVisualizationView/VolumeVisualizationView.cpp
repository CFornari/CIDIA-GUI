/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#include "VolumeVisualizationView.h"
#include "ui_VolumeVisualizationView.h"

#include <QComboBox>

#include <vtkVersionMacros.h>
#include <vtkSmartVolumeMapper.h>

#include <mitkProperties.h>
#include <mitkNodePredicateDataType.h>

#include <mitkTransferFunction.h>
#include <mitkTransferFunctionProperty.h>
#include <mitkTransferFunctionInitializer.h>
#include "mitkHistogramGenerator.h"
#include "QmitkPiecewiseFunctionCanvas.h"
#include "QmitkColorTransferFunctionCanvas.h"

#include "mitkBaseRenderer.h"

#include "mitkVtkVolumeRenderingProperty.h"

#include <QToolTip>

//#include <.h>

enum {DEFAULT_RENDERMODE = 0, RAYCAST_RENDERMODE = 1, GPU_RENDERMODE = 2};

VolumeVisualizationView::VolumeVisualizationView(QWidget *parent)
	: QWidget(parent),
		ui(new Ui::VolumeVisualizationView)
{
	ui->setupUi(this);

	// Fill the tf presets in the generator widget
	std::vector<std::string> names;
	mitk::TransferFunctionInitializer::GetPresetNames(names);
	for (std::vector<std::string>::const_iterator it = names.begin();
			 it != names.end(); ++it)
	{
		ui->m_TransferFunctionGeneratorWidget->AddPreset(QString::fromStdString(*it));
	}

	// see enum in vtkSmartVolumeMapper
	ui->m_RenderMode->addItem("Default");
	ui->m_RenderMode->addItem("RayCast");
	ui->m_RenderMode->addItem("GPU");

	// see vtkVolumeMapper::BlendModes
	ui->m_BlendMode->addItem("Comp");
	ui->m_BlendMode->addItem("Max");
	ui->m_BlendMode->addItem("Min");
	ui->m_BlendMode->addItem("Avg");
	ui->m_BlendMode->addItem("Add");

	connect( ui->m_EnableRenderingCB, SIGNAL( toggled(bool) ),this, SLOT( OnEnableRendering(bool) ));
	connect(ui->m_RenderMode, SIGNAL(activated(int)), this, SLOT(OnRenderMode(int)));
	connect(ui->m_BlendMode, SIGNAL(activated(int)), this, SLOT(OnBlendMode(int)));

	connect( ui->m_TransferFunctionGeneratorWidget, SIGNAL( SignalUpdateCanvas( ) ),   ui->m_TransferFunctionWidget, SLOT( OnUpdateCanvas( ) ) );
	connect( ui->m_TransferFunctionGeneratorWidget, SIGNAL(SignalTransferFunctionModeChanged(int)), SLOT(OnMitkInternalPreset(int)));

	ui->m_EnableRenderingCB->setEnabled(false);
	ui->m_BlendMode->setEnabled(false);
	ui->m_RenderMode->setEnabled(false);
	ui->m_TransferFunctionWidget->setEnabled(false);
	ui->m_TransferFunctionGeneratorWidget->setEnabled(false);

	ui->m_SelectedImageLabel->hide();
	ui->m_ErrorImageLabel->hide();
}

VolumeVisualizationView::~VolumeVisualizationView()
{
}


void VolumeVisualizationView::OnMitkInternalPreset(int mode)
{
	if (m_SelectedNode.IsExpired()) return;

	auto node = m_SelectedNode.Lock();
	mitk::TransferFunctionProperty::Pointer transferFuncProp;
	if (node->GetProperty(transferFuncProp, "TransferFunction"))
	{
		//first item is only information
		if( --mode == -1 )
			return;

		// -- Creat new TransferFunction
		mitk::TransferFunctionInitializer::Pointer tfInit = mitk::TransferFunctionInitializer::New(transferFuncProp->GetValue());
		tfInit->SetTransferFunctionMode(mode);
		mitk::RenderingManager::GetInstance()->RequestUpdateAll();
		ui->m_TransferFunctionWidget->OnUpdateCanvas();
	}
}

void VolumeVisualizationView::SetDataNode(const mitk::DataNode::Pointer node)
{
	bool weHadAnImageButItsNotThreeDeeOrFourDee = false;

	if( node.IsNotNull() && dynamic_cast<mitk::Image*>(node->GetData()) )
	{
		if( dynamic_cast<mitk::Image*>(node->GetData())->GetDimension()>=3 )
		{}
		else
		{
			weHadAnImageButItsNotThreeDeeOrFourDee = true;
		}
	}

	if( node.IsNotNull() )
	{
		ui->m_NoSelectedImageLabel->hide();
		ui->m_ErrorImageLabel->hide();
		ui->m_SelectedImageLabel->show();

		std::string  infoText;

		if (node->GetName().empty())
			infoText = std::string("Selected Image: [currently selected image has no name]");
		else
			infoText = std::string("Selected Image: ") + node->GetName();

		ui->m_SelectedImageLabel->setText( QString( infoText.c_str() ) );

		m_SelectedNode = node;
	}
	else
	{
		if(weHadAnImageButItsNotThreeDeeOrFourDee)
		{
			ui->m_NoSelectedImageLabel->hide();
			ui->m_ErrorImageLabel->show();
			std::string  infoText;
			infoText = std::string("only 3D or 4D images are supported");
			ui->m_ErrorImageLabel->setText( QString( infoText.c_str() ) );
		}
		else
		{
			ui->m_SelectedImageLabel->hide();
			ui->m_ErrorImageLabel->hide();
			ui->m_NoSelectedImageLabel->show();
		}

		m_SelectedNode = nullptr;
	}

	UpdateInterface();
}


void VolumeVisualizationView::UpdateInterface()
{
	if(m_SelectedNode.IsExpired())
	{
		// turnoff all
		ui->m_EnableRenderingCB->setChecked(false);
		ui->m_EnableRenderingCB->setEnabled(false);

		ui->m_BlendMode->setCurrentIndex(0);
		ui->m_BlendMode->setEnabled(false);

		ui->m_RenderMode->setCurrentIndex(0);
		ui->m_RenderMode->setEnabled(false);

		ui->m_TransferFunctionWidget->SetDataNode(nullptr);
		ui->m_TransferFunctionWidget->setEnabled(false);

		ui->m_TransferFunctionGeneratorWidget->SetDataNode(nullptr);
		ui->m_TransferFunctionGeneratorWidget->setEnabled(false);
		return;
	}

	bool enabled = false;
	auto selectedNode = m_SelectedNode.Lock();

	selectedNode->GetBoolProperty("volumerendering",enabled);
	ui->m_EnableRenderingCB->setEnabled(true);
	ui->m_EnableRenderingCB->setChecked(enabled);

	if(!enabled)
	{
		// turnoff all except volumerendering checkbox
		ui->m_BlendMode->setCurrentIndex(0);
		ui->m_BlendMode->setEnabled(false);

		ui->m_RenderMode->setCurrentIndex(0);
		ui->m_RenderMode->setEnabled(false);

		ui->m_TransferFunctionWidget->SetDataNode(nullptr);
		ui->m_TransferFunctionWidget->setEnabled(false);

		ui->m_TransferFunctionGeneratorWidget->SetDataNode(nullptr);
		ui->m_TransferFunctionGeneratorWidget->setEnabled(false);
		return;
	}

	// otherwise we can activate em all
	ui->m_BlendMode->setEnabled(true);
	ui->m_RenderMode->setEnabled(true);

	// Determine Combo Box mode
	{
		bool usegpu=false;
		bool useray=false;
		bool usemip=false;
		selectedNode->GetBoolProperty("volumerendering.usegpu",usegpu);
		selectedNode->GetBoolProperty("volumerendering.useray",useray);
		selectedNode->GetBoolProperty("volumerendering.usemip",usemip);

		int blendMode;
		if (selectedNode->GetIntProperty("volumerendering.blendmode", blendMode))
			ui->m_BlendMode->setCurrentIndex(blendMode);

		if (usemip)
			ui->m_BlendMode->setCurrentIndex(vtkVolumeMapper::MAXIMUM_INTENSITY_BLEND);

		int mode = DEFAULT_RENDERMODE;

		if (useray)
			mode = RAYCAST_RENDERMODE;
		else if(usegpu)
			mode = GPU_RENDERMODE;

		ui->m_RenderMode->setCurrentIndex(mode);

	}

	ui->m_TransferFunctionWidget->SetDataNode(selectedNode);
	ui->m_TransferFunctionWidget->setEnabled(true);
	ui->m_TransferFunctionGeneratorWidget->SetDataNode(selectedNode);
	ui->m_TransferFunctionGeneratorWidget->setEnabled(true);
}


void VolumeVisualizationView::OnEnableRendering(bool state)
{
	if(m_SelectedNode.IsExpired())
		return;

	m_SelectedNode.Lock()->SetProperty("volumerendering",mitk::BoolProperty::New(state));
	UpdateInterface();
	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void VolumeVisualizationView::OnBlendMode(int mode)
{
	if (m_SelectedNode.IsExpired())
		return;

	auto selectedNode = m_SelectedNode.Lock();

	bool usemip = false;
	if (mode == vtkVolumeMapper::MAXIMUM_INTENSITY_BLEND)
		usemip = true;

	selectedNode->SetProperty("volumerendering.usemip", mitk::BoolProperty::New(usemip));
	selectedNode->SetProperty("volumerendering.blendmode", mitk::IntProperty::New(mode));

	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void VolumeVisualizationView::OnRenderMode(int mode)
{
	if(m_SelectedNode.IsExpired())
		return;

	auto selectedNode = m_SelectedNode.Lock();

	bool usegpu = false;
	if (mode == GPU_RENDERMODE)
		usegpu = true;

	bool useray = false;
	if (mode == RAYCAST_RENDERMODE)
		useray = true;

	if (mode == DEFAULT_RENDERMODE)
	{
		useray = true;
		usegpu = true;
	}

	selectedNode->SetProperty("volumerendering.usegpu",mitk::BoolProperty::New(usegpu));
	selectedNode->SetProperty("volumerendering.useray",mitk::BoolProperty::New(useray));

	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

//void VolumeVisualizationView::NodeRemoved(const mitk::DataNode* node)
//{
//	if(m_SelectedNode == node)
//	{
//		m_SelectedNode = nullptr;
//		ui->m_SelectedImageLabel->hide();
//		ui->m_ErrorImageLabel->hide();
//		ui->m_NoSelectedImageLabel->show();
//		UpdateInterface();
//	}
//}
