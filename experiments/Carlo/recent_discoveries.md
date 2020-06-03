# Rendering Windows
MITK rendering windows are heavily inherited from VTK, we have many options but the most used and convenient are:
 
1. **QmitkRenderWindow: **
	MITK implementation of the `QVTKWidget`.

2. **QmitkRenderWindowWidget:**
	The `QmitkRenderWindowWidget` is a `QFrame` that holds a render window and some associates properties, like a crosshair (pointset) and decorations.
	Decorations are corner annotation (text and color), frame color or background color and can be set using this class.
	The `QmitkRenderWindowWidget` is used inside a `QmitkAbstractMultiWidget`, where a map contains several render window widgets to create the multi widget display.

- **QmitkStdMultiWidget:**
	The `QmitkStdMultiWidget` is a `QmitkAbstractMultiWidget` that is used to display multiple render windows, `QmitkRenderWindowWidget`, at once.
	 Render windows are predefined in a 2x2 design with 3 different 2D view planes and a 3D render window.

# Datastorage and QmitkStdMultiWidget

- **How is datastorage set in `QmitkStdMultiWidget`? *IMPORTANT* !**

The class `QmitkStdMultiWidget` inherits:
```cpp
void QmitkAbstractMultiWidget::SetDataStorage(mitk::DataStorage* dataStorage);
```
This function sets the datastorage for each `QmitkRenderWindowWidget`, calling 4 times the method:
```cpp
void QmitkRenderWindowWidget::SetDataStorage(mitk::DataStorage* dataStorage)
{
  if (dataStorage == m_DataStorage)
  {
    return;
  }

  m_DataStorage = dataStorage;
  if (nullptr != m_RenderWindow)
  {
    mitk::BaseRenderer::GetInstance(m_RenderWindow->GetRenderWindow())->SetDataStorage(dataStorage);
  }
}

```
Three things are happening in this method:

1. Check datastorage is already set; 
 - Private variable `m_DataStorage` is set;
 -  Method `void mitk::BaseRender::SetDataStorage(DataStorage *storage)` is called;
 
 The second thing is the one we are interested. Internal variable `m_DataStorage` is used by `QmitkRenderWindowWidget` constructor for calling this line of code:
```cpp
mitk::RenderingManager::GetInstance()->SetDataStorage(m_DataStorage);
```

But there seems to be a desing flaw, this sets  `m_DataStorage` for ALL rendering windows in the program. 

The minor issue is the ineficiency, if this is the desired behavior and is set globally at the at constructor, there is no need to call `void QmitkRenderWindowWidget::SetDataStorage(mitk::DataStorage* dataStorage)` 4 times.

The major issue is that this affects EVERY rendering window, breaking completely code's encapsulation.

- **Can I create two independent datastorage variables for the same file?**

Apparently not.

```cpp

```

# Displacement Planes

Planes are visual indicators for the current slice position in any of the three views. Information regarding them should be stored inside some datastorage variable. (NOT! In the Image object.)

- **How can we create a displacement plane object?**

From `QmitkStdMultiWidget` class:
```cpp
// QmitkStdMultiWidget.h
mitk::DataNode::Pointer m_PlaneNode1;
  
// QmitkStdMultiWidget.cpp
void QmitkStdMultiWidget::InitializeMultiWidget()
{
	(...)
	m_PlaneNode1 = mitk::BaseRenderer::GetInstance(GetRenderWindow1()->GetRenderWindow())->GetCurrentWorldPlaneGeometryNode();
	(...)
}
```

- **How displacement planes become visible in the rendering window?**

After creating them, they should be added to the datastorage variable from the desired redering window.

From `QmitkStdMultiWidget` class:
```cpp
// QmitkStdMultiWidget.h
  // The 3 helper objects which contain the plane geometry.
  mitk::DataNode::Pointer m_PlaneNode1;
  mitk::DataNode::Pointer m_PlaneNode2;
  mitk::DataNode::Pointer m_PlaneNode3;

  // m_ParentNodeForGeometryPlanes This helper object is added to the datastorage
  // and contains the 3 planes for displaying the image geometry (crosshair and 3D planes).
  mitk::DataNode::Pointer m_ParentNodeForGeometryPlanes;
  
// QmitkStdMultiWidget.cpp
void QmitkStdMultiWidget::AddPlanesToDataStorage()
{
  auto dataStorage = GetDataStorage();
  if (nullptr == dataStorage)
  {
    return;
  }

  if (m_PlaneNode1.IsNotNull() && m_PlaneNode2.IsNotNull()
   && m_PlaneNode3.IsNotNull() && m_ParentNodeForGeometryPlanes.IsNotNull())
  {
    dataStorage->Add(m_ParentNodeForGeometryPlanes);
    dataStorage->Add(m_PlaneNode1, m_ParentNodeForGeometryPlanes);
    dataStorage->Add(m_PlaneNode2, m_ParentNodeForGeometryPlanes);
    dataStorage->Add(m_PlaneNode3, m_ParentNodeForGeometryPlanes);
  }
}
```