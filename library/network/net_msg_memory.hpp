﻿#ifndef INCLUDED__cx__network__net_msg_memory__hpp
#define INCLUDED__cx__network__net_msg_memory__hpp





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{
namespace network
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool net_msg_memory_initialize(std::size_t capacity = 1024U);
void net_msg_memory_cleanup(void);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
net_msg* net_msg_alloc(void) noexcept;
void net_msg_free(net_msg* ptr) noexcept;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif


