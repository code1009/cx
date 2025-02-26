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
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::EnumObjects(static_cast<TBase*>(this)->getDCHandle(), objectType, pObjectFunc, lparam);
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
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::RestoreDC(static_cast<TBase*>(this)->getDCHandle(), savedDC);
}

template <typename TBase>
int BasicDCT<TBase>::saveDC() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SaveDC(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
HBITMAP BasicDCT<TBase>::selectBitmap(HBITMAP bitmap) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	HBITMAP oldBitmap = static_cast<HBITMAP>(::SelectObject(static_cast<TBase*>(this)->getDCHandle(), bitmap));

	return oldBitmap;
}

template <typename TBase>
HBRUSH BasicDCT<TBase>::selectBrush(HBRUSH brush) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	HBRUSH oldBrush = static_cast<HBRUSH>(::SelectObject(static_cast<TBase*>(this)->getDCHandle(), brush));

	return oldBrush;
}

template <typename TBase>
HFONT BasicDCT<TBase>::selectFont(HFONT font) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	HFONT oldFont = static_cast<HFONT>(::SelectObject(static_cast<TBase*>(this)->getDCHandle(), font));

	return oldFont;
}

template <typename TBase>
HPEN BasicDCT<TBase>::selectPen(HPEN pen) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	HPEN oldPen = static_cast<HPEN>(::SelectObject(static_cast<TBase*>(this)->getDCHandle(), pen));
	
	return oldPen;
}

template <typename TBase>
int BasicDCT<TBase>::selectRGN(HRGN rgn) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	HANDLE rgnType = ::SelectObject(static_cast<TBase*>(this)->getDCHandle(), rgn);

	return static_cast<int> (reinterpret_cast<INT_PTR>(rgnType));
}

template <typename TBase>
HPALETTE BasicDCT<TBase>::selectPalette(const HPALETTE palette, BOOL forceBkgnd) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	HPALETTE oldPalette = ::SelectPalette(static_cast<TBase*>(this)->getDCHandle(), palette, forceBkgnd);

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
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return static_cast<HBITMAP>(::GetCurrentObject(static_cast<TBase*>(this)->getDCHandle(), OBJ_BITMAP));
}

//===========================================================================
// Brush Functions
//===========================================================================
template <typename TBase>
HBRUSH BasicDCT<TBase>::getCurrentBrush() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return static_cast<HBRUSH>(::GetCurrentObject(static_cast<TBase*>(this)->getDCHandle(), OBJ_BRUSH));
}

template <typename TBase>
CoordPoint BasicDCT<TBase>::getBrushOrgEx() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	CoordPoint pt;
	::GetBrushOrgEx(static_cast<TBase*>(this)->getDCHandle(), &pt);
	return pt;
}

template <typename TBase>
CoordPoint BasicDCT<TBase>::setBrushOrgEx(int x, int y)
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	CoordPoint oldPt;
	::SetBrushOrgEx(static_cast<TBase*>(this)->getDCHandle(), x, y, &oldPt);
	return oldPt;
}

//===========================================================================
// Font Functions
//===========================================================================
template <typename TBase>
HFONT BasicDCT<TBase>::getCurrentFont() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return static_cast<HFONT>(::GetCurrentObject(static_cast<TBase*>(this)->getDCHandle(), OBJ_FONT));
}

//===========================================================================
// Palette Functions
//===========================================================================
template <typename TBase>
HPALETTE BasicDCT<TBase>::getCurrentPalette() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return static_cast<HPALETTE>(::GetCurrentObject(static_cast<TBase*>(this)->getDCHandle(), OBJ_PAL));
}

template <typename TBase>
COLORREF BasicDCT<TBase>::getNearestColor(COLORREF color) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetNearestColor(static_cast<TBase*>(this)->getDCHandle(), color);
}

template <typename TBase>
UINT BasicDCT<TBase>::realizePalette() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::RealizePalette(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
BOOL BasicDCT<TBase>::getColorAdjustment(LPCOLORADJUSTMENT pCA) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetColorAdjustment(static_cast<TBase*>(this)->getDCHandle(), pCA);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setColorAdjustment(const COLORADJUSTMENT* pCA) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetColorAdjustment(static_cast<TBase*>(this)->getDCHandle(), pCA);
}

template <typename TBase>
BOOL BasicDCT<TBase>::updateColors() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::UpdateColors(static_cast<TBase*>(this)->getDCHandle());
}

//===========================================================================
// Pen Functions
//===========================================================================
template <typename TBase>
HPEN BasicDCT<TBase>::getCurrentPen() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return static_cast<HPEN>(::GetCurrentObject(static_cast<TBase*>(this)->getDCHandle(), OBJ_PEN));
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
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	HGDIOBJ stockObject = ::GetStockObject(index);

	HGDIOBJ oldObject = ::SelectObject(static_cast<TBase*>(this)->getDCHandle(), stockObject);

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
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetDeviceCaps(static_cast<TBase*>(this)->getDCHandle(), index);
}

