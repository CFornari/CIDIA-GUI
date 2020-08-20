#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

class QSettings;

namespace Ui {
	class Settings;
}

namespace Segmentation
{
	class Settings : public QWidget
	{
		Q_OBJECT

	public:
		static Settings* GetInstance();

		void performOk();

		QString getServerUri() const;
		int getServerTimeout() const;
		bool getFilterByLabel() const;
		int getNeighborhoodSize() const;

		void onVolumeRenderingCheckboxChecked(int state);

		bool getRadioOutline() const;
		bool getRadioOverlay() const;
		bool getVolumeRenderingCheckBox() const;
		bool getSelectionModeCheckBox() const;
		double getSmoothingSpinBox() const;
		double getDecimationSpinBox() const;

	protected:
		Settings(QWidget *parent = nullptr);
		~Settings();

	private:
		static Settings* m_Instance;

		Ui::Settings *ui;

		const QString m_NvidiaTitle;
		const QString m_SegTitle;

		bool m_RadioOutline;
		bool m_RadioOverlay;
		bool m_VolumeRenderingCheckBox;
		bool m_SelectionModeCheckBox;
		double m_SmoothingSpinBox;
		double m_DecimationSpinBox;

		QString m_ServerUri;
		int m_ServerTimeout;
		bool m_FilterByLabel;
		int m_NeighborhoodSize;

		QSettings *m_Settings;
	};
}
#endif // SETTINGS_H
