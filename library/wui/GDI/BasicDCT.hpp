#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/*
From: 
https://github.com/DavidNash2024/Win32xx/blob/v10.1/include/wxx_gdi.h
*/





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
template <typename TBase>
class BasicDCT : public TBase
{
private:
	HWND getBasicDCHandle(void) const
	{
		return static_cast<const TBase*>(this)->getDCHandle();
	}

public:
	int enumObjects(int objectType, GOBJENUMPROC pObjectFunc, LPARAM lparam) const;
	void gradientFill(COLORREF color1, COLORREF color2, RECT rc, BOOL isVertical) const;
	BOOL restoreDC(int savedDC) const;
	int saveDC() const;
	HBITMAP selectBitmap(HBITMAP bitmap) const;
	HBRUSH selectBrush(HBRUSH brush) const;
	HFONT selectFont(HFONT font) const;
	HPEN selectPen(HPEN pen) const;
	int selectRGN(HRGN rgn) const;
	HPALETTE selectPalette(const HPALETTE palette, BOOL forceBkgnd) const;
	void solidFill(COLORREF color, RECT rc) const;

	// Bitmap Functions
	HBITMAP getCurrentBitmap() const;

	// Brush Functions
	HBRUSH getCurrentBrush() const;
	CoordPoint getBrushOrgEx() const;
	CoordPoint setBrushOrgEx(int x, int y);

	// Font Functions
	HFONT getCurrentFont() const;

	// Palette Functions
	HPALETTE getCurrentPalette() const;
	COLORREF getNearestColor(COLORREF color) const;
	UINT realizePalette() const;
	BOOL getColorAdjustment(LPCOLORADJUSTMENT pCA) const;
	BOOL setColorAdjustment(const COLORADJUSTMENT* pCA) const;
	BOOL updateColors() const;

	// Pen Functions
	HPEN getCurrentPen() const;

	// Retrieve and Select Stock Objects
	HGDIOBJ getStockObject(int index) const;
	HGDIOBJ selectStockObject(int index) const;

	// Region Functions

	// Wrappers for WinAPI Functions
	int getDeviceCaps(int index) const;

	// Brush Functions
	COLORREF getDCBrushColor() const;
	COLORREF setDCBrushColor(COLORREF color) const;

	// Font Functions
	DWORD getFontData(DWORD table, DWORD offset, LPVOID buffer, DWORD data) const;
	DWORD getFontLanguageInfo() const;
	DWORD getGlyphOutline(UINT query, UINT format, LPGLYPHMETRICS pGM, DWORD bufSize, LPVOID buffer, const MAT2* pMAT2) const;
	DWORD getKerningPairs(DWORD numPairs, LPKERNINGPAIR pKrnPair) const;
	DWORD setMapperFlags(DWORD flag) const;
	BOOL getMiterLimit(PFLOAT pLimit) const;
	BOOL setMiterLimit(FLOAT newLimit, PFLOAT pOldLimit) const;

	// Clipping Functions
	int excludeClipRect(int left, int top, int right, int bottom) const;
	int excludeClipRect(RECT rc) const;
	int getClipBox(RECT& rc) const;
	int intersectClipRect(int left, int top, int right, int bottom) const;
	int intersectClipRect(RECT rc) const;
	BOOL rectVisible(RECT rc) const;
	int selectClipRgn(HRGN rgn) const;
	BOOL abortPath() const;
	BOOL beginPath() const;
	BOOL endPath() const;
	int extSelectClipRgn(HRGN rgn, int mode) const;
	BOOL flattenPath() const;
	int getPath(POINT* pointArray, BYTE* types, int count) const;
	BOOL ptVisible(int x, int y) const;
	int offsetClipRgn(int xOffset, int yOffset) const;
	BOOL selectClipPath(int mode) const;
	BOOL strokeAndFillPath() const;
	BOOL strokePath() const;
	BOOL widenPath() const;

	// Point and Line Drawing Functions
	CoordPoint getCurrentPosition() const;
	COLORREF getPixel(int x, int y) const;
	COLORREF getPixel(POINT pt) const;
	CoordPoint moveTo(int x, int y) const;
	CoordPoint moveTo(POINT pt) const;
	BOOL lineTo(int x, int y) const;
	BOOL lineTo(POINT pt) const;
	int setROP2(int drawMode) const;
	COLORREF setPixel(int x, int y, COLORREF color) const;
	BOOL arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const;
	BOOL arc(RECT rc, POINT start, POINT end) const;
	BOOL arcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const;
	BOOL arcTo(RECT rc, POINT ptStart, POINT ptEnd) const;
	BOOL angleArc(int x, int y, int radius, float startAngle, float sweepAngle) const;
	BOOL closeFigure() const;
	int getROP2() const;
	int getArcDirection() const;
	int setArcDirection(int arcDirection) const;
	BOOL polyDraw(const POINT* pPointArray, const BYTE* pTypes, int count) const;
	BOOL polyline(LPPOINT pPointArray, int count) const;
	BOOL polyPolyline(const POINT* pPointArray, const DWORD* pPolyPoints, int count) const;
	BOOL polylineTo(const POINT* pPointArray, int count) const;
	BOOL polyBezier(const POINT* pPointArray, int count) const;
	BOOL polyBezierTo(const POINT* pPointArray, int count) const;
	COLORREF setPixel(POINT pt, COLORREF color) const;
	BOOL setPixelV(int x, int y, COLORREF color) const;
	BOOL setPixelV(POINT pt, COLORREF color) const;