//===========================================================================
// Brush Functions
//===========================================================================
template <typename TBase>
COLORREF BasicDCT<TBase>::getDCBrushColor() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetDCBrushColor(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
COLORREF BasicDCT<TBase>::setDCBrushColor(COLORREF color) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetDCBrushColor(static_cast<TBase*>(this)->getDCHandle(), color);
}

//===========================================================================
// Font Functions
//===========================================================================
template <typename TBase>
DWORD BasicDCT<TBase>::getFontData(DWORD table, DWORD offset, LPVOID buffer, DWORD data) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetFontData(static_cast<TBase*>(this)->getDCHandle(), table, offset, buffer, data);
}

template <typename TBase>
DWORD BasicDCT<TBase>::getFontLanguageInfo() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetFontLanguageInfo(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
DWORD BasicDCT<TBase>::getGlyphOutline(UINT query, UINT format, LPGLYPHMETRICS pGM, DWORD bufSize,
	LPVOID buffer, const MAT2* pMAT2) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetGlyphOutline(static_cast<TBase*>(this)->getDCHandle(), query, format, pGM, bufSize, buffer, pMAT2);
}

template <typename TBase>
DWORD BasicDCT<TBase>::getKerningPairs(DWORD numPairs, LPKERNINGPAIR pKrnPair) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetKerningPairs(static_cast<TBase*>(this)->getDCHandle(), numPairs, pKrnPair);
}

template <typename TBase>
DWORD BasicDCT<TBase>::setMapperFlags(DWORD flag) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetMapperFlags(static_cast<TBase*>(this)->getDCHandle(), flag);
}

template <typename TBase>
BOOL BasicDCT<TBase>::getMiterLimit(PFLOAT pLimit) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetMiterLimit(static_cast<TBase*>(this)->getDCHandle(), pLimit);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setMiterLimit(FLOAT newLimit, PFLOAT pOldLimit) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetMiterLimit(static_cast<TBase*>(this)->getDCHandle(), newLimit, pOldLimit);
}

//===========================================================================
// Clipping Functions
//===========================================================================
template <typename TBase>
int BasicDCT<TBase>::excludeClipRect(int left, int top, int right, int bottom) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::ExcludeClipRect(static_cast<TBase*>(this)->getDCHandle(), left, top, right, bottom);
}

template <typename TBase>
int BasicDCT<TBase>::excludeClipRect(RECT rc) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::ExcludeClipRect(static_cast<TBase*>(this)->getDCHandle(), rc.left, rc.top, rc.right, rc.bottom);
}

template <typename TBase>
int BasicDCT<TBase>::getClipBox(RECT& rc) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetClipBox(static_cast<TBase*>(this)->getDCHandle(), &rc);
}

template <typename TBase>
int BasicDCT<TBase>::intersectClipRect(int left, int top, int right, int bottom) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::IntersectClipRect(static_cast<TBase*>(this)->getDCHandle(), left, top, right, bottom);
}

template <typename TBase>
int BasicDCT<TBase>::intersectClipRect(RECT rc) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::IntersectClipRect(static_cast<TBase*>(this)->getDCHandle(), rc.left, rc.top, rc.right, rc.bottom);
}

template <typename TBase>
BOOL BasicDCT<TBase>::rectVisible(RECT rc) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::RectVisible(static_cast<TBase*>(this)->getDCHandle(), &rc);
}

template <typename TBase>
int BasicDCT<TBase>::selectClipRgn(HRGN rgn) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	int rgnType = ::SelectClipRgn(static_cast<TBase*>(this)->getDCHandle(), rgn);

	return rgnType;
}

template <typename TBase>
BOOL BasicDCT<TBase>::abortPath() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::AbortPath(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
BOOL BasicDCT<TBase>::beginPath() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::BeginPath(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
BOOL BasicDCT<TBase>::endPath() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::EndPath(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::extSelectClipRgn(HRGN rgn, int mode) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	int rgnType = ::ExtSelectClipRgn(static_cast<TBase*>(this)->getDCHandle(), rgn, mode);

	return rgnType;
}

template <typename TBase>
BOOL BasicDCT<TBase>::flattenPath() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::FlattenPath(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::getPath(POINT* pointArray, BYTE* types, int count) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetPath(static_cast<TBase*>(this)->getDCHandle(), pointArray, types, count);
}

template <typename TBase>
BOOL BasicDCT<TBase>::ptVisible(int x, int y) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::PtVisible(static_cast<TBase*>(this)->getDCHandle(), x, y);
}

template <typename TBase>
int BasicDCT<TBase>::offsetClipRgn(int xOffset, int yOffset) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::OffsetClipRgn(static_cast<TBase*>(this)->getDCHandle(), xOffset, yOffset);
}

