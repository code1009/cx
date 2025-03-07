#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// SDKDDKVer.h를 포함하면 최고 수준의 가용성을 가진 Windows 플랫폼이 정의됩니다.
// 이전 Windows 플랫폼용 애플리케이션을 빌드하려는 경우에는 SDKDDKVer.h를 포함하기 전에
// WinSDKVer.h를 포함하고 _WIN32_WINNT를 지원하려는 플랫폼으로 설정합니다.
#include <SDKDDKVer.h>



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// CRT Debug
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#endif

//===========================================================================
#ifdef _DEBUG
#define cpp_new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define cpp_new new
#endif

#ifdef _DEBUG
#include <crtdbg.h>
#endif



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define NOMINMAX             // min 및 max 매크로 함수 정의 방지
#define WIN32_LEAN_AND_MEAN  // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

// Windows 헤더 파일
#include <windows.h>
#include <windowsx.h>
#include <winuser.h>

//===========================================================================
#include <shellapi.h>
#include <commctrl.h>
#include <olectl.h>

//===========================================================================
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// C++ 런타임 헤더 파일입니다.
#include <cinttypes>
#include <cstdint>
#include <cstddef>
#include <stdexcept>

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <string>
#include <string_view>

#include <vector>
#include <stack>
#include <deque>
#include <list>
#include <map>
#include <unordered_map>

#include <algorithm>
#include <utility>

#include <memory>
#include <functional>
#include <chrono>

#include <atomic>
#include <thread>
#include <mutex>

#include <filesystem>
