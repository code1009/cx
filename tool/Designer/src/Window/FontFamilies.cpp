/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include <wil/com.h>
#include <dwrite.h>
#include <vector>

#include <cx/runtime/runtime.hpp>





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::vector<std::wstring> getFontFamilies(void)
{
	std::vector<std::wstring> fontFamilies;

	HRESULT hr = S_OK;
	wil::com_ptr<::IDWriteFactory> dwriteFactory;

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<::IUnknown**>(dwriteFactory.put()));
	if (!SUCCEEDED(hr))
	{
		CX_RUNTIME_LOG(cxLError) << L"Failed to create IDWriteFactory.";
		return fontFamilies;
	}


	wil::com_ptr<::IDWriteFontCollection> fontCollection;
	hr = dwriteFactory->GetSystemFontCollection(fontCollection.put());
	if (!SUCCEEDED(hr))
	{
		CX_RUNTIME_LOG(cxLError) << L"Failed to get system font collection.";
		return fontFamilies;
	}


	UINT32 familyCount = fontCollection->GetFontFamilyCount();
	for (UINT32 i = 0; i < familyCount; ++i)
	{
		wil::com_ptr<::IDWriteFontFamily> fontFamily;
		hr = fontCollection->GetFontFamily(i, fontFamily.put());
		if (SUCCEEDED(hr))
		{
			wil::com_ptr<::IDWriteLocalizedStrings> familyNames;
			hr = fontFamily->GetFamilyNames(familyNames.put());
			if (SUCCEEDED(hr))
			{
				UINT32 index = 0;
				BOOL exists = false;

				if (SUCCEEDED(familyNames->FindLocaleName(L"ko-KR", &index, &exists)) && exists)
				{
					UINT32 length = 0;
					familyNames->GetStringLength(index, &length);
					std::wstring name(length, L'\0');
					familyNames->GetString(index, name.data(), length + 1);
					fontFamilies.push_back(name);
				}
				else if (SUCCEEDED(familyNames->FindLocaleName(L"en-us", &index, &exists)) && exists)
				{
					UINT32 length = 0;
					familyNames->GetStringLength(index, &length);
					std::wstring name(length, L'\0');
					familyNames->GetString(index, name.data(), length + 1);
					fontFamilies.push_back(name);
				}
			}
		}
	}

	return fontFamilies;
}