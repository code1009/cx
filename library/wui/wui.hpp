#pragma once

/////////////////////////////////////////////////////////////////////////////
//
// File: wui.hpp
//
// Created by Eui-kwon, MOON.(code1009@naver.com)
// Created on Jan-11th, 2025.
//
// - wui(Window User Interface) Library
// - version 2.0.0.0a
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// # TODO
// - GDIObject
// - Common Control
// - Common Dialog
// - Resource
// - Menu
// - Toolbar




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// # HINSTANCE
#include "AppModule.hpp"

//===========================================================================
// # Coordinates
#include "Coord.hpp"

//===========================================================================
// # Window
#include "Window/WindowClass.hpp"
#include "Window/WindowProc.hpp"
#include "Window/Window.hpp"

//#  WindowBase
#include "Window/WindowStyle.hpp"
#include "Window/BaseWindow.hpp"

//===========================================================================
// # WindowMessageLoop
//   - GetMessage()
#include "WindowMessage/WindowMessageLoop.hpp"

//===========================================================================
// # WindowMessage
// WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
#include "WindowMessage/WindowMessage.hpp"

//===========================================================================
// # Window Function
#include "Window/BasicWindowT.hpp"
#include "Window/Window_Functions.hpp"

// # WindowMessageMap
#include "Window/WindowMessageHandler.hpp"
#include "Window/WindowMessageMap.hpp"
#include "Window/MessageMapWindow.hpp"

// # Window Message : WPARAM LPARAM, LRESULT
#include "WindowMessage/WindowMessageCrack.hpp"

//===========================================================================
// # DeviceContext
#include "GDI/DC.hpp"
#include "GDI/BasicDCT.hpp"
#include "GDI/DC_Classes.hpp"
#include "GDI/DC_Functions.hpp"

// # GDIObject
#include "GDI/GDIObject_Pen.hpp"
#include "GDI/GDIObject_Brush.hpp"
#include "GDI/GDIObject_Font.hpp"
#include "GDI/GDIObject_Bitmap.hpp"

//===========================================================================
// # .rc Resource
#include "Resource/Resource.hpp"
#include "Resource/Resource_Functions.hpp"
#include "Resource/DialogTemplate.hpp"