template <typename TBase>
BOOL BasicDCT<TBase>::selectClipPath(int mode) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SelectClipPath(static_cast<TBase*>(this)->getDCHandle(), mode);
}

template <typename TBase>
BOOL BasicDCT<TBase>::strokeAndFillPath() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::StrokeAndFillPath(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
BOOL BasicDCT<TBase>::strokePath() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::StrokePath(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
BOOL BasicDCT<TBase>::widenPath() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::WidenPath(static_cast<TBase*>(this)->getDCHandle());
}

//===========================================================================
// Point and Line Drawing Functions
//===========================================================================
template <typename TBase>
CoordPoint BasicDCT<TBase>::getCurrentPosition() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	CoordPoint pt;
	::GetCurrentPositionEx(static_cast<TBase*>(this)->getDCHandle(), &pt);
	return pt;
}

template <typename TBase>
COLORREF BasicDCT<TBase>::getPixel(int x, int y) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetPixel(static_cast<TBase*>(this)->getDCHandle(), x, y);
}

template <typename TBase>
COLORREF BasicDCT<TBase>::getPixel(POINT pt) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetPixel(static_cast<TBase*>(this)->getDCHandle(), pt.x, pt.y);
}

template <typename TBase>
CoordPoint BasicDCT<TBase>::moveTo(int x, int y) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	CoordPoint previous;
	::MoveToEx(static_cast<TBase*>(this)->getDCHandle(), x, y, &previous);
	return previous;
}

template <typename TBase>
CoordPoint BasicDCT<TBase>::moveTo(POINT pt) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	CoordPoint previous;
	::MoveToEx(static_cast<TBase*>(this)->getDCHandle(), pt.x, pt.y, &previous);
	return previous;
}

template <typename TBase>
BOOL BasicDCT<TBase>::lineTo(int x, int y) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::LineTo(static_cast<TBase*>(this)->getDCHandle(), x, y);
}

template <typename TBase>
BOOL BasicDCT<TBase>::lineTo(POINT pt) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::LineTo(static_cast<TBase*>(this)->getDCHandle(), pt.x, pt.y);
}

template <typename TBase>
int BasicDCT<TBase>::setROP2(int drawMode) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetROP2(static_cast<TBase*>(this)->getDCHandle(), drawMode);
}

template <typename TBase>
COLORREF BasicDCT<TBase>::setPixel(int x, int y, COLORREF color) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetPixel(static_cast<TBase*>(this)->getDCHandle(), x, y, color);
}

template <typename TBase>
BOOL BasicDCT<TBase>::arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::Arc(static_cast<TBase*>(this)->getDCHandle(), x1, y1, x2, y2, x3, y3, x4, y4);
}

template <typename TBase>
BOOL BasicDCT<TBase>::arc(RECT rc, POINT start, POINT end) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::Arc(static_cast<TBase*>(this)->getDCHandle(), rc.left, rc.top, rc.right, rc.bottom,
		start.x, start.y, end.x, end.y);
}

template <typename TBase>
BOOL BasicDCT<TBase>::arcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::ArcTo(static_cast<TBase*>(this)->getDCHandle(), x1, y1, x2, y2, x3, y3, x4, y4);
}

template <typename TBase>
BOOL BasicDCT<TBase>::arcTo(RECT rc, POINT ptStart, POINT ptEnd) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::ArcTo(static_cast<TBase*>(this)->getDCHandle(), rc.left, rc.top, rc.right, rc.bottom,
		ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
}

template <typename TBase>
BOOL BasicDCT<TBase>::angleArc(int x, int y, int radius, float startAngle, float sweepAngle) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::AngleArc(static_cast<TBase*>(this)->getDCHandle(), x, y, static_cast<DWORD>(radius), startAngle, sweepAngle);
}

template <typename TBase>
BOOL BasicDCT<TBase>::closeFigure() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::CloseFigure(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::getROP2() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetROP2(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::getArcDirection() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetArcDirection(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::setArcDirection(int arcDirection) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetArcDirection(static_cast<TBase*>(this)->getDCHandle(), arcDirection);
}

template <typename TBase>
BOOL BasicDCT<TBase>::polyDraw(const POINT* pPointArray, const BYTE* pTypes, int count) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::PolyDraw(static_cast<TBase*>(this)->getDCHandle(), pPointArray, pTypes, count);
}

template <typename TBase>
BOOL BasicDCT<TBase>::polyline(LPPOINT pPointArray, int count) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::Polyline(static_cast<TBase*>(this)->getDCHandle(), pPointArray, count);
}

template <typename TBase>
BOOL BasicDCT<TBase>::polyPolyline(const POINT* pPointArray, const DWORD* pPolyPoints, int count) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::PolyPolyline(static_cast<TBase*>(this)->getDCHandle(), pPointArray, pPolyPoints, static_cast<DWORD>(count));
}

