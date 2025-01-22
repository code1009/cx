#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
constexpr DWORD FrameWindowStyle      = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
constexpr DWORD FrameWindowStyleEx    = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

constexpr DWORD ControlWindowStyle    = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
constexpr DWORD ControlWindowStyleEx  = 0;

constexpr DWORD MDIChildWindowStyle   = WS_OVERLAPPEDWINDOW | WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
constexpr DWORD MDIChildWindowStyleEx = WS_EX_MDICHILD;

constexpr DWORD DialogWindowStyle     = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
constexpr DWORD DialogWindowStyleEx   = 0;

constexpr DWORD ToolWindowStyle       = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
constexpr DWORD ToolWindowStyleEx     = WS_EX_TOOLWINDOW;

constexpr DWORD PopupWindowStyle      = WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
constexpr DWORD PopupWindowStyleEx    = 0;

constexpr DWORD ChildWindowStyle      = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
constexpr DWORD ChildWindowStyleEx    = 0;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