	// Shape Drawing Functions
	BOOL drawFocusRect(RECT rc) const;
	BOOL ellipse(int x1, int y1, int x2, int y2) const;
	BOOL ellipse(RECT rc) const;
	BOOL polygon(LPPOINT pPointArray, int count) const;
	BOOL rectangle(int x1, int y1, int x2, int y2) const;
	BOOL rectangle(RECT rc) const;
	BOOL roundRect(int x1, int y1, int x2, int y2, int width, int height) const;
	BOOL roundRect(RECT rc, int width, int height) const;
	BOOL chord(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const;
	BOOL chord(RECT rc, POINT start, POINT end) const;
	BOOL pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const;
	BOOL pie(RECT rc, POINT start, POINT end) const;
	BOOL polyPolygon(LPPOINT pPointArray, LPINT pPolyCounts, int count) const;

	// Fill and 3D Drawing Functions
	BOOL fillRect(RECT rc, HBRUSH brush) const;
	BOOL invertRect(RECT rc) const;
	BOOL drawIconEx(int xLeft, int yTop, HICON icon, int cxWidth, int cyWidth, UINT index, HBRUSH flickerFreeDraw, UINT flags) const;
	BOOL drawEdge(RECT rc, UINT edge, UINT flags) const;
	BOOL drawFrameControl(RECT rc, UINT type, UINT state) const;
	BOOL fillRgn(HRGN rgn, HBRUSH brush) const;
	BOOL gradientFill(PTRIVERTEX pVertex, ULONG vertex, PVOID pMesh, ULONG mesh, ULONG mode) const;
	BOOL drawIcon(int x, int y, HICON icon) const;
	BOOL drawIcon(POINT pt, HICON icon) const;
	BOOL frameRect(RECT rc, HBRUSH brush) const;
	BOOL frameRgn(HRGN rgn, HBRUSH brush, int width, int height) const;
	int getPolyFillMode() const;
	BOOL paintRgn(HRGN rgn) const;
	int setPolyFillMode(int polyFillMode) const;

	// Bitmap Functions
	int stretchDIBits(int xDest, int yDest, int destWidth, int destHeight, int xSrc, int ySrc, int srcWidth, int srcHeight, LPCVOID pBits, const LPBITMAPINFO pBMI, UINT usage, DWORD rop) const;
	BOOL patBlt(int x, int y, int width, int height, DWORD rop) const;
	BOOL bitBlt(int x, int y, int width, int height, HDC hSrc, int xSrc, int ySrc, DWORD rop) const;
	BOOL maskBlt(int xDest, int yDest, int width, int height, HDC hSrc, int xSrc, int ySrc, HBITMAP mask, int xMask, int yMask, DWORD rop) const;
	BOOL stretchBlt(int x, int y, int width, int height, HDC src, int xSrc, int ySrc, int srcWidth, int srcHeight, DWORD rop) const;
	int getDIBits(HBITMAP bitmap, UINT startScan, UINT scanLines, LPVOID pBits, LPBITMAPINFO pBMI, UINT usage) const;
	int setDIBits(HBITMAP bitmap, UINT startScan, UINT scanLines, LPCVOID pBits, LPBITMAPINFO pBMI, UINT colorUse) const;
	int getStretchBltMode() const;
	int setStretchBltMode(int stretchMode) const;
	BOOL transparentBlt(int x, int y, int width, int height, HDC hSrc, int xSrc, int ySrc, int widthSrc, int heightSrc, UINT transparent) const;
	BOOL floodFill(int x, int y, COLORREF color) const;
	BOOL extFloodFill(int x, int y, COLORREF color, UINT fillType) const;

	// coordinate Functions
	BOOL DPtoLP(LPPOINT pPointArray, int count) const;
	BOOL DPtoLP(RECT& rc) const;
	BOOL LPtoDP(LPPOINT pPointArray, int count) const;
	BOOL LPtoDP(RECT& rc) const;

	// Layout Functions
	DWORD getLayout() const;
	DWORD setLayout(DWORD layout) const;

	// Mapping Functions
	int getMapMode() const;
	BOOL getViewportOrgEx(LPPOINT pPoint) const;
	int setMapMode(int mapMode) const;
	BOOL setViewportOrgEx(int x, int y, LPPOINT pPoint = nullptr) const;
	BOOL setViewportOrgEx(POINT point, LPPOINT pPointRet = nullptr) const;
	BOOL offsetViewportOrgEx(int width, int height, LPPOINT pPoint = nullptr) const;
	BOOL getViewportExtEx(LPSIZE pSize) const;
	BOOL setViewportExtEx(int x, int y, LPSIZE pSize) const;
	BOOL setViewportExtEx(SIZE size, LPSIZE pSizeRet) const;
	BOOL scaleViewportExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE pSize) const;
	BOOL getWindowOrgEx(LPPOINT pPoint) const;
	BOOL setWindowOrgEx(int x, int y, LPPOINT pPoint) const;
	BOOL setWindowOrgEx(POINT point, LPPOINT pPointRet) const;
	BOOL offsetWindowOrgEx(int width, int height, LPPOINT pPoint) const;
	BOOL getWindowExtEx(LPSIZE pSize) const;
	BOOL setWindowExtEx(int x, int y, LPSIZE pSize) const;
	BOOL setWindowExtEx(SIZE size, LPSIZE pSizeRet) const;
	BOOL scaleWindowExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE pSize) const;

	// MetaFile Functions
	BOOL playMetaFile(HMETAFILE metaFile) const;
	BOOL playMetaFile(HENHMETAFILE enhMetaFile, RECT bounds) const;

	// Printer Functions
	int startDoc(LPDOCINFO pDocInfo) const;
	int endDoc() const;
	int startPage() const;
	int endPage() const;
	int abortDoc() const;
	int setAbortProc(BOOL(CALLBACK* pfn)(HDC, int)) const;

	// Text Functions
	BOOL extTextOut(int x, int y, UINT options, RECT rc, LPCTSTR string, int count = -1, LPINT pDxWidths = nullptr) const;
	int drawText(LPCTSTR string, int count, RECT rc, UINT format) const;
	UINT getTextAlign() const;
	UINT setTextAlign(UINT flags) const;
	int getTextFace(int count, LPTSTR faceName) const;
	BOOL getTextMetrics(TEXTMETRIC& metrics) const;
	COLORREF getBkColor() const;
	COLORREF setBkColor(COLORREF color) const;
	COLORREF getTextColor() const;
	COLORREF setTextColor(COLORREF color) const;
	int getBkMode() const;
	int setBkMode(int bkMode) const;
	int drawTextEx(LPTSTR string, int count, RECT rc, UINT format, LPDRAWTEXTPARAMS pDTParams) const;
	BOOL getCharABCWidths(UINT firstChar, UINT lastChar, LPABC pABC) const;
	DWORD getCharacterPlacement(LPCTSTR string, int count, int maxExtent, LPGCP_RESULTS results, DWORD flags) const;
	BOOL getCharWidth(UINT firstChar, UINT lastChar, int* buffer) const;
	BOOL getCharWidthFloat(UINT firstChar, UINT lastChar, float* buffer) const;
	CoordSize getTextExtentPoint32(LPCTSTR string, int count) const;
	CoordSize getTextExtentPoint32(LPCTSTR string) const;
	CoordSize getTabbedTextExtent(LPCTSTR string, int count, int tabPositions, LPINT pTabStopPositions) const;
	BOOL grayString(HBRUSH brush, GRAYSTRINGPROC pOutputFunc, LPARAM pData, int count, int x, int y, int width, int height) const;
	int setTextJustification(int breakExtra, int breakCount) const;
	int getTextCharacterExtra() const;
	int setTextCharacterExtra(int charExtra) const;
	CoordSize tabbedTextOut(int x, int y, LPCTSTR string, int count, int tabPositions, LPINT pTabStopPositions, int tabOrigin) const;
	BOOL textOut(int x, int y, LPCTSTR string, int count = -1) const;
	BOOL getCharABCWidthsI(UINT giFirst, UINT cgi, LPWORD pGI, LPABC pABC) const;
	BOOL getCharWidthI(UINT giFirst, UINT cgi, LPWORD pGI, int* buffer) const;
};

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
template <typename TBase>
int BasicDCT<TBase>::enumObjects(int objectType, GOBJENUMPROC pObjectFunc, LPARAM lparam) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::EnumObjects(getBasicDCHandle(), objectType, pObjectFunc, lparam);
}