template <typename TBase>
BOOL BasicDCT<TBase>::polylineTo(const POINT* pPointArray, int count) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::PolylineTo(static_cast<TBase*>(this)->getDCHandle(), pPointArray, static_cast<DWORD>(count));
}

template <typename TBase>
BOOL BasicDCT<TBase>::polyBezier(const POINT* pPointArray, int count) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::PolyBezier(static_cast<TBase*>(this)->getDCHandle(), pPointArray, static_cast<DWORD>(count));
}

template <typename TBase>
BOOL BasicDCT<TBase>::polyBezierTo(const POINT* pPointArray, int count) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::PolyBezierTo(static_cast<TBase*>(this)->getDCHandle(), pPointArray, static_cast<DWORD>(count));
}

template <typename TBase>
COLORREF BasicDCT<TBase>::setPixel(POINT pt, COLORREF color) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetPixel(static_cast<TBase*>(this)->getDCHandle(), pt.x, pt.y, color);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setPixelV(int x, int y, COLORREF color) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetPixelV(static_cast<TBase*>(this)->getDCHandle(), x, y, color);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setPixelV(POINT pt, COLORREF color) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetPixelV(static_cast<TBase*>(this)->getDCHandle(), pt.x, pt.y, color);
}

//===========================================================================
// Shape Drawing Functions
//===========================================================================
template <typename TBase>
BOOL BasicDCT<TBase>::drawFocusRect(RECT rc) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::DrawFocusRect(static_cast<TBase*>(this)->getDCHandle(), &rc);
}

template <typename TBase>
BOOL BasicDCT<TBase>::ellipse(int x1, int y1, int x2, int y2) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::Ellipse(static_cast<TBase*>(this)->getDCHandle(), x1, y1, x2, y2);
}

template <typename TBase>
BOOL BasicDCT<TBase>::ellipse(RECT rc) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::Ellipse(static_cast<TBase*>(this)->getDCHandle(), rc.left, rc.top, rc.right, rc.bottom);
}

template <typename TBase>
BOOL BasicDCT<TBase>::polygon(LPPOINT pPointArray, int count) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::Polygon(static_cast<TBase*>(this)->getDCHandle(), pPointArray, count);
}

template <typename TBase>
BOOL BasicDCT<TBase>::rectangle(int x1, int y1, int x2, int y2) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::Rectangle(static_cast<TBase*>(this)->getDCHandle(), x1, y1, x2, y2);
}

template <typename TBase>
BOOL BasicDCT<TBase>::rectangle(RECT rc) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::Rectangle(static_cast<TBase*>(this)->getDCHandle(), rc.left, rc.top, rc.right, rc.bottom);
}

template <typename TBase>
BOOL BasicDCT<TBase>::roundRect(int x1, int y1, int x2, int y2, int width, int height) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::RoundRect(static_cast<TBase*>(this)->getDCHandle(), x1, y1, x2, y2, width, height);
}

template <typename TBase>
BOOL BasicDCT<TBase>::roundRect(RECT rc, int width, int height) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::RoundRect(static_cast<TBase*>(this)->getDCHandle(), rc.left, rc.top, rc.right, rc.bottom, width, height);
}

template <typename TBase>
BOOL BasicDCT<TBase>::chord(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::Chord(static_cast<TBase*>(this)->getDCHandle(), x1, y1, x2, y2, x3, y3, x4, y4);
}

template <typename TBase>
BOOL BasicDCT<TBase>::chord(RECT rc, POINT start, POINT end) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::Chord(static_cast<TBase*>(this)->getDCHandle(), rc.left, rc.top, rc.right, rc.bottom,
		start.x, start.y, end.x, end.y);
}

template <typename TBase>
BOOL BasicDCT<TBase>::pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::Pie(static_cast<TBase*>(this)->getDCHandle(), x1, y1, x2, y2, x3, y3, x4, y4);
}

template <typename TBase>
BOOL BasicDCT<TBase>::pie(RECT rc, POINT start, POINT end) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::Pie(static_cast<TBase*>(this)->getDCHandle(), rc.left, rc.top, rc.right, rc.bottom,
		start.x, start.y, end.x, end.y);
}

template <typename TBase>
BOOL BasicDCT<TBase>::polyPolygon(LPPOINT pPointArray, LPINT pPolyCounts, int count) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::PolyPolygon(static_cast<TBase*>(this)->getDCHandle(), pPointArray, pPolyCounts, count);
}

//===========================================================================
// Fill and 3D Drawing Functions
//===========================================================================
template <typename TBase>
BOOL BasicDCT<TBase>::fillRect(RECT rc, HBRUSH brush) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return (::FillRect(static_cast<TBase*>(this)->getDCHandle(), &rc, brush) ? TRUE : FALSE);
}

