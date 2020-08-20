#include "SegmentationSettings.h"
#include "ui_SegmentationSettings.h"

#include <QSettings>
#include <QMessageBox>

#define DEFAULT_RADIO_OUTLINE true
#define DEFAULT_RADIO_OVERLAY false
#define DEFAULT_VOLUME_RENDERING false
#define DEFAULT_SELECTION_MODE false
#define DEFAULT_SMOOTHING_VALUE 0.10
#define DEFAULT_DECIMATION_VALUE 0.50

#define DEFAULT_SERVER_URI "localhost"
#define DEFAULT_SERVER_TIMEOUT 60
#define DEFAULT_FILTER_BY_LABEL true
#define DEFAULT_NEIGHBORHOOD_SIZE 1

namespace Segmentation
{
	Settings* Settings::m_Instance = nullptr;

	Settings* Settings::GetInstance()
	{
		if (m_Instance == nullptr)
		{
			m_Instance = new Settings(QApplication::topLevelWidgets()[0]);
			m_Instance->hide();
		}
		return m_Instance;
	}

	Settings::Settings(QWidget *parent)
		:	QWidget(parent),
			ui(new Ui::Settings),
			m_NvidiaTitle("nvidia"),
			m_SegTitle("Segmentation")
	{
		ui->setupUi(this);

		m_Settings = new QSettings;

		m_Settings->beginGroup(m_SegTitle);
		ui->radioOutline->setChecked(m_Settings->value("radioOutline", DEFAULT_RADIO_OUTLINE).toBool());
		ui->radioOverlay->setChecked(m_Settings->value("radioOverlay", DEFAULT_RADIO_OVERLAY).toBool());
		ui->volumeRenderingCheckBox->setChecked(m_Settings->value("volumeRenderingCheckBox", DEFAULT_VOLUME_RENDERING).toBool());
		ui->selectionModeCheckBox->setChecked(m_Settings->value("selectionModeCheckBox", DEFAULT_SELECTION_MODE).toBool());
		ui->smoothingSpinBox->setValue(m_Settings->value("smoothingSpinBox", DEFAULT_SMOOTHING_VALUE).toDouble());
		ui->decimationSpinBox->setValue(m_Settings->value("decimationSpinBox", DEFAULT_DECIMATION_VALUE).toDouble());
		m_Settings->endGroup();

		// Checks for saved values, if they don't exist, set default values.
		m_Settings->beginGroup(m_NvidiaTitle);
		ui->serverURILineEdit->setText(m_Settings->value("serverUri", DEFAULT_SERVER_URI).toString());
		ui->serverTimeoutSpinBox->setValue(m_Settings->value("serverTimeout", DEFAULT_SERVER_TIMEOUT).toInt());
		ui->modelFilterCheckBox->setChecked(m_Settings->value("filterByLabel", DEFAULT_FILTER_BY_LABEL).toBool());
		ui->neighborhoodSizeSpinBox->setValue(m_Settings->value("neighborhoodSize", DEFAULT_NEIGHBORHOOD_SIZE).toInt());
		m_Settings->endGroup();

		connect( ui->volumeRenderingCheckBox, SIGNAL(stateChanged(int)), this, SLOT(OnVolumeRenderingCheckboxChecked(int)) );
	}

	Settings::~Settings()
	{
		delete ui;
	}

	double Settings::getDecimationSpinBox() const
	{
		return m_DecimationSpinBox;
	}

	double Settings::getSmoothingSpinBox() const
	{
		return m_SmoothingSpinBox;
	}

	bool Settings::getSelectionModeCheckBox() const
	{
		return m_SelectionModeCheckBox;
	}

	bool Settings::getVolumeRenderingCheckBox() const
	{
		return m_VolumeRenderingCheckBox;
	}

	bool Settings::getRadioOverlay() const
	{
		return m_RadioOverlay;
	}

	bool Settings::getRadioOutline() const
	{
		return m_RadioOutline;
	}

	void Settings::performOk()
	{
		// Saving gui values on variables
		m_RadioOutline = ui->radioOutline->isChecked();
		m_RadioOverlay = ui->radioOverlay->isChecked();
		m_VolumeRenderingCheckBox = ui->volumeRenderingCheckBox->isChecked();
		m_SelectionModeCheckBox = ui->selectionModeCheckBox->isChecked();
		m_SmoothingSpinBox = ui->smoothingSpinBox->value();
		m_DecimationSpinBox = ui->decimationSpinBox->value();

		// Saving settings persistently
		m_Settings->beginGroup(m_SegTitle);
		m_Settings->setValue("radioOutline", m_RadioOutline);
		m_Settings->setValue("radioOverlay", m_RadioOverlay);
		m_Settings->setValue("volumeRenderingCheckBox", m_VolumeRenderingCheckBox);
		m_Settings->setValue("selectionModeCheckBox", m_SelectionModeCheckBox);
		m_Settings->setValue("smoothingSpinBox", m_SmoothingSpinBox);
		m_Settings->setValue("decimationSpinBox", m_DecimationSpinBox);
		m_Settings->endGroup();

		// Saving gui values on variables
		m_ServerUri = ui->serverURILineEdit->text();
		m_ServerTimeout = ui->serverTimeoutSpinBox->value();
		m_FilterByLabel = ui->modelFilterCheckBox->isChecked();
		m_NeighborhoodSize = ui->neighborhoodSizeSpinBox->value();

		// Saving settings persistently
		m_Settings->beginGroup(m_NvidiaTitle);
		m_Settings->setValue("serverUri", m_ServerUri);
		m_Settings->setValue("serverTimeout", m_ServerTimeout);
		m_Settings->setValue("filterByLabel", m_FilterByLabel);
		m_Settings->setValue("neighborhoodSize", m_NeighborhoodSize);
		m_Settings->endGroup();
	}

	QString Settings::getServerUri() const
	{
		return m_ServerUri;
	}

	int Settings::getServerTimeout() const
	{
		return m_ServerTimeout;
	}

	bool Settings::getFilterByLabel() const
	{
		return m_FilterByLabel;
	}

	int Settings::getNeighborhoodSize() const
	{
		return m_NeighborhoodSize;
	}

	void Settings::onVolumeRenderingCheckboxChecked(int state)
	{
		if ( state != Qt::Unchecked )
		{
			QMessageBox::information(nullptr,
															 "Memory warning",
															 "Turning on volume rendering of segmentations will make the application more memory intensive (and potentially prone to crashes).\n\n"
															 "If you encounter out-of-memory problems, try turning off volume rendering again.");
		}
	}
}