template <typename TBase>
void BasicDCT<TBase>::gradientFill(COLORREF color1, COLORREF color2, RECT rc, BOOL isVertical) const
{
	solidFill(color1, rc);

	TRIVERTEX vertex[2]{};
	vertex[0].x = rc.left;
	vertex[0].y = rc.top;
	vertex[0].Red = COLOR16(GetRValue(color1) << 8);
	vertex[0].Green = COLOR16(GetGValue(color1) << 8);
	vertex[0].Blue = COLOR16(GetBValue(color1) << 8);
	vertex[0].Alpha = 0;

	vertex[1].x = rc.right;
	vertex[1].y = rc.bottom;
	vertex[1].Red = COLOR16(GetRValue(color2) << 8);
	vertex[1].Green = COLOR16(GetGValue(color2) << 8);
	vertex[1].Blue = COLOR16(GetBValue(color2) << 8);
	vertex[1].Alpha = 0;

	// Create a GRADIENT_RECT structure that
	// references the TRIVERTEX vertices.
	GRADIENT_RECT rect{};
	rect.UpperLeft = 0;
	rect.LowerRight = 1;

	// Draw a gradient filled rectangle.
	const ULONG GradientFillRectH = 0x00000000;
	const ULONG GradientFillRectV = 0x00000001;
	ULONG mode = isVertical ? GradientFillRectV : GradientFillRectH;
	gradientFill(vertex, 2, &rect, 1, mode);
}

template <typename TBase>
BOOL BasicDCT<TBase>::restoreDC(int savedDC) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::RestoreDC(getBasicDCHandle(), savedDC);
}

template <typename TBase>
int BasicDCT<TBase>::saveDC() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SaveDC(getBasicDCHandle());
}

template <typename TBase>
HBITMAP BasicDCT<TBase>::selectBitmap(HBITMAP bitmap) const
{
	assert(getBasicDCHandle() != nullptr);
	HBITMAP oldBitmap = static_cast<HBITMAP>(::SelectObject(getBasicDCHandle(), bitmap));

	return oldBitmap;
}

template <typename TBase>
HBRUSH BasicDCT<TBase>::selectBrush(HBRUSH brush) const
{
	assert(getBasicDCHandle() != nullptr);
	HBRUSH oldBrush = static_cast<HBRUSH>(::SelectObject(getBasicDCHandle(), brush));

	return oldBrush;
}

template <typename TBase>
HFONT BasicDCT<TBase>::selectFont(HFONT font) const
{
	assert(getBasicDCHandle() != nullptr);
	HFONT oldFont = static_cast<HFONT>(::SelectObject(getBasicDCHandle(), font));

	return oldFont;
}

template <typename TBase>
HPEN BasicDCT<TBase>::selectPen(HPEN pen) const
{
	assert(getBasicDCHandle() != nullptr);
	HPEN oldPen = static_cast<HPEN>(::SelectObject(getBasicDCHandle(), pen));
	
	return oldPen;
}

template <typename TBase>
int BasicDCT<TBase>::selectRGN(HRGN rgn) const
{
	assert(getBasicDCHandle() != nullptr);
	HANDLE rgnType = ::SelectObject(getBasicDCHandle(), rgn);

	return static_cast<int> (reinterpret_cast<INT_PTR>(rgnType));
}

template <typename TBase>
HPALETTE BasicDCT<TBase>::selectPalette(const HPALETTE palette, BOOL forceBkgnd) const
{
	assert(getBasicDCHandle() != nullptr);
	HPALETTE oldPalette = ::SelectPalette(getBasicDCHandle(), palette, forceBkgnd);

	return oldPalette;
}

template <typename TBase>
void BasicDCT<TBase>::solidFill(COLORREF color, RECT rc) const
{
	COLORREF oldColor = setBkColor(color);
	extTextOut(0, 0, ETO_OPAQUE, rc, nullptr, 0, 0);
	setBkColor(oldColor);
}

//===========================================================================
// Bitmap Functions
//===========================================================================
template <typename TBase>
HBITMAP BasicDCT<TBase>::getCurrentBitmap() const
{
	assert(getBasicDCHandle() != nullptr);
	return static_cast<HBITMAP>(::GetCurrentObject(getBasicDCHandle(), OBJ_BITMAP));
}

//===========================================================================
// Brush Functions
//===========================================================================
template <typename TBase>
HBRUSH BasicDCT<TBase>::getCurrentBrush() const
{
	assert(getBasicDCHandle() != nullptr);
	return static_cast<HBRUSH>(::GetCurrentObject(getBasicDCHandle(), OBJ_BRUSH));
}

template <typename TBase>
CoordPoint BasicDCT<TBase>::getBrushOrgEx() const
{
	assert(getBasicDCHandle() != nullptr);
	CoordPoint pt;
	::GetBrushOrgEx(getBasicDCHandle(), &pt);
	return pt;
}

template <typename TBase>
CoordPoint BasicDCT<TBase>::setBrushOrgEx(int x, int y)
{
	assert(getBasicDCHandle() != nullptr);
	CoordPoint oldPt;
	::SetBrushOrgEx(getBasicDCHandle(), x, y, &oldPt);
	return oldPt;
}

//===========================================================================
// Font Functions
//===========================================================================
template <typename TBase>
HFONT BasicDCT<TBase>::getCurrentFont() const
{
	assert(getBasicDCHandle() != nullptr);
	return static_cast<HFONT>(::GetCurrentObject(getBasicDCHandle(), OBJ_FONT));
}

//===========================================================================
// Palette Functions
//===========================================================================
template <typename TBase>
HPALETTE BasicDCT<TBase>::getCurrentPalette() const
{
	assert(getBasicDCHandle() != nullptr);
	return static_cast<HPALETTE>(::GetCurrentObject(getBasicDCHandle(), OBJ_PAL));
}

template <typename TBase>
COLORREF BasicDCT<TBase>::getNearestColor(COLORREF color) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetNearestColor(getBasicDCHandle(), color);
}

template <typename TBase>
UINT BasicDCT<TBase>::realizePalette() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::RealizePalette(getBasicDCHandle());
}

template <typename TBase>
BOOL BasicDCT<TBase>::getColorAdjustment(LPCOLORADJUSTMENT pCA) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetColorAdjustment(getBasicDCHandle(), pCA);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setColorAdjustment(const COLORADJUSTMENT* pCA) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetColorAdjustment(getBasicDCHandle(), pCA);
}

