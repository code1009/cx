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
// # HINSTANCE
#include "AppModule.hpp"

//===========================================================================
// # MessageLoop
//   - GetMessage()
#include "WindowMessageLoop.hpp"

//===========================================================================
// # Window
#include "WindowClass.hpp"
#include "WindowProc.hpp"
#include "Window.hpp"

//#  Window 확장
#include "WindowStyle.hpp"
#include "BaseWindow.hpp"

// # Window 메시지
// WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
#include "WindowMessage.hpp"

// # Window 메시지 맵
#include "WindowMessageHandler.hpp"
#include "WindowMessageMap.hpp"
#include "MessageMapWindow.hpp"

// # WPARAM LPARAM 추출, LRESULT 반환
#include "WindowMessageCrack.hpp"

// # Window 함수들
#include "WindowFunctions.hpp"

//===========================================================================
// # 좌표
#include "Coord.hpp"

//===========================================================================
// # DeviceContext
#include "DC.hpp"
#include "DCClasses.hpp"
#include "DCFunctions.hpp"

// # GDI 객체
#include "GDIObject.hpp"

//===========================================================================
// # .rc 리소스
#include "Resource.hpp"
#include "ResourceFunctions.hpp"
#include "DialogTemplate.hpp"