template <typename TBase>
BOOL BasicDCT<TBase>::invertRect(RECT rc) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::InvertRect(static_cast<TBase*>(this)->getDCHandle(), &rc);
}

template <typename TBase>
BOOL BasicDCT<TBase>::drawIconEx(int xLeft, int yTop, HICON icon, int cxWidth, int cyWidth, UINT index, HBRUSH flickerFreeDraw, UINT flags) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::DrawIconEx(static_cast<TBase*>(this)->getDCHandle(), xLeft, yTop, icon, cxWidth, cyWidth, index, flickerFreeDraw, flags);
}

template <typename TBase>
BOOL BasicDCT<TBase>::drawEdge(RECT rc, UINT edge, UINT flags) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::DrawEdge(static_cast<TBase*>(this)->getDCHandle(), const_cast<LPRECT>(&rc), edge, flags);
}

template <typename TBase>
BOOL BasicDCT<TBase>::drawFrameControl(RECT rc, UINT type, UINT state) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::DrawFrameControl(static_cast<TBase*>(this)->getDCHandle(), const_cast<LPRECT>(&rc), type, state);
}

template <typename TBase>
BOOL BasicDCT<TBase>::fillRgn(HRGN rgn, HBRUSH brush) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::FillRgn(static_cast<TBase*>(this)->getDCHandle(), rgn, brush);
}

template <typename TBase>
BOOL BasicDCT<TBase>::gradientFill(PTRIVERTEX pVertex, ULONG vertex, PVOID pMesh, ULONG mesh, ULONG mode) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);

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
			return pfn(static_cast<TBase*>(this)->getDCHandle(), pVertex, vertex, pMesh, mesh, mode);
		}
	}

	return FALSE;
}

template <typename TBase>
BOOL BasicDCT<TBase>::drawIcon(int x, int y, HICON icon) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::DrawIcon(static_cast<TBase*>(this)->getDCHandle(), x, y, icon);
}

template <typename TBase>
BOOL BasicDCT<TBase>::drawIcon(POINT pt, HICON icon) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::DrawIcon(static_cast<TBase*>(this)->getDCHandle(), pt.x, pt.y, icon);
}

template <typename TBase>
BOOL BasicDCT<TBase>::frameRect(RECT rc, HBRUSH brush) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return (::FrameRect(static_cast<TBase*>(this)->getDCHandle(), &rc, brush)) ? TRUE : FALSE;
}

template <typename TBase>
BOOL BasicDCT<TBase>::frameRgn(HRGN rgn, HBRUSH brush, int width, int height) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::FrameRgn(static_cast<TBase*>(this)->getDCHandle(), rgn, brush, width, height);
}

template <typename TBase>
int BasicDCT<TBase>::getPolyFillMode() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetPolyFillMode(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
BOOL BasicDCT<TBase>::paintRgn(HRGN rgn) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::PaintRgn(static_cast<TBase*>(this)->getDCHandle(), rgn);
}

template <typename TBase>
int BasicDCT<TBase>::setPolyFillMode(int polyFillMode) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetPolyFillMode(static_cast<TBase*>(this)->getDCHandle(), polyFillMode);
}

//===========================================================================
// Bitmap Functions
//===========================================================================
template <typename TBase>
int BasicDCT<TBase>::stretchDIBits(int xDest, int yDest, int destWidth, int destHeight, int xSrc, int ySrc, int srcWidth,
	int srcHeight, LPCVOID pBits, const LPBITMAPINFO pBMI, UINT usage, DWORD rop) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::StretchDIBits(static_cast<TBase*>(this)->getDCHandle(), xDest, yDest, destWidth, destHeight, xSrc, ySrc, srcWidth, srcHeight, pBits, pBMI, usage, rop);
}

template <typename TBase>
BOOL BasicDCT<TBase>::patBlt(int x, int y, int width, int height, DWORD rop) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::PatBlt(static_cast<TBase*>(this)->getDCHandle(), x, y, width, height, rop);
}

template <typename TBase>
BOOL BasicDCT<TBase>::bitBlt(int x, int y, int width, int height, HDC hSrc, int xSrc, int ySrc, DWORD rop) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::BitBlt(static_cast<TBase*>(this)->getDCHandle(), x, y, width, height, hSrc, xSrc, ySrc, rop);
}

template <typename TBase>
BOOL BasicDCT<TBase>::maskBlt(int xDest, int yDest, int width, int height, HDC hSrc, int xSrc, int ySrc, HBITMAP mask, int xMask, int yMask, DWORD rop) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::MaskBlt(static_cast<TBase*>(this)->getDCHandle(), xDest, yDest, width, height, hSrc, xSrc, ySrc, mask, xMask, yMask, rop);
}

template <typename TBase>
BOOL BasicDCT<TBase>::stretchBlt(int x, int y, int width, int height, HDC src, int xSrc, int ySrc, int srcWidth, int srcHeight, DWORD rop) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::StretchBlt(static_cast<TBase*>(this)->getDCHandle(), x, y, width, height, src, xSrc, ySrc, srcWidth, srcHeight, rop);
}

