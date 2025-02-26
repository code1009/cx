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
WidgetDesignerWindow::WidgetDesignerWindow(HWND hwnd, bool center) :
	BaseEditWindow{ hwnd, center }
{
	//-----------------------------------------------------------------------
	_WidgetDesignerModel = std::make_unique<WidgetDesignerModel>(this);


	//-----------------------------------------------------------------------
	getViewport()->setWindowSize(0, 0);
	getViewport()->setDocumentSize(1920, 1080);
	getViewport()->enableScrollbar(true);
}

//===========================================================================
bool WidgetDesignerWindow::createDeviceIndependentResources(void)
{
	bool rv;

	rv = BaseEditWindow::createDeviceIndependentResources();
	if (!rv)
	{
		return false;
	}

	rv = _WidgetDesignerModel->createDeviceIndependentResources(getContext());
	if (!rv)
	{
		return false;
	}

	return true;
}

void WidgetDesignerWindow::destroyDeviceIndependentResources(void)
{
	_WidgetDesignerModel->destroyDeviceIndependentResources();

	BaseEditWindow::destroyDeviceIndependentResources();
}

//===========================================================================
bool WidgetDesignerWindow::createDeviceResources(void)
{
	bool rv;


	//-----------------------------------------------------------------------
	rv = BaseEditWindow::createDeviceResources();
	if (!rv)
	{
		return false;
	}


	//-----------------------------------------------------------------------
	rv = _WidgetDesignerModel->createDeviceResources(getContext());
	if (!rv)
	{
		return false;
	}


	return true;
}

void WidgetDesignerWindow::destroyDeviceResources(void)
{
	//-----------------------------------------------------------------------
	_WidgetDesignerModel->destroyDeviceResources();


	//-----------------------------------------------------------------------
	BaseEditWindow::destroyDeviceResources();
}

void WidgetDesignerWindow::drawDocument(void)
{
	_WidgetDesignerModel->draw(getContext());
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}