template <typename TBase>
BOOL BasicDCT<TBase>::updateColors() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::UpdateColors(getBasicDCHandle());
}

//===========================================================================
// Pen Functions
//===========================================================================
template <typename TBase>
HPEN BasicDCT<TBase>::getCurrentPen() const
{
	assert(getBasicDCHandle() != nullptr);
	return static_cast<HPEN>(::GetCurrentObject(getBasicDCHandle(), OBJ_PEN));
}

//===========================================================================
// Retrieve and Select Stock Objects
//===========================================================================
template <typename TBase>
HGDIOBJ BasicDCT<TBase>::getStockObject(int index) const
{
	return ::GetStockObject(index);
}

template <typename TBase>
HGDIOBJ BasicDCT<TBase>::selectStockObject(int index) const
{
	assert(getBasicDCHandle() != nullptr);
	HGDIOBJ stockObject = ::GetStockObject(index);

	HGDIOBJ oldObject = ::SelectObject(getBasicDCHandle(), stockObject);

	return oldObject;
}

//===========================================================================
// Region Functions
//===========================================================================

//===========================================================================
// Wrappers for WinAPI Functions
//===========================================================================
template <typename TBase>
int BasicDCT<TBase>::getDeviceCaps(int index) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetDeviceCaps(getBasicDCHandle(), index);
}

//===========================================================================
// Brush Functions
//===========================================================================
template <typename TBase>
COLORREF BasicDCT<TBase>::getDCBrushColor() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetDCBrushColor(getBasicDCHandle());
}

template <typename TBase>
COLORREF BasicDCT<TBase>::setDCBrushColor(COLORREF color) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetDCBrushColor(getBasicDCHandle(), color);
}

//===========================================================================
// Font Functions
//===========================================================================
template <typename TBase>
DWORD BasicDCT<TBase>::getFontData(DWORD table, DWORD offset, LPVOID buffer, DWORD data) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetFontData(getBasicDCHandle(), table, offset, buffer, data);
}

template <typename TBase>
DWORD BasicDCT<TBase>::getFontLanguageInfo() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetFontLanguageInfo(getBasicDCHandle());
}

template <typename TBase>
DWORD BasicDCT<TBase>::getGlyphOutline(UINT query, UINT format, LPGLYPHMETRICS pGM, DWORD bufSize,
	LPVOID buffer, const MAT2* pMAT2) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetGlyphOutline(getBasicDCHandle(), query, format, pGM, bufSize, buffer, pMAT2);
}

template <typename TBase>
DWORD BasicDCT<TBase>::getKerningPairs(DWORD numPairs, LPKERNINGPAIR pKrnPair) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetKerningPairs(getBasicDCHandle(), numPairs, pKrnPair);
}

template <typename TBase>
DWORD BasicDCT<TBase>::setMapperFlags(DWORD flag) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetMapperFlags(getBasicDCHandle(), flag);
}

template <typename TBase>
BOOL BasicDCT<TBase>::getMiterLimit(PFLOAT pLimit) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetMiterLimit(getBasicDCHandle(), pLimit);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setMiterLimit(FLOAT newLimit, PFLOAT pOldLimit) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetMiterLimit(getBasicDCHandle(), newLimit, pOldLimit);
}

//===========================================================================
// Clipping Functions
//===========================================================================
template <typename TBase>
int BasicDCT<TBase>::excludeClipRect(int left, int top, int right, int bottom) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::ExcludeClipRect(getBasicDCHandle(), left, top, right, bottom);
}

template <typename TBase>
int BasicDCT<TBase>::excludeClipRect(RECT rc) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::ExcludeClipRect(getBasicDCHandle(), rc.left, rc.top, rc.right, rc.bottom);
}

template <typename TBase>
int BasicDCT<TBase>::getClipBox(RECT& rc) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetClipBox(getBasicDCHandle(), &rc);
}

template <typename TBase>
int BasicDCT<TBase>::intersectClipRect(int left, int top, int right, int bottom) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::IntersectClipRect(getBasicDCHandle(), left, top, right, bottom);
}

template <typename TBase>
int BasicDCT<TBase>::intersectClipRect(RECT rc) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::IntersectClipRect(getBasicDCHandle(), rc.left, rc.top, rc.right, rc.bottom);
}

template <typename TBase>
BOOL BasicDCT<TBase>::rectVisible(RECT rc) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::RectVisible(getBasicDCHandle(), &rc);
}

template <typename TBase>
int BasicDCT<TBase>::selectClipRgn(HRGN rgn) const
{
	assert(getBasicDCHandle() != nullptr);
	int rgnType = ::SelectClipRgn(getBasicDCHandle(), rgn);

	return rgnType;
}

template <typename TBase>
BOOL BasicDCT<TBase>::abortPath() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::AbortPath(getBasicDCHandle());
}

template <typename TBase>
BOOL BasicDCT<TBase>::beginPath() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::BeginPath(getBasicDCHandle());
}

template <typename TBase>
BOOL BasicDCT<TBase>::endPath() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::EndPath(getBasicDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::extSelectClipRgn(HRGN rgn, int mode) const
{
	assert(getBasicDCHandle() != nullptr);
	int rgnType = ::ExtSelectClipRgn(getBasicDCHandle(), rgn, mode);

	return rgnType;
}

template <typename TBase>
BOOL BasicDCT<TBase>::flattenPath() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::FlattenPath(getBasicDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::getPath(POINT* pointArray, BYTE* types, int count) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetPath(getBasicDCHandle(), pointArray, types, count);
}

template <typename TBase>
BOOL BasicDCT<TBase>::ptVisible(int x, int y) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::PtVisible(getBasicDCHandle(), x, y);
}

template <typename TBase>
int BasicDCT<TBase>::offsetClipRgn(int xOffset, int yOffset) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::OffsetClipRgn(getBasicDCHandle(), xOffset, yOffset);
}

template <typename TBase>
BOOL BasicDCT<TBase>::selectClipPath(int mode) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SelectClipPath(getBasicDCHandle(), mode);
}

template <typename TBase>
BOOL BasicDCT<TBase>::strokeAndFillPath() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::StrokeAndFillPath(getBasicDCHandle());
}

template <typename TBase>
BOOL BasicDCT<TBase>::strokePath() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::StrokePath(getBasicDCHandle());
}

template <typename TBase>
BOOL BasicDCT<TBase>::widenPath() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::WidenPath(getBasicDCHandle());
}

//===========================================================================
// Point and Line Drawing Functions
//===========================================================================
template <typename TBase>
CoordPoint BasicDCT<TBase>::getCurrentPosition() const
{
	assert(getBasicDCHandle() != nullptr);
	CoordPoint pt;
	::GetCurrentPositionEx(getBasicDCHandle(), &pt);
	return pt;
}

template <typename TBase>
COLORREF BasicDCT<TBase>::getPixel(int x, int y) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetPixel(getBasicDCHandle(), x, y);
}

template <typename TBase>
COLORREF BasicDCT<TBase>::getPixel(POINT pt) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetPixel(getBasicDCHandle(), pt.x, pt.y);
}

template <typename TBase>
CoordPoint BasicDCT<TBase>::moveTo(int x, int y) const
{
	assert(getBasicDCHandle() != nullptr);
	CoordPoint previous;
	::MoveToEx(getBasicDCHandle(), x, y, &previous);
	return previous;
}

template <typename TBase>
CoordPoint BasicDCT<TBase>::moveTo(POINT pt) const
{
	assert(getBasicDCHandle() != nullptr);
	CoordPoint previous;
	::MoveToEx(getBasicDCHandle(), pt.x, pt.y, &previous);
	return previous;
}

template <typename TBase>
BOOL BasicDCT<TBase>::lineTo(int x, int y) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::LineTo(getBasicDCHandle(), x, y);
}

template <typename TBase>
BOOL BasicDCT<TBase>::lineTo(POINT pt) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::LineTo(getBasicDCHandle(), pt.x, pt.y);
}

template <typename TBase>
int BasicDCT<TBase>::setROP2(int drawMode) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetROP2(getBasicDCHandle(), drawMode);
}

template <typename TBase>
COLORREF BasicDCT<TBase>::setPixel(int x, int y, COLORREF color) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetPixel(getBasicDCHandle(), x, y, color);
}

template <typename TBase>
BOOL BasicDCT<TBase>::arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::Arc(getBasicDCHandle(), x1, y1, x2, y2, x3, y3, x4, y4);
}

template <typename TBase>
BOOL BasicDCT<TBase>::arc(RECT rc, POINT start, POINT end) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::Arc(getBasicDCHandle(), rc.left, rc.top, rc.right, rc.bottom,
		start.x, start.y, end.x, end.y);
}

template <typename TBase>
BOOL BasicDCT<TBase>::arcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::ArcTo(getBasicDCHandle(), x1, y1, x2, y2, x3, y3, x4, y4);
}

template <typename TBase>
BOOL BasicDCT<TBase>::arcTo(RECT rc, POINT ptStart, POINT ptEnd) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::ArcTo(getBasicDCHandle(), rc.left, rc.top, rc.right, rc.bottom,
		ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
}

template <typename TBase>
BOOL BasicDCT<TBase>::angleArc(int x, int y, int radius, float startAngle, float sweepAngle) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::AngleArc(getBasicDCHandle(), x, y, static_cast<DWORD>(radius), startAngle, sweepAngle);
}

template <typename TBase>
BOOL BasicDCT<TBase>::closeFigure() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::CloseFigure(getBasicDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::getROP2() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetROP2(getBasicDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::getArcDirection() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetArcDirection(getBasicDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::setArcDirection(int arcDirection) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetArcDirection(getBasicDCHandle(), arcDirection);
}

template <typename TBase>
BOOL BasicDCT<TBase>::polyDraw(const POINT* pPointArray, const BYTE* pTypes, int count) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::PolyDraw(getBasicDCHandle(), pPointArray, pTypes, count);
}

template <typename TBase>
BOOL BasicDCT<TBase>::polyline(LPPOINT pPointArray, int count) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::Polyline(getBasicDCHandle(), pPointArray, count);
}

template <typename TBase>
BOOL BasicDCT<TBase>::polyPolyline(const POINT* pPointArray, const DWORD* pPolyPoints, int count) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::PolyPolyline(getBasicDCHandle(), pPointArray, pPolyPoints, static_cast<DWORD>(count));
}

template <typename TBase>
BOOL BasicDCT<TBase>::polylineTo(const POINT* pPointArray, int count) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::PolylineTo(getBasicDCHandle(), pPointArray, static_cast<DWORD>(count));
}

template <typename TBase>
BOOL BasicDCT<TBase>::polyBezier(const POINT* pPointArray, int count) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::PolyBezier(getBasicDCHandle(), pPointArray, static_cast<DWORD>(count));
}

template <typename TBase>
BOOL BasicDCT<TBase>::polyBezierTo(const POINT* pPointArray, int count) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::PolyBezierTo(getBasicDCHandle(), pPointArray, static_cast<DWORD>(count));
}

template <typename TBase>
COLORREF BasicDCT<TBase>::setPixel(POINT pt, COLORREF color) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetPixel(getBasicDCHandle(), pt.x, pt.y, color);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setPixelV(int x, int y, COLORREF color) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetPixelV(getBasicDCHandle(), x, y, color);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setPixelV(POINT pt, COLORREF color) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetPixelV(getBasicDCHandle(), pt.x, pt.y, color);
}

//===========================================================================
// Shape Drawing Functions
//===========================================================================
template <typename TBase>
BOOL BasicDCT<TBase>::drawFocusRect(RECT rc) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::DrawFocusRect(getBasicDCHandle(), &rc);
}

template <typename TBase>
BOOL BasicDCT<TBase>::ellipse(int x1, int y1, int x2, int y2) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::Ellipse(getBasicDCHandle(), x1, y1, x2, y2);
}

template <typename TBase>
BOOL BasicDCT<TBase>::ellipse(RECT rc) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::Ellipse(getBasicDCHandle(), rc.left, rc.top, rc.right, rc.bottom);
}

template <typename TBase>
BOOL BasicDCT<TBase>::polygon(LPPOINT pPointArray, int count) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::Polygon(getBasicDCHandle(), pPointArray, count);
}

template <typename TBase>
BOOL BasicDCT<TBase>::rectangle(int x1, int y1, int x2, int y2) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::Rectangle(getBasicDCHandle(), x1, y1, x2, y2);
}

template <typename TBase>
BOOL BasicDCT<TBase>::rectangle(RECT rc) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::Rectangle(getBasicDCHandle(), rc.left, rc.top, rc.right, rc.bottom);
}

template <typename TBase>
BOOL BasicDCT<TBase>::roundRect(int x1, int y1, int x2, int y2, int width, int height) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::RoundRect(getBasicDCHandle(), x1, y1, x2, y2, width, height);
}

template <typename TBase>
BOOL BasicDCT<TBase>::roundRect(RECT rc, int width, int height) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::RoundRect(getBasicDCHandle(), rc.left, rc.top, rc.right, rc.bottom, width, height);
}

template <typename TBase>
BOOL BasicDCT<TBase>::chord(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::Chord(getBasicDCHandle(), x1, y1, x2, y2, x3, y3, x4, y4);
}

template <typename TBase>
BOOL BasicDCT<TBase>::chord(RECT rc, POINT start, POINT end) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::Chord(getBasicDCHandle(), rc.left, rc.top, rc.right, rc.bottom,
		start.x, start.y, end.x, end.y);
}

