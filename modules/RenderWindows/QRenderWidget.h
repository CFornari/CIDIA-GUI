#ifndef QRENDERWIDGET_H
#define QRENDERWIDGET_H

#include <mitkBaseRenderer.h>
#include <mitkDisplayActionEventHandler.h>
#include <mitkInteractionSchemeSwitcher.h>
#include <mitkPoint.h>
#include <mitkPointSet.h>

#include <QmitkRenderWindow.h>

#include <QFrame>
#include <QHBoxLayout>

class vtkCornerAnnotation;

class QRenderWidget : public QFrame
{
	Q_OBJECT
public:
	explicit QRenderWidget(QWidget *parent,
												 const QString& widgetName,
												 mitk::DataStorage* dataStorage);

	void SetDataStorage(mitk::DataStorage* dataStorage);

	void AppendChildPlaneNodes(mitk::DataNode::Pointer childPlaneNode);
	void ClearChildPlaneNodes();

	void AddPlanesToDataStorage();
	void RemovePlanesFromDataStorage();

	const QString& GetWidgetName() const { return m_WidgetName; };
	QmitkRenderWindow* GetRenderWindow() const { return m_RenderWindow; };
	mitk::DataNode::Pointer GetPlaneNode() const { return m_PlaneNode; };

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

	void setAxialView();
	void setSagittalView();
	void setCoronalView();
	void set3DView();


public slots:

signals:
	void MouseEvent(QMouseEvent* e);

protected:
	void CreatePlaneNode();

private:

	void InitGUI();
	void InitDecorations();
	void InitDisplayActionEventHandling();

	void SetCrosshair(mitk::Point3D selectedPoint);

	virtual void showEvent(QShowEvent *e) override;
	virtual void hideEvent(QHideEvent *e) override;

	QString m_WidgetName;
	QHBoxLayout* m_Layout;

	mitk::DataStorage* m_DataStorage;

	QmitkRenderWindow* m_RenderWindow;

	mitk::DataNode::Pointer m_PointSetNode;
	mitk::PointSet::Pointer m_PointSet;

	std::pair<mitk::Color, mitk::Color> m_GradientBackgroundColors;
	mitk::Color m_DecorationColor;
	vtkSmartPointer<vtkCornerAnnotation> m_CornerAnnotation;

	mitk::SliceNavigationController* m_TimeNavigationController;

	mitk::DataNode::Pointer m_PlaneNode;

	QVector<mitk::DataNode::Pointer> m_ChildPlaneNodes;

	bool m_PendingCrosshairPositionEvent;

	bool m_Is3DView; 

	// interaction
	mitk::DisplayActionEventBroadcast::Pointer m_DisplayActionEventBroadcast;
	std::unique_ptr<mitk::DisplayActionEventHandler> m_DisplayActionEventHandler;
};

#endif // QRENDERWIDGET_H
