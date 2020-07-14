#ifndef QMITKABSTRACTRENDERWIDGET_H
#define QMITKABSTRACTRENDERWIDGET_H

#include <mitkStandaloneDataStorage.h>

#include <mitkBaseRenderer.h>
#include <mitkDisplayActionEventHandler.h>
#include <mitkInteractionSchemeSwitcher.h>
#include <mitkPoint.h>
#include <mitkPointSet.h>

#include <QmitkRenderWindow.h>

#include <QFrame>
#include <QHBoxLayout>

class vtkCornerAnnotation;
class QmitkDataStorageTreeModel;

class QmitkAbstractRenderWidget : public QFrame
{
	Q_OBJECT
public:
	explicit QmitkAbstractRenderWidget(QWidget *parent,
																		 const QString& widgetName);
	~QmitkAbstractRenderWidget();

	void SetDataStorage(mitk::DataStorage* dataStorage);

	const QString& GetWidgetName() const { return m_WidgetName; };
	QmitkRenderWindow* GetRenderWindow() const { return m_RenderWindow; };
	mitk::DataNode::Pointer GetPlaneNode() const { return m_PlaneNode; };
	mitk::DataStorage* GetDataStorage() const { return m_DataStorage; };

	mitk::SliceNavigationController* GetSliceNavigationController() const;

	void RequestUpdate();//
	void ForceImmediateUpdate();//

	void SetGradientBackgroundColors(const mitk::Color& upper, const mitk::Color& lower);
	void ShowGradientBackground(bool enable);
	std::pair<mitk::Color, mitk::Color> GetGradientBackgroundColors() const { return m_GradientBackgroundColors; };
	bool IsGradientBackgroundOn() const;

	void SetDecorationColor(const mitk::Color& color);
	mitk::Color GetDecorationColor() const { return m_DecorationColor; };

	void ShowColoredRectangle(bool show);
	bool IsColoredRectangleVisible() const;

	void ShowCornerAnnotation(bool show);
	bool IsCornerAnnotationVisible() const;
	void SetCornerAnnotationText(const std::string& cornerAnnotation);
	std::string GetCornerAnnotationText() const;

	bool IsRenderWindowMenuActivated() const;

	void ActivateCrosshair(bool activate);

	void SetCrosshairVisibility(bool visible);
	bool GetCrosshairVisibility() const;
	void SetWidgetPlaneMode(int mode);
	void SetInteractionScheme(mitk::InteractionSchemeSwitcher::InteractionScheme scheme);

	void ResetView();

signals:
	void MouseEvent(QMouseEvent* e);

protected:
	void InitGUI();
	void InitDecorations();
	void InitDisplayActionEventHandling();

	void SetCrosshair(mitk::Point3D selectedPoint);

	virtual void showEvent(QShowEvent *e);
	virtual void hideEvent(QHideEvent *e);

	QString m_WidgetName;
	QHBoxLayout* m_Layout;

	mitk::DataStorage::Pointer m_DataStorage;
	QmitkDataStorageTreeModel *m_DataStorageTree;

	mitk::DataNode::Pointer m_PlaneNode;
	QVector<mitk::DataNode::Pointer> m_ChildPlaneNodes;

	QmitkRenderWindow* m_RenderWindow;

	mitk::DataNode::Pointer m_PointSetNode;
	mitk::PointSet::Pointer m_PointSet;

	std::pair<mitk::Color, mitk::Color> m_GradientBackgroundColors;
	mitk::Color m_DecorationColor;
	vtkSmartPointer<vtkCornerAnnotation> m_CornerAnnotation;

	mitk::SliceNavigationController* m_TimeNavigationController;

	bool m_PendingCrosshairPositionEvent;

	// interaction
	mitk::DisplayActionEventBroadcast::Pointer m_DisplayActionEventBroadcast;
	std::unique_ptr<mitk::DisplayActionEventHandler> m_DisplayActionEventHandler;
};

#endif // QMITKABSTRACTRENDERWIDGET_H