template <typename TBase>
BOOL BasicDCT<TBase>::pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::Pie(getBasicDCHandle(), x1, y1, x2, y2, x3, y3, x4, y4);
}

template <typename TBase>
BOOL BasicDCT<TBase>::pie(RECT rc, POINT start, POINT end) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::Pie(getBasicDCHandle(), rc.left, rc.top, rc.right, rc.bottom,
		start.x, start.y, end.x, end.y);
}

template <typename TBase>
BOOL BasicDCT<TBase>::polyPolygon(LPPOINT pPointArray, LPINT pPolyCounts, int count) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::PolyPolygon(getBasicDCHandle(), pPointArray, pPolyCounts, count);
}

//===========================================================================
// Fill and 3D Drawing Functions
//===========================================================================
template <typename TBase>
BOOL BasicDCT<TBase>::fillRect(RECT rc, HBRUSH brush) const
{
	assert(getBasicDCHandle() != nullptr);
	return (::FillRect(getBasicDCHandle(), &rc, brush) ? TRUE : FALSE);
}

template <typename TBase>
BOOL BasicDCT<TBase>::invertRect(RECT rc) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::InvertRect(getBasicDCHandle(), &rc);
}

template <typename TBase>
BOOL BasicDCT<TBase>::drawIconEx(int xLeft, int yTop, HICON icon, int cxWidth, int cyWidth, UINT index, HBRUSH flickerFreeDraw, UINT flags) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::DrawIconEx(getBasicDCHandle(), xLeft, yTop, icon, cxWidth, cyWidth, index, flickerFreeDraw, flags);
}

template <typename TBase>
BOOL BasicDCT<TBase>::drawEdge(RECT rc, UINT edge, UINT flags) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::DrawEdge(getBasicDCHandle(), const_cast<LPRECT>(&rc), edge, flags);
}

template <typename TBase>
BOOL BasicDCT<TBase>::drawFrameControl(RECT rc, UINT type, UINT state) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::DrawFrameControl(getBasicDCHandle(), const_cast<LPRECT>(&rc), type, state);
}

template <typename TBase>
BOOL BasicDCT<TBase>::fillRgn(HRGN rgn, HBRUSH brush) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::FillRgn(getBasicDCHandle(), rgn, brush);
}

template <typename TBase>
BOOL BasicDCT<TBase>::gradientFill(PTRIVERTEX pVertex, ULONG vertex, PVOID pMesh, ULONG mesh, ULONG mode) const
{
	assert(getBasicDCHandle() != nullptr);

	wchar_t systemDirectory[MAX_PATH];
	::GetSystemDirectoryW(systemDirectory, MAX_PATH);

	std::wstring path;
	path.assign(systemDirectory);
	path.append(L"\\msimg32.dll");

	static HMODULE dll = ::LoadLibrary(path.c_str());
	if (dll)
	{
		using pfnGradientFill = UINT(WINAPI*)(HDC, PTRIVERTEX, ULONG, PVOID, ULONG, ULONG);

		pfnGradientFill pfn = reinterpret_cast<pfnGradientFill>(
			reinterpret_cast<void*>(::GetProcAddress(dll, "GradientFill")));

		if (pfn)
		{
			return pfn(getBasicDCHandle(), pVertex, vertex, pMesh, mesh, mode);
		}
	}

	return FALSE;
}

template <typename TBase>
BOOL BasicDCT<TBase>::drawIcon(int x, int y, HICON icon) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::DrawIcon(getBasicDCHandle(), x, y, icon);
}

template <typename TBase>
BOOL BasicDCT<TBase>::drawIcon(POINT pt, HICON icon) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::DrawIcon(getBasicDCHandle(), pt.x, pt.y, icon);
}

template <typename TBase>
BOOL BasicDCT<TBase>::frameRect(RECT rc, HBRUSH brush) const
{
	assert(getBasicDCHandle() != nullptr);
	return (::FrameRect(getBasicDCHandle(), &rc, brush)) ? TRUE : FALSE;
}

template <typename TBase>
BOOL BasicDCT<TBase>::frameRgn(HRGN rgn, HBRUSH brush, int width, int height) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::FrameRgn(getBasicDCHandle(), rgn, brush, width, height);
}

template <typename TBase>
int BasicDCT<TBase>::getPolyFillMode() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetPolyFillMode(getBasicDCHandle());
}

template <typename TBase>
BOOL BasicDCT<TBase>::paintRgn(HRGN rgn) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::PaintRgn(getBasicDCHandle(), rgn);
}

template <typename TBase>
int BasicDCT<TBase>::setPolyFillMode(int polyFillMode) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetPolyFillMode(getBasicDCHandle(), polyFillMode);
}

//===========================================================================
// Bitmap Functions
//===========================================================================
template <typename TBase>
int BasicDCT<TBase>::stretchDIBits(int xDest, int yDest, int destWidth, int destHeight, int xSrc, int ySrc, int srcWidth,
	int srcHeight, LPCVOID pBits, const LPBITMAPINFO pBMI, UINT usage, DWORD rop) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::StretchDIBits(getBasicDCHandle(), xDest, yDest, destWidth, destHeight, xSrc, ySrc, srcWidth, srcHeight, pBits, pBMI, usage, rop);
}

template <typename TBase>
BOOL BasicDCT<TBase>::patBlt(int x, int y, int width, int height, DWORD rop) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::PatBlt(getBasicDCHandle(), x, y, width, height, rop);
}

template <typename TBase>
BOOL BasicDCT<TBase>::bitBlt(int x, int y, int width, int height, HDC hSrc, int xSrc, int ySrc, DWORD rop) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::BitBlt(getBasicDCHandle(), x, y, width, height, hSrc, xSrc, ySrc, rop);
}

template <typename TBase>
BOOL BasicDCT<TBase>::maskBlt(int xDest, int yDest, int width, int height, HDC hSrc, int xSrc, int ySrc, HBITMAP mask, int xMask, int yMask, DWORD rop) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::MaskBlt(getBasicDCHandle(), xDest, yDest, width, height, hSrc, xSrc, ySrc, mask, xMask, yMask, rop);
}

template <typename TBase>
BOOL BasicDCT<TBase>::stretchBlt(int x, int y, int width, int height, HDC src, int xSrc, int ySrc, int srcWidth, int srcHeight, DWORD rop) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::StretchBlt(getBasicDCHandle(), x, y, width, height, src, xSrc, ySrc, srcWidth, srcHeight, rop);
}

template <typename TBase>
int BasicDCT<TBase>::getDIBits(HBITMAP bitmap, UINT startScan, UINT scanLines, LPVOID pBits, LPBITMAPINFO pBMI, UINT usage) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetDIBits(getBasicDCHandle(), bitmap, startScan, scanLines, pBits, pBMI, usage);
}

