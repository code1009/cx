#pragma once

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
#define cpp_delete delete
#else
#define cpp_new new
#define cpp_delete delete
#endif

#ifdef _DEBUG
#include <crtdbg.h>
#endif





//////////////////////////////////////////////////////////////////////////////
//============================================================================
// Windows.h 관련 설정
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

//============================================================================
// window c 헤더
#include <Windows.h>

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

//===========================================================================
#include <Unknwn.h>
#include <shobjidl.h>





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>




//////////////////////////////////////////////////////////////////////////////
//============================================================================
// c++ stl 헤더
#include <cinttypes>
#include <cstdint>
#include <cstddef>
#include <stdexcept>

#include <string>
#include <string_view>

#include <utility>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#include <vector>
#include <stack>
#include <deque>
#include <list>
#include <map>
#include <unordered_map>

#include <optional>

#include <algorithm>

#include <memory>
#include <functional>

#include <atomic>
#include <thread>
#include <mutex>
#include <future>
#include <coroutine>

#include <chrono>
#include <format>
#include <filesystem>
