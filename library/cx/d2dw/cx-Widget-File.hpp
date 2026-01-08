#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
    //=======================================================================
    bool saveFile(std::wstring filePath, View& view);
    bool loadFile(std::wstring filePath, View& view, bool designMode=false);
}
