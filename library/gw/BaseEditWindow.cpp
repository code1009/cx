/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "gw.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
BaseEditWindow::BaseEditWindow(HWND hwnd, bool center) :
	Window{ hwnd, center }
{
	//-----------------------------------------------------------------------
	_DocumentGrid = std::make_unique<DocumentGrid>();
	_DocumentGrid->setVisibleChangedHandler(
		std::bind(&BaseEditWindow::render, this)
	);


	//-----------------------------------------------------------------------
	_StatusOverayPanel = std::make_unique<StatusOverayPanel>();
	_StatusOverayPanel->setVisibleChangedHandler(
		std::bind(&BaseEditWindow::render, this)
	);


	//-----------------------------------------------------------------------
	getViewport()->setWindowSize(0, 0);
	getViewport()->setDocumentSize(1920, 1080);
	getViewport()->enableScrollbar(true);
}

//===========================================================================
bool BaseEditWindow::createDeviceResources(void)
{
	bool rv;


	//-----------------------------------------------------------------------
	rv = Window::createDeviceResources();
	if (!rv)
	{
		return false;
	}


	//-----------------------------------------------------------------------
	rv = _StatusOverayPanel->createDeviceResources(this);
	if (!rv)
	{
		return false;
	}
	rv = _DocumentGrid->createDeviceResources(this);
	if (!rv)
	{
		return false;
	}


	return true;
}

void BaseEditWindow::destroyDeviceResources(void)
{
	//-----------------------------------------------------------------------
	_StatusOverayPanel->destroyDeviceResources();
	_DocumentGrid->destroyDeviceResources();


	//-----------------------------------------------------------------------
	Window::destroyDeviceResources();
}

void BaseEditWindow::draw(void)
{
	//-----------------------------------------------------------------------
	Window::draw();


	//-----------------------------------------------------------------------
	_DocumentGrid->draw(this);


	//-----------------------------------------------------------------------
	drawDocument();


	//-----------------------------------------------------------------------
	_StatusOverayPanel->draw(this);
}

void BaseEditWindow::drawDocument(void)
{

}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}



