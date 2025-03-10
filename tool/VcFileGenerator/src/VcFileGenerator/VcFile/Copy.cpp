/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <runtime/runtime.hpp>

#include <component\std_wstring_utility.hpp>

//===========================================================================
#include "Parameter.hpp"
#include "Copy.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool Copy(Parameter& param)
{
	//-----------------------------------------------------------------------
	std::wstring splash;
	std::wstring backsplash;


	splash    = L"/";
	backsplash= L"\\";


	//-----------------------------------------------------------------------
	std::wstring source_directory;
	std::wstring target_directory;


	source_directory = param.get(L"$SourceDirectory") + L"/TargetRoot/";
	target_directory = param.get(L"$TargetDirectory");


	source_directory = cx::replace_std_wstring(source_directory, splash, backsplash);
	target_directory = cx::replace_std_wstring(target_directory, splash, backsplash);


	//-----------------------------------------------------------------------
	std::wstring source;
	std::wstring target;

	source.push_back(0); // FOF_MULTIDESTFILES \0\0
	target.push_back(0); // FOF_MULTIDESTFILES \0\0


	//-----------------------------------------------------------------------
	SHFILEOPSTRUCTW shfo;
	int             rc;


	memset (&shfo, 0, sizeof(shfo));

	shfo.hwnd                      = NULL;
	shfo.wFunc                     = FO_COPY;
	shfo.pFrom                     = source.c_str();
	shfo.pTo                       = target.c_str();
	shfo.fFlags                    = FOF_MULTIDESTFILES | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
	//shfo.fFlags                  = FOF_SIMPLEPROGRESS | FOF_MULTIDESTFILES | FOF_NOCONFIRMMKDIR;
	shfo.fAnyOperationsAborted     = FALSE;
	shfo.hNameMappings             = NULL;
	//shfo.lpszProgressTitle       = L"Copy";

	rc = SHFileOperationW (&shfo);
	return true;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
