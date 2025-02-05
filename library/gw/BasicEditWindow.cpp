﻿/////////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
BasicEditWindow::BasicEditWindow(HWND hwnd, bool center) :
	Window{ hwnd, center }
{
	//-----------------------------------------------------------------------
	_DocumentGrid = std::make_unique<DocumentGrid>();
	_DocumentGrid->setVisibleChangedHandler(
		std::bind(&BasicEditWindow::render, this)
	);


	//-----------------------------------------------------------------------
	_StatusOverayPanel = std::make_unique<StatusOverayPanel>();
	_StatusOverayPanel->setVisibleChangedHandler(
		std::bind(&BasicEditWindow::render, this)
	);


	//-----------------------------------------------------------------------
	_Viewport->setWindowSize(0, 0);
	_Viewport->setDocumentSize(1920, 1080);
	_Viewport->enableScrollbar(true);
}

//===========================================================================
bool BasicEditWindow::createDeviceResources(void)
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

void BasicEditWindow::destroyDeviceResources(void)
{
	//-----------------------------------------------------------------------
	_StatusOverayPanel->destroyDeviceResources();
	_DocumentGrid->destroyDeviceResources();


	//-----------------------------------------------------------------------
	Window::destroyDeviceResources();
}

void BasicEditWindow::draw(void)
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

void BasicEditWindow::drawDocument(void)
{

}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}