template <typename TBase>
int BasicDCT<TBase>::getDIBits(HBITMAP bitmap, UINT startScan, UINT scanLines, LPVOID pBits, LPBITMAPINFO pBMI, UINT usage) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetDIBits(static_cast<TBase*>(this)->getDCHandle(), bitmap, startScan, scanLines, pBits, pBMI, usage);
}

template <typename TBase>
int BasicDCT<TBase>::setDIBits(HBITMAP bitmap, UINT startScan, UINT scanLines, LPCVOID pBits, LPBITMAPINFO pBMI, UINT colorUse) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetDIBits(static_cast<TBase*>(this)->getDCHandle(), bitmap, startScan, scanLines, pBits, pBMI, colorUse);
}

template <typename TBase>
int BasicDCT<TBase>::getStretchBltMode() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetStretchBltMode(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::setStretchBltMode(int stretchMode) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetStretchBltMode(static_cast<TBase*>(this)->getDCHandle(), stretchMode);
}

template <typename TBase>
BOOL BasicDCT<TBase>::transparentBlt(int x, int y, int width, int height, HDC hSrc, int xSrc, int ySrc,
	int widthSrc, int heightSrc, UINT transparent) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::TransparentBlt(static_cast<TBase*>(this)->getDCHandle(), x, y, width, height, hSrc, xSrc, ySrc, widthSrc, heightSrc, transparent);
}

template <typename TBase>
BOOL BasicDCT<TBase>::floodFill(int x, int y, COLORREF color) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::FloodFill(static_cast<TBase*>(this)->getDCHandle(), x, y, color);
}

template <typename TBase>
BOOL BasicDCT<TBase>::extFloodFill(int x, int y, COLORREF color, UINT fillType) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::ExtFloodFill(static_cast<TBase*>(this)->getDCHandle(), x, y, color, fillType);
}

//===========================================================================
// coordinate Functions
//===========================================================================
template <typename TBase>
BOOL BasicDCT<TBase>::DPtoLP(LPPOINT pPointArray, int count) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::DPtoLP(static_cast<TBase*>(this)->getDCHandle(), pPointArray, count);
}

template <typename TBase>
BOOL BasicDCT<TBase>::DPtoLP(RECT& rc) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::DPtoLP(static_cast<TBase*>(this)->getDCHandle(), reinterpret_cast<LPPOINT>(&rc), 2);
}

template <typename TBase>
BOOL BasicDCT<TBase>::LPtoDP(LPPOINT pPointArray, int count) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::LPtoDP(static_cast<TBase*>(this)->getDCHandle(), pPointArray, count);
}

template <typename TBase>
BOOL BasicDCT<TBase>::LPtoDP(RECT& rc) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::LPtoDP(static_cast<TBase*>(this)->getDCHandle(), reinterpret_cast<LPPOINT>(&rc), 2);
}

//===========================================================================
// Layout Functions
//===========================================================================
template <typename TBase>
DWORD BasicDCT<TBase>::getLayout() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetLayout(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
DWORD BasicDCT<TBase>::setLayout(DWORD layout) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetLayout(static_cast<TBase*>(this)->getDCHandle(), layout);
}

//===========================================================================
// Mapping Functions
//===========================================================================
template <typename TBase>
int BasicDCT<TBase>::getMapMode()  const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetMapMode(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
BOOL BasicDCT<TBase>::getViewportOrgEx(LPPOINT pPoint)  const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetViewportOrgEx(static_cast<TBase*>(this)->getDCHandle(), pPoint);
}

template <typename TBase>
int BasicDCT<TBase>::setMapMode(int mapMode) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetMapMode(static_cast<TBase*>(this)->getDCHandle(), mapMode);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setViewportOrgEx(int x, int y, LPPOINT pPoint) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetViewportOrgEx(static_cast<TBase*>(this)->getDCHandle(), x, y, pPoint);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setViewportOrgEx(POINT point, LPPOINT pPointRet) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return setViewportOrgEx(point.x, point.y, pPointRet);
}

template <typename TBase>
BOOL BasicDCT<TBase>::offsetViewportOrgEx(int width, int height, LPPOINT pPoint) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::OffsetViewportOrgEx(static_cast<TBase*>(this)->getDCHandle(), width, height, pPoint);
}

template <typename TBase>
BOOL BasicDCT<TBase>::getViewportExtEx(LPSIZE pSize)  const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetViewportExtEx(static_cast<TBase*>(this)->getDCHandle(), pSize);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setViewportExtEx(int x, int y, LPSIZE pSize) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetViewportExtEx(static_cast<TBase*>(this)->getDCHandle(), x, y, pSize);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setViewportExtEx(SIZE size, LPSIZE pSizeRet) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return setViewportExtEx(size.cx, size.cy, pSizeRet);
}