template <typename TBase>
int BasicDCT<TBase>::setDIBits(HBITMAP bitmap, UINT startScan, UINT scanLines, LPCVOID pBits, LPBITMAPINFO pBMI, UINT colorUse) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetDIBits(getBasicDCHandle(), bitmap, startScan, scanLines, pBits, pBMI, colorUse);
}

template <typename TBase>
int BasicDCT<TBase>::getStretchBltMode() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetStretchBltMode(getBasicDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::setStretchBltMode(int stretchMode) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetStretchBltMode(getBasicDCHandle(), stretchMode);
}

template <typename TBase>
BOOL BasicDCT<TBase>::transparentBlt(int x, int y, int width, int height, HDC hSrc, int xSrc, int ySrc,
	int widthSrc, int heightSrc, UINT transparent) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::TransparentBlt(getBasicDCHandle(), x, y, width, height, hSrc, xSrc, ySrc, widthSrc, heightSrc, transparent);
}

template <typename TBase>
BOOL BasicDCT<TBase>::floodFill(int x, int y, COLORREF color) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::FloodFill(getBasicDCHandle(), x, y, color);
}

template <typename TBase>
BOOL BasicDCT<TBase>::extFloodFill(int x, int y, COLORREF color, UINT fillType) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::ExtFloodFill(getBasicDCHandle(), x, y, color, fillType);
}

//===========================================================================
// coordinate Functions
//===========================================================================
template <typename TBase>
BOOL BasicDCT<TBase>::DPtoLP(LPPOINT pPointArray, int count) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::DPtoLP(getBasicDCHandle(), pPointArray, count);
}

template <typename TBase>
BOOL BasicDCT<TBase>::DPtoLP(RECT& rc) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::DPtoLP(getBasicDCHandle(), reinterpret_cast<LPPOINT>(&rc), 2);
}

template <typename TBase>
BOOL BasicDCT<TBase>::LPtoDP(LPPOINT pPointArray, int count) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::LPtoDP(getBasicDCHandle(), pPointArray, count);
}

template <typename TBase>
BOOL BasicDCT<TBase>::LPtoDP(RECT& rc) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::LPtoDP(getBasicDCHandle(), reinterpret_cast<LPPOINT>(&rc), 2);
}

//===========================================================================
// Layout Functions
//===========================================================================
template <typename TBase>
DWORD BasicDCT<TBase>::getLayout() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetLayout(getBasicDCHandle());
}

template <typename TBase>
DWORD BasicDCT<TBase>::setLayout(DWORD layout) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetLayout(getBasicDCHandle(), layout);
}

//===========================================================================
// Mapping Functions
//===========================================================================
template <typename TBase>
int BasicDCT<TBase>::getMapMode()  const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetMapMode(getBasicDCHandle());
}

template <typename TBase>
BOOL BasicDCT<TBase>::getViewportOrgEx(LPPOINT pPoint)  const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetViewportOrgEx(getBasicDCHandle(), pPoint);
}

template <typename TBase>
int BasicDCT<TBase>::setMapMode(int mapMode) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetMapMode(getBasicDCHandle(), mapMode);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setViewportOrgEx(int x, int y, LPPOINT pPoint) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetViewportOrgEx(getBasicDCHandle(), x, y, pPoint);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setViewportOrgEx(POINT point, LPPOINT pPointRet) const
{
	assert(getBasicDCHandle() != nullptr);
	return setViewportOrgEx(point.x, point.y, pPointRet);
}

template <typename TBase>
BOOL BasicDCT<TBase>::offsetViewportOrgEx(int width, int height, LPPOINT pPoint) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::OffsetViewportOrgEx(getBasicDCHandle(), width, height, pPoint);
}

template <typename TBase>
BOOL BasicDCT<TBase>::getViewportExtEx(LPSIZE pSize)  const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetViewportExtEx(getBasicDCHandle(), pSize);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setViewportExtEx(int x, int y, LPSIZE pSize) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetViewportExtEx(getBasicDCHandle(), x, y, pSize);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setViewportExtEx(SIZE size, LPSIZE pSizeRet) const
{
	assert(getBasicDCHandle() != nullptr);
	return setViewportExtEx(size.cx, size.cy, pSizeRet);
}

template <typename TBase>
BOOL BasicDCT<TBase>::scaleViewportExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE pSize) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::ScaleViewportExtEx(getBasicDCHandle(), xNum, xDenom, yNum, yDenom, pSize);
}

template <typename TBase>
BOOL BasicDCT<TBase>::getWindowOrgEx(LPPOINT pPoint) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetWindowOrgEx(getBasicDCHandle(), pPoint);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setWindowOrgEx(int x, int y, LPPOINT pPoint) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetWindowOrgEx(getBasicDCHandle(), x, y, pPoint);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setWindowOrgEx(POINT point, LPPOINT pPointRet) const
{
	assert(getBasicDCHandle() != nullptr);
	return setWindowOrgEx(point.x, point.y, pPointRet);
}

template <typename TBase>
BOOL BasicDCT<TBase>::offsetWindowOrgEx(int width, int height, LPPOINT pPoint) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::OffsetWindowOrgEx(getBasicDCHandle(), width, height, pPoint);
}

template <typename TBase>
BOOL BasicDCT<TBase>::getWindowExtEx(LPSIZE pSize)  const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetWindowExtEx(getBasicDCHandle(), pSize);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setWindowExtEx(int x, int y, LPSIZE pSize) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetWindowExtEx(getBasicDCHandle(), x, y, pSize);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setWindowExtEx(SIZE size, LPSIZE pSizeRet) const
{
	assert(getBasicDCHandle() != nullptr);
	return setWindowExtEx(size.cx, size.cy, pSizeRet);
}

template <typename TBase>
BOOL BasicDCT<TBase>::scaleWindowExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE pSize) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::ScaleWindowExtEx(getBasicDCHandle(), xNum, xDenom, yNum, yDenom, pSize);
}

//===========================================================================
// MetaFile Functions
//===========================================================================
template <typename TBase>
BOOL BasicDCT<TBase>::playMetaFile(HMETAFILE metaFile) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::PlayMetaFile(getBasicDCHandle(), metaFile);
}

template <typename TBase>
BOOL BasicDCT<TBase>::playMetaFile(HENHMETAFILE enhMetaFile, RECT bounds) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::PlayEnhMetaFile(getBasicDCHandle(), enhMetaFile, &bounds);
}

//===========================================================================
// Printer Functions
//===========================================================================
template <typename TBase>
int BasicDCT<TBase>::startDoc(LPDOCINFO pDocInfo) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::StartDoc(getBasicDCHandle(), pDocInfo);
}

