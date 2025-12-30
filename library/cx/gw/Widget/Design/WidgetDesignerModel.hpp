/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetDesignerModel
{
private:
	Window* _Window{ nullptr };

	std::unique_ptr<WidgetDocument>                  _WidgetDocument;
	std::unique_ptr<WidgetEventWindowMessageHandler> _WidgetEventWindowMessageHandler;
	std::unique_ptr<WidgetEventDragDropHandler>      _WidgetEventDragDropHandler;
	
	std::unique_ptr<WidgetResourceMap>   _WidgetResourceMap;
	std::unique_ptr<DesignWidgetContext> _DesignWidgetContext;	
	std::unique_ptr<WidgetDesigner>      _WidgetDesigner;
	
	bool _UpdateWidgetResourceMap{ true };

public:
	explicit WidgetDesignerModel(Window* window);

public:
	virtual ~WidgetDesignerModel();

public:
	Window* getWindow(void) const { return _Window; }

	WidgetDocument*                  getWidgetDocument                 (void) const { return _WidgetDocument.get(); }
	WidgetEventWindowMessageHandler* getWidgetEventWindowMessageHandler(void) const { return _WidgetEventWindowMessageHandler.get(); }
	WidgetEventDragDropHandler*      getWidgetEventDragDropHandler     (void) const { return _WidgetEventDragDropHandler.get(); }
	WidgetResourceMap*   getWidgetResourceMap  (void) const { return _WidgetResourceMap.get(); }
	DesignWidgetContext* getDesignWidgetContext(void) const { return _DesignWidgetContext.get(); }
	WidgetDesigner*      getWidgetDesigner     (void) const { return _WidgetDesigner.get(); }

public:
	virtual bool createDeviceIndependentResources(Context* ctx);
	virtual void destroyDeviceIndependentResources(void);
	virtual bool createDeviceResources(Context* ctx);
	virtual void destroyDeviceResources(void);

public:
	virtual void draw(Context* ctx);

protected:
	virtual void drawWidgetDocument(Context* ctx);

protected:
	virtual void onWidgetPropertyChanged(Widget::PropertyChangedParam* param);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