template <typename TBase>
BOOL BasicDCT<TBase>::scaleViewportExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE pSize) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::ScaleViewportExtEx(static_cast<TBase*>(this)->getDCHandle(), xNum, xDenom, yNum, yDenom, pSize);
}

template <typename TBase>
BOOL BasicDCT<TBase>::getWindowOrgEx(LPPOINT pPoint) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetWindowOrgEx(static_cast<TBase*>(this)->getDCHandle(), pPoint);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setWindowOrgEx(int x, int y, LPPOINT pPoint) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetWindowOrgEx(static_cast<TBase*>(this)->getDCHandle(), x, y, pPoint);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setWindowOrgEx(POINT point, LPPOINT pPointRet) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return setWindowOrgEx(point.x, point.y, pPointRet);
}

template <typename TBase>
BOOL BasicDCT<TBase>::offsetWindowOrgEx(int width, int height, LPPOINT pPoint) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::OffsetWindowOrgEx(static_cast<TBase*>(this)->getDCHandle(), width, height, pPoint);
}

template <typename TBase>
BOOL BasicDCT<TBase>::getWindowExtEx(LPSIZE pSize)  const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetWindowExtEx(static_cast<TBase*>(this)->getDCHandle(), pSize);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setWindowExtEx(int x, int y, LPSIZE pSize) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetWindowExtEx(static_cast<TBase*>(this)->getDCHandle(), x, y, pSize);
}

template <typename TBase>
BOOL BasicDCT<TBase>::setWindowExtEx(SIZE size, LPSIZE pSizeRet) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return setWindowExtEx(size.cx, size.cy, pSizeRet);
}

template <typename TBase>
BOOL BasicDCT<TBase>::scaleWindowExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE pSize) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::ScaleWindowExtEx(static_cast<TBase*>(this)->getDCHandle(), xNum, xDenom, yNum, yDenom, pSize);
}

//===========================================================================
// MetaFile Functions
//===========================================================================
template <typename TBase>
BOOL BasicDCT<TBase>::playMetaFile(HMETAFILE metaFile) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::PlayMetaFile(static_cast<TBase*>(this)->getDCHandle(), metaFile);
}

template <typename TBase>
BOOL BasicDCT<TBase>::playMetaFile(HENHMETAFILE enhMetaFile, RECT bounds) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::PlayEnhMetaFile(static_cast<TBase*>(this)->getDCHandle(), enhMetaFile, &bounds);
}

//===========================================================================
// Printer Functions
//===========================================================================
template <typename TBase>
int BasicDCT<TBase>::startDoc(LPDOCINFO pDocInfo) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::StartDoc(static_cast<TBase*>(this)->getDCHandle(), pDocInfo);
}

template <typename TBase>
int BasicDCT<TBase>::endDoc() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::EndDoc(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::startPage() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::StartPage(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::endPage() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::EndPage(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::abortDoc() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::AbortDoc(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::setAbortProc(BOOL(CALLBACK* pfn)(HDC, int)) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetAbortProc(static_cast<TBase*>(this)->getDCHandle(), pfn);
}

//===========================================================================
// Text Functions
//===========================================================================
template <typename TBase>
BOOL BasicDCT<TBase>::extTextOut(int x, int y, UINT options, RECT rc, LPCTSTR string, int count, LPINT pDxWidths) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);

	if (count == -1)
		count = lstrlen(string);

	return ::ExtTextOut(static_cast<TBase*>(this)->getDCHandle(), x, y, options, &rc, string, static_cast<UINT>(count), pDxWidths);
}

template <typename TBase>
int BasicDCT<TBase>::drawText(LPCTSTR string, int count, RECT rc, UINT format) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::DrawText(static_cast<TBase*>(this)->getDCHandle(), string, count, const_cast<LPRECT>(&rc), format);
}

template <typename TBase>
UINT BasicDCT<TBase>::getTextAlign() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetTextAlign(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
UINT BasicDCT<TBase>::setTextAlign(UINT flags) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetTextAlign(static_cast<TBase*>(this)->getDCHandle(), flags);
}

template <typename TBase>
int BasicDCT<TBase>::getTextFace(int count, LPTSTR faceName) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetTextFace(static_cast<TBase*>(this)->getDCHandle(), count, faceName);
}

template <typename TBase>
BOOL BasicDCT<TBase>::getTextMetrics(TEXTMETRIC& metrics) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetTextMetrics(static_cast<TBase*>(this)->getDCHandle(), &metrics);
}

template <typename TBase>
COLORREF BasicDCT<TBase>::getBkColor() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetBkColor(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
COLORREF BasicDCT<TBase>::setBkColor(COLORREF color) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetBkColor(static_cast<TBase*>(this)->getDCHandle(), color);
}

