/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../../gw.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WidgetDesignerModel::WidgetDesignerModel(Window* window):
	_Window{window}
{
	//-----------------------------------------------------------------------
	_WidgetResourceMap = std::make_unique<WidgetResourceMap>(_Window->getContext());

	_DesigeWidgetContext = std::make_unique<DesigeWidgetContext>();


	//-----------------------------------------------------------------------
	_WidgetDocument = std::make_unique<WidgetDocument>();
	_WidgetDocument->setWidgetPropertyChangedHandler(
		std::bind(&WidgetDesignerModel::onWidgetPropertyChanged, this, std::placeholders::_1)
	);


	//-----------------------------------------------------------------------
	_WidgetDesigner = std::make_unique<WidgetDesigner>(this);


	//-----------------------------------------------------------------------
	_WidgetEventWindowMessageHandler =
		std::make_unique<WidgetEventWindowMessageHandler>();
	_WidgetEventWindowMessageHandler->setWidgetDocument(_WidgetDocument.get());
	_WidgetEventWindowMessageHandler->setViewport(_Window->getViewport());

	_WidgetEventDragDropHandler =
		std::make_unique<WidgetEventDragDropHandler>(_WidgetDocument.get(), _Window->getViewport());
}

//===========================================================================
WidgetDesignerModel::~WidgetDesignerModel()
{
	_WidgetResourceMap->clear();

	destroyDeviceResources();
	destroyDeviceIndependentResources();
}

//===========================================================================
bool WidgetDesignerModel::createDeviceIndependentResources(Context* ctx)
{
	return true;
}

void WidgetDesignerModel::destroyDeviceIndependentResources(void)
{

}

//===========================================================================
bool WidgetDesignerModel::createDeviceResources(Context* ctx)
{
	bool rv;


	//-----------------------------------------------------------------------
	if (_UpdateWidgetResourceMap)
	{
		rv = getDesigeWidgetContext()->loadResources(_WidgetResourceMap.get());
		if (!rv)
		{
			return false;
		}
		rv = getWidgetDocument()->loadResources(_WidgetResourceMap.get());
		if (!rv)
		{
			return false;
		}
		rv = getWidgetDesigner()->loadResources(_WidgetResourceMap.get());
		if (!rv)
		{
			return false;
		}

		_UpdateWidgetResourceMap = false;
	}


	return true;
}

void WidgetDesignerModel::destroyDeviceResources(void)
{
	_WidgetResourceMap->discardDeviceResources();
	_UpdateWidgetResourceMap = true;
}

//===========================================================================
void WidgetDesignerModel::draw(Context* ctx)
{
	drawWidgetDocument(ctx);
}

void WidgetDesignerModel::drawWidgetDocument(Context* ctx)
{
	_WidgetDocument->draw(ctx);
	_WidgetDesigner->draw(ctx);
}

//===========================================================================
void WidgetDesignerModel::onWidgetPropertyChanged(Widget::PropertyChangedParam* param)
{
	if (param->_Code == 0x80000000)
	{
		bool rv;
		rv = param->_Sender->loadResources(_WidgetResourceMap.get());
		if (!rv)
		{
			_WidgetResourceMap->clear();
			_UpdateWidgetResourceMap = true;
		}
	}

	_Window->render();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