template <typename TBase>
int BasicDCT<TBase>::endDoc() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::EndDoc(getBasicDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::startPage() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::StartPage(getBasicDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::endPage() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::EndPage(getBasicDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::abortDoc() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::AbortDoc(getBasicDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::setAbortProc(BOOL(CALLBACK* pfn)(HDC, int)) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetAbortProc(getBasicDCHandle(), pfn);
}

//===========================================================================
// Text Functions
//===========================================================================
template <typename TBase>
BOOL BasicDCT<TBase>::extTextOut(int x, int y, UINT options, RECT rc, LPCTSTR string, int count, LPINT pDxWidths) const
{
	assert(getBasicDCHandle() != nullptr);

	if (count == -1)
		count = lstrlen(string);

	return ::ExtTextOut(getBasicDCHandle(), x, y, options, &rc, string, static_cast<UINT>(count), pDxWidths);
}

template <typename TBase>
int BasicDCT<TBase>::drawText(LPCTSTR string, int count, RECT rc, UINT format) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::DrawText(getBasicDCHandle(), string, count, const_cast<LPRECT>(&rc), format);
}

template <typename TBase>
UINT BasicDCT<TBase>::getTextAlign() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetTextAlign(getBasicDCHandle());
}

template <typename TBase>
UINT BasicDCT<TBase>::setTextAlign(UINT flags) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetTextAlign(getBasicDCHandle(), flags);
}

template <typename TBase>
int BasicDCT<TBase>::getTextFace(int count, LPTSTR faceName) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetTextFace(getBasicDCHandle(), count, faceName);
}

template <typename TBase>
BOOL BasicDCT<TBase>::getTextMetrics(TEXTMETRIC& metrics) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetTextMetrics(getBasicDCHandle(), &metrics);
}

template <typename TBase>
COLORREF BasicDCT<TBase>::getBkColor() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetBkColor(getBasicDCHandle());
}

template <typename TBase>
COLORREF BasicDCT<TBase>::setBkColor(COLORREF color) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetBkColor(getBasicDCHandle(), color);
}

template <typename TBase>
COLORREF BasicDCT<TBase>::getTextColor() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetTextColor(getBasicDCHandle());
}

template <typename TBase>
COLORREF BasicDCT<TBase>::setTextColor(COLORREF color) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetTextColor(getBasicDCHandle(), color);
}

template <typename TBase>
int BasicDCT<TBase>::getBkMode() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetBkMode(getBasicDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::setBkMode(int bkMode) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetBkMode(getBasicDCHandle(), bkMode);
}

template <typename TBase>
int BasicDCT<TBase>::drawTextEx(LPTSTR string, int count, RECT rc, UINT format, LPDRAWTEXTPARAMS pDTParams) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::DrawTextEx(getBasicDCHandle(), string, count, const_cast<LPRECT>(&rc), format, pDTParams);
}

template <typename TBase>
BOOL BasicDCT<TBase>::getCharABCWidths(UINT firstChar, UINT lastChar, LPABC pABC) const
{
	assert(getBasicDCHandle() != nullptr);
	return::GetCharABCWidths(getBasicDCHandle(), firstChar, lastChar, pABC);
}

template <typename TBase>
DWORD BasicDCT<TBase>::getCharacterPlacement(LPCTSTR string, int count, int maxExtent, LPGCP_RESULTS results, DWORD flags) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetCharacterPlacement(getBasicDCHandle(), string, count, maxExtent, results, flags);
}

template <typename TBase>
BOOL BasicDCT<TBase>::getCharWidth(UINT firstChar, UINT lastChar, int* buffer) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetCharWidth(getBasicDCHandle(), firstChar, lastChar, buffer);
}

template <typename TBase>
BOOL BasicDCT<TBase>::getCharWidthFloat(UINT firstChar, UINT lastChar, float* buffer) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetCharWidthFloat(getBasicDCHandle(), firstChar, lastChar, buffer);
}

template <typename TBase>
CoordSize BasicDCT<TBase>::getTextExtentPoint32(LPCTSTR string, int count) const
{
	assert(getBasicDCHandle() != nullptr);
	assert(string != nullptr);
	assert(count <= lstrlen(string));
	CoordSize sz;
	::GetTextExtentPoint32(getBasicDCHandle(), string, count, &sz);
	return sz;
}

template <typename TBase>
CoordSize BasicDCT<TBase>::getTextExtentPoint32(LPCTSTR string) const
{
	assert(string != nullptr);
	return getTextExtentPoint32(string, lstrlen(string));
}

template <typename TBase>
CoordSize BasicDCT<TBase>::getTabbedTextExtent(LPCTSTR string, int count, int tabPositions, LPINT pTabStopPositions) const
{
	assert(getBasicDCHandle() != nullptr);
	DWORD size = ::GetTabbedTextExtent(getBasicDCHandle(), string, count, tabPositions, pTabStopPositions);
	CoordSize sz(size);
	return sz;
}

template <typename TBase>
BOOL BasicDCT<TBase>::grayString(HBRUSH brush, GRAYSTRINGPROC pOutputFunc, LPARAM pData, int count, int x, int y, int width, int height) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GrayString(getBasicDCHandle(), brush, pOutputFunc, pData, count, x, y, width, height);
}

template <typename TBase>
int BasicDCT<TBase>::setTextJustification(int breakExtra, int breakCount) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetTextJustification(getBasicDCHandle(), breakExtra, breakCount);
}

template <typename TBase>
int BasicDCT<TBase>::getTextCharacterExtra() const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetTextCharacterExtra(getBasicDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::setTextCharacterExtra(int charExtra) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::SetTextCharacterExtra(getBasicDCHandle(), charExtra);
}

template <typename TBase>
CoordSize BasicDCT<TBase>::tabbedTextOut(int x, int y, LPCTSTR string, int count, int tabPositions, LPINT pTabStopPositions, int tabOrigin) const
{
	assert(getBasicDCHandle() != nullptr);
	LONG size = ::TabbedTextOut(getBasicDCHandle(), x, y, string, count, tabPositions, pTabStopPositions, tabOrigin);
	CoordSize sz(static_cast<DWORD>(size));
	return sz;
}

template <typename TBase>
BOOL BasicDCT<TBase>::textOut(int x, int y, LPCTSTR string, int count) const
{
	assert(getBasicDCHandle() != nullptr);
	if (count == -1)
		count = lstrlen(string);

	return ::TextOut(getBasicDCHandle(), x, y, string, count);
}

template <typename TBase>
BOOL BasicDCT<TBase>::getCharABCWidthsI(UINT giFirst, UINT cgi, LPWORD pGI, LPABC pABC) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetCharABCWidthsI(getBasicDCHandle(), giFirst, cgi, pGI, pABC);
}

template <typename TBase>
BOOL BasicDCT<TBase>::getCharWidthI(UINT giFirst, UINT cgi, LPWORD pGI, int* buffer) const
{
	assert(getBasicDCHandle() != nullptr);
	return ::GetCharWidthI(getBasicDCHandle(), giFirst, cgi, pGI, buffer);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