template <typename TBase>
COLORREF BasicDCT<TBase>::getTextColor() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetTextColor(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
COLORREF BasicDCT<TBase>::setTextColor(COLORREF color) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetTextColor(static_cast<TBase*>(this)->getDCHandle(), color);
}

template <typename TBase>
int BasicDCT<TBase>::getBkMode() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetBkMode(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::setBkMode(int bkMode) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetBkMode(static_cast<TBase*>(this)->getDCHandle(), bkMode);
}

template <typename TBase>
int BasicDCT<TBase>::drawTextEx(LPTSTR string, int count, RECT rc, UINT format, LPDRAWTEXTPARAMS pDTParams) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::DrawTextEx(static_cast<TBase*>(this)->getDCHandle(), string, count, const_cast<LPRECT>(&rc), format, pDTParams);
}

template <typename TBase>
BOOL BasicDCT<TBase>::getCharABCWidths(UINT firstChar, UINT lastChar, LPABC pABC) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return::GetCharABCWidths(static_cast<TBase*>(this)->getDCHandle(), firstChar, lastChar, pABC);
}

template <typename TBase>
DWORD BasicDCT<TBase>::getCharacterPlacement(LPCTSTR string, int count, int maxExtent, LPGCP_RESULTS results, DWORD flags) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetCharacterPlacement(static_cast<TBase*>(this)->getDCHandle(), string, count, maxExtent, results, flags);
}

template <typename TBase>
BOOL BasicDCT<TBase>::getCharWidth(UINT firstChar, UINT lastChar, int* buffer) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetCharWidth(static_cast<TBase*>(this)->getDCHandle(), firstChar, lastChar, buffer);
}

template <typename TBase>
BOOL BasicDCT<TBase>::getCharWidthFloat(UINT firstChar, UINT lastChar, float* buffer) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetCharWidthFloat(static_cast<TBase*>(this)->getDCHandle(), firstChar, lastChar, buffer);
}

template <typename TBase>
CoordSize BasicDCT<TBase>::getTextExtentPoint32(LPCTSTR string, int count) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	assert(string != nullptr);
	assert(count <= lstrlen(string));
	CoordSize sz;
	::GetTextExtentPoint32(static_cast<TBase*>(this)->getDCHandle(), string, count, &sz);
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
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	DWORD size = ::GetTabbedTextExtent(static_cast<TBase*>(this)->getDCHandle(), string, count, tabPositions, pTabStopPositions);
	CoordSize sz(size);
	return sz;
}

template <typename TBase>
BOOL BasicDCT<TBase>::grayString(HBRUSH brush, GRAYSTRINGPROC pOutputFunc, LPARAM pData, int count, int x, int y, int width, int height) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GrayString(static_cast<TBase*>(this)->getDCHandle(), brush, pOutputFunc, pData, count, x, y, width, height);
}

template <typename TBase>
int BasicDCT<TBase>::setTextJustification(int breakExtra, int breakCount) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetTextJustification(static_cast<TBase*>(this)->getDCHandle(), breakExtra, breakCount);
}

template <typename TBase>
int BasicDCT<TBase>::getTextCharacterExtra() const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetTextCharacterExtra(static_cast<TBase*>(this)->getDCHandle());
}

template <typename TBase>
int BasicDCT<TBase>::setTextCharacterExtra(int charExtra) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::SetTextCharacterExtra(static_cast<TBase*>(this)->getDCHandle(), charExtra);
}

template <typename TBase>
CoordSize BasicDCT<TBase>::tabbedTextOut(int x, int y, LPCTSTR string, int count, int tabPositions, LPINT pTabStopPositions, int tabOrigin) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	LONG size = ::TabbedTextOut(static_cast<TBase*>(this)->getDCHandle(), x, y, string, count, tabPositions, pTabStopPositions, tabOrigin);
	CoordSize sz(static_cast<DWORD>(size));
	return sz;
}

template <typename TBase>
BOOL BasicDCT<TBase>::textOut(int x, int y, LPCTSTR string, int count) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	if (count == -1)
		count = lstrlen(string);

	return ::TextOut(static_cast<TBase*>(this)->getDCHandle(), x, y, string, count);
}

template <typename TBase>
BOOL BasicDCT<TBase>::getCharABCWidthsI(UINT giFirst, UINT cgi, LPWORD pGI, LPABC pABC) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetCharABCWidthsI(static_cast<TBase*>(this)->getDCHandle(), giFirst, cgi, pGI, pABC);
}

template <typename TBase>
BOOL BasicDCT<TBase>::getCharWidthI(UINT giFirst, UINT cgi, LPWORD pGI, int* buffer) const
{
	assert(static_cast<TBase*>(this)->getDCHandle() != nullptr);
	return ::GetCharWidthI(static_cast<TBase*>(this)->getDCHandle(), giFirst, cgi, pGI, buffer);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




