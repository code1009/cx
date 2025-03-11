/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <rpc.h>
#include <midles.h>
#include <Rpcdce.h>

//#include <midles.h>
//#include <rpcasync.h>
//#include <rpcdcep.h>
//#include <rpcnsi.h>
//#include <rpcproxy.h>
//#include <rpcssl.h>

//===========================================================================
#include <runtime/runtime.hpp>

#include <component/std_wstring_utility.hpp>

//===========================================================================
#include "VcFile.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma comment(lib, "rpcrt4.lib")





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring makeGuid(void)
{
	std::wstring uuid_string;
	std::wstring guid_string;


	UUID       uuid;
	RPC_WSTR   uuid_cstr;
	RPC_STATUS status;


	status = UuidCreate(&uuid);

	uuid_cstr = 0;
	status = UuidToStringW(&uuid, &uuid_cstr);
	if (RPC_S_OK == status)
	{
		uuid_string = reinterpret_cast<const wchar_t*>( uuid_cstr );

		RpcStringFreeW(&uuid_cstr);
	}

	guid_string = L"{";
	guid_string+= uuid_string;
	guid_string+= L"}";

	cx::to_uppercase_std_wstring(guid_string);

	return guid_string;
}







/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
