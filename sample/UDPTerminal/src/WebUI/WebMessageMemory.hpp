﻿#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace app
{




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool WebMessageMemory_Initialize(std::size_t capacity = 1024U);
void WebMessageMemory_Cleanup(void);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WebMessage* WebMessage_Alloc(void) noexcept;
void WebMessage_Free(WebMessage* ptr) noexcept;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}









