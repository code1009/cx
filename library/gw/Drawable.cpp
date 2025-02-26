/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "gw.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool Drawable::getVisible(void) const
{
	return _Visible;
}

bool Drawable::setVisible(bool value)
{
	if (_Visible != value)
	{
		_Visible = value;

		setVisibleChanged();

		return true;
	}

	return false;
}

void Drawable::setVisibleChangedHandler(VisibleChangedHandler handler)
{
	_VisibleChangedHandler = handler;
}

void Drawable::setVisibleChanged(void)
{
	if (_VisibleChangedHandler)
	{
		_VisibleChangedHandler(this);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
DocumentGrid::DocumentGrid(Viewport* viewport) :
	_Viewport(viewport)
{
}

//===========================================================================
DocumentGrid::~DocumentGrid()
{
	destroyDeviceResources();
	destroyDeviceIndependentResources();
}

//===========================================================================
bool DocumentGrid::createDeviceIndependentResources(Context* ctx)
{
	HRESULT hr;


	if (!_pCoord_TextFormat)
	{
		hr = ctx->getDWriteFactory()->CreateTextFormat(
			L"Arial",
			nullptr,
			DWRITE_FONT_WEIGHT_THIN,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_ULTRA_CONDENSED,
			9.0f,
			L"ko-kr",
			&_pCoord_TextFormat
		);
		if (FAILED(hr))
		{
			return false;
		}
		_pCoord_TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		_pCoord_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	}

	return true;
}

void DocumentGrid::destroyDeviceIndependentResources(void)
{
	if (_pCoord_TextFormat)
	{
		_pCoord_TextFormat->Release();
		_pCoord_TextFormat = nullptr;
	}
}

//===========================================================================
bool DocumentGrid::createDeviceResources(Context* ctx)
{
	HRESULT hr; 
	
	
	//-----------------------------------------------------------------------
	if (!_pDocumentGrid0_LineBrush)
	{
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.9f, 0.9f, 0.9f),
			&_pDocumentGrid0_LineBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pDocumentGrid1_LineBrush)
	{
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.5f, 0.5f, 0.5f),
			&_pDocumentGrid1_LineBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pDocumentGrid2_LineBrush)
	{
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.5f, 0.5f, 1.0f),
			&_pDocumentGrid2_LineBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	//-----------------------------------------------------------------------
	if (!_pCoord_TextBrush)
	{
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.5f, 0.5f, 0.5f, 0.5f),
			&_pCoord_TextBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	return true;
}

void DocumentGrid::destroyDeviceResources(void)
{
	//-----------------------------------------------------------------------
	if (_pDocumentGrid0_LineBrush)
	{
		_pDocumentGrid0_LineBrush->Release();
		_pDocumentGrid0_LineBrush = nullptr;
	}
	if (_pDocumentGrid1_LineBrush)
	{
		_pDocumentGrid1_LineBrush->Release();
		_pDocumentGrid1_LineBrush = nullptr;
	}
	if (_pDocumentGrid2_LineBrush)
	{
		_pDocumentGrid2_LineBrush->Release();
		_pDocumentGrid2_LineBrush = nullptr;
	}

	//-----------------------------------------------------------------------
	if (_pCoord_TextBrush)
	{
		_pCoord_TextBrush->Release();
		_pCoord_TextBrush = nullptr;
	}
}

//===========================================================================
void DocumentGrid::draw(Context* ctx)
{
	if (getVisible())
	{
		drawDocumentGrid(ctx);
	}
}

//===========================================================================
void DocumentGrid::drawDocumentGrid(Context* ctx)
{
	//-----------------------------------------------------------------------
	double document_cx;
	double document_cy;


	_Viewport->getDocumentSize(document_cx, document_cy);


	//-----------------------------------------------------------------------
	double viewport_x;
	double viewport_y;
	double viewport_cx;
	double viewport_cy;


	_Viewport->getDocumentViewportPoint(viewport_x, viewport_y);
	_Viewport->getDocumentViewportSize(viewport_cx, viewport_cy);


	//-----------------------------------------------------------------------
	float line0_dx = 20.0f;
	float line0_dy = 20.0f;
	float line1_dx = 100.0f;
	float line1_dy = 100.0f;


	//-----------------------------------------------------------------------
	float ob;
	float oe;
	float cb;
	float ce;
	float ci;
	float cd;


	//-----------------------------------------------------------------------
	// Draw horizontal lines
	cd = static_cast<float>(line0_dx);
	cb = static_cast<float>(0.0f);
	ce = static_cast<float>(document_cx);
	ob = static_cast<float>(0.0f);
	oe = static_cast<float>(document_cy);
	for (ci = cb; ci < ce; ci += cd)
	{
		ctx->getD2dRenderTarget()->DrawLine(
			D2D1::Point2F(ci, ob),
			D2D1::Point2F(ci, oe),
			_pDocumentGrid0_LineBrush,
			1.0f
		);
	}
	// Draw vertical lines
	cd = static_cast<float>(line0_dy);
	cb = static_cast<float>(0.0f);
	ce = static_cast<float>(document_cy);
	ob = static_cast<float>(0.0f);
	oe = static_cast<float>(document_cx);
	for (ci = cb; ci < ce; ci += cd)
	{
		ctx->getD2dRenderTarget()->DrawLine(
			D2D1::Point2F(ob, ci),
			D2D1::Point2F(oe, ci),
			_pDocumentGrid0_LineBrush,
			1.0f
		);
	}

	//-----------------------------------------------------------------------
	// Draw horizontal lines
	cd = static_cast<float>(line1_dx);
	cb = static_cast<float>(0.0f);
	ce = static_cast<float>(document_cx);
	ob = static_cast<float>(0.0f);
	oe = static_cast<float>(document_cy);
	for (ci = cb; ci < ce; ci += cd)
	{
		ctx->getD2dRenderTarget()->DrawLine(
			D2D1::Point2F(ci, ob),
			D2D1::Point2F(ci, oe),
			_pDocumentGrid1_LineBrush,
			1.0f
		);
	}
	// Draw vertical lines
	cd = static_cast<float>(line1_dy);
	cb = static_cast<float>(0.0f);
	ce = static_cast<float>(document_cy);
	ob = static_cast<float>(0.0f);
	oe = static_cast<float>(document_cx);
	for (ci = cb; ci < ce; ci += cd)
	{
		ctx->getD2dRenderTarget()->DrawLine(
			D2D1::Point2F(ob, ci),
			D2D1::Point2F(oe, ci),
			_pDocumentGrid1_LineBrush,
			1.0f
		);
	}


	//-----------------------------------------------------------------------
	// Draw center lines
	cb = static_cast<float>(0.0f);
	ce = static_cast<float>(document_cx);
	ob = static_cast<float>(0.0f);
	oe = static_cast<float>(document_cy);
	ci = (ce - cb) / 2.0f;
	ctx->getD2dRenderTarget()->DrawLine(
		D2D1::Point2F(ci, ob),
		D2D1::Point2F(ci, oe),
		_pDocumentGrid2_LineBrush,
		2.0f
	);

	cb = static_cast<float>(0.0f);
	ce = static_cast<float>(document_cy);
	ob = static_cast<float>(0.0f);
	oe = static_cast<float>(document_cx);
	ci = (ce - cb) / 2.0f;
	ctx->getD2dRenderTarget()->DrawLine(
		D2D1::Point2F(ob, ci),
		D2D1::Point2F(oe, ci),
		_pDocumentGrid2_LineBrush,
		2.0f
	);


	//-----------------------------------------------------------------------
	// Draw the outline of a rectangle.
	D2D1_RECT_F rect;


	rect.left   = static_cast<float>(0.0f);
	rect.top    = static_cast<float>(0.0f);
	rect.right  = static_cast<float>(document_cx);
	rect.bottom = static_cast<float>(document_cy);
	ctx->getD2dRenderTarget()->DrawRectangle(
		&rect,
		_pDocumentGrid2_LineBrush,
		4.0f
	);


	//-----------------------------------------------------------------------
	// Draw coordinate text
	float tx  = static_cast<float>(0.0f);
	float ty  = static_cast<float>(0.0f);
	float tbx = static_cast<float>(0.0f);
	float tby = static_cast<float>(0.0f);
	float tcx = static_cast<float>(document_cx);
	float tcy = static_cast<float>(document_cy);
	float tdx = static_cast<float>(line1_dx);
	float tdy = static_cast<float>(line1_dy);

	wchar_t text[128];


	for (ty = tby; ty < tcy; ty += tdy)
	{
		for (tx = tbx; tx < tcx; tx += tdx)
		{
			rect.left   = tx + 2.0f;
			rect.top    = ty + 2.0f;
			rect.right  = rect.left + 50.0f;
			rect.bottom = rect.top  + 15.0f;

			swprintf_s(text, L"(%d,%d)", 
				static_cast<int>(tx / tdx),
				static_cast<int>(ty / tdy));

			ctx->getD2dRenderTarget()->DrawTextW(
				text,
				static_cast<UINT32>(lstrlen(text)),
				_pCoord_TextFormat,
				rect,
				_pCoord_TextBrush
			);
		}
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
StatusOverayPanel::StatusOverayPanel(Viewport* viewport) :
	_Viewport{ viewport }
{
	_LastDrawTime = std::chrono::steady_clock::now();
}

//===========================================================================
StatusOverayPanel::~StatusOverayPanel()
{
	destroyDeviceResources();
	destroyDeviceIndependentResources();
}

//===========================================================================
bool StatusOverayPanel::createDeviceIndependentResources(Context* ctx)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	if (!_pStatus_TextFormat)
	{
		hr = ctx->getDWriteFactory()->CreateTextFormat(
			//L"Arial",
			L"돋움",
			//L"FixedSys",
			nullptr,
			DWRITE_FONT_WEIGHT_ULTRA_BLACK, // DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			12.0f,
			//L"en-us",
			L"ko-kr",
			&_pStatus_TextFormat
		);
		if (FAILED(hr))
		{
			return false;
		}
		//_pStatus_TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER); // hcenter
		//_pStatus_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER); // vcenter
	}

	return true;
}

void StatusOverayPanel::destroyDeviceIndependentResources(void)
{
	//-----------------------------------------------------------------------
	if (_pStatus_TextFormat)
	{
		_pStatus_TextFormat->Release();
		_pStatus_TextFormat = nullptr;
	}
}

//===========================================================================
bool StatusOverayPanel::createDeviceResources(Context* ctx)
{
	//-----------------------------------------------------------------------
	HRESULT hr; 
	
	
	//-----------------------------------------------------------------------
	if (!_pStatusBox_FillBrush)
	{
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.75f, 0.75f, 0.75f, 0.25f),
			&_pStatusBox_FillBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pStatusBox_LineBrush)
	{
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.5f, 0.5f, 0.5f, 0.5f),
			&_pStatusBox_LineBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	//-----------------------------------------------------------------------
	if (!_pStatus_TextBrush)
	{
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.5f, 0.5f, 0.5f, 1.0f),
			&_pStatus_TextBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	return true;
}

void StatusOverayPanel::destroyDeviceResources(void)
{
	//-----------------------------------------------------------------------
	if (_pStatusBox_FillBrush)
	{
		_pStatusBox_FillBrush->Release();
		_pStatusBox_FillBrush = nullptr;
	}
	if (_pStatusBox_LineBrush)
	{
		_pStatusBox_LineBrush->Release();
		_pStatusBox_LineBrush = nullptr;
	}

	//-----------------------------------------------------------------------
	if (_pStatus_TextBrush)
	{
		_pStatus_TextBrush->Release();
		_pStatus_TextBrush = nullptr;
	}
}

//===========================================================================
void StatusOverayPanel::draw(Context* ctx)
{
	//-----------------------------------------------------------------------
	calculateFPS();


	//-----------------------------------------------------------------------
	D2D1::Matrix3x2F matrix;


	matrix = D2D1::Matrix3x2F::Identity();
	ctx->getD2dRenderTarget()->SetTransform(matrix);


	//-----------------------------------------------------------------------
	if (getVisible())
	{
		drawStatusOverayPanel(ctx);
	}
}

void StatusOverayPanel::drawStatusOverayPanel(Context* ctx)
{
	//-----------------------------------------------------------------------
	SYSTEMTIME st;


	GetLocalTime(&st);


	//-----------------------------------------------------------------------
	wchar_t timeText[256];


	swprintf_s(timeText, L"%02d:%02d:%02d.%03d",
		st.wHour,
		st.wMinute,
		st.wSecond,
		st.wMilliseconds
	);


	//-----------------------------------------------------------------------
	wchar_t fpsText[256];


	swprintf_s(fpsText, L"%03d FPS",
		static_cast<int>(_FPS)
	);


	//-----------------------------------------------------------------------
	wchar_t frameText[256];


	swprintf_s(frameText, L"프레임: %03d",
		static_cast<int>(_FrameDrawCount)
	);


	//-----------------------------------------------------------------------
	double scale;


	_Viewport->getScale(scale);


	wchar_t scaleText[256];

	
	swprintf_s(scaleText, L"배율: %.2f",
		scale
	);


	//-----------------------------------------------------------------------
	double _Document_Mouse_X;
	double _Document_Mouse_Y;
	std::int64_t _Window_Mouse_X;
	std::int64_t _Window_Mouse_Y;

	_Viewport->WindowToDocument(_Mouse_X, _Mouse_Y, _Document_Mouse_X, _Document_Mouse_Y);
	_Viewport->DocumentToWindow(_Document_Mouse_X, _Document_Mouse_Y, _Window_Mouse_X, _Window_Mouse_Y);


	wchar_t mouseText[256];


#if defined(_DEBUG)
	swprintf_s(mouseText, L"마우스: (%d,%d)->(%.0f,%.0f)->(%d,%d)",
		static_cast<int>(_Mouse_X),
		static_cast<int>(_Mouse_Y),
		_Document_Mouse_X,
		_Document_Mouse_Y,
		static_cast<int>(_Window_Mouse_X),
		static_cast<int>(_Window_Mouse_Y)
		);
#else
	swprintf_s(mouseText, L"마우스: (%d,%d)->(%.0f,%.0f)",
		static_cast<int>(_Mouse_X),
		static_cast<int>(_Mouse_Y),
		_Document_Mouse_X,
		_Document_Mouse_Y
	);
#endif


	//-----------------------------------------------------------------------
	std::wstring text;


	text  = timeText ; text += L"\n";
	text += fpsText  ; text += L"\n";
	text += frameText; text += L"\n";
	text += scaleText; text += L"\n";
	text += mouseText; text += L"\n";


	//-----------------------------------------------------------------------
	D2D1_SIZE_F size = ctx->getD2dRenderTarget()->GetSize();

	float space  = 20.0f;
	float box_cx = 320;
	float box_cy = 80;


	float cx;
	float cy;
		
	cx = box_cx + space * 2;
	cy = box_cy + space * 2;


	D2D1_RECT_F rect;


	rect.left   = (size.width  > cx) ? size.width  - cx + space : space;
	rect.right  = rect.left + box_cx;
//	rect.top    = (size.height > cy) ? size.height - cy + space : space;
	rect.top    = space;
	rect.bottom = rect.top  + box_cy;

#if 0
	ctx->getD2dRenderTarget()->FillRectangle(&rect, _pStatusBoxFillBrush);
	ctx->getD2dRenderTarget()->DrawRectangle(&rect, _pStatusBoxLineBrush);
#else
	D2D1_ROUNDED_RECT rrect;
	

	rrect.rect.left   = rect.left  ;
	rrect.rect.top    = rect.top   ;
	rrect.rect.right  = rect.right ;
	rrect.rect.bottom = rect.bottom;
	rrect.radiusX = 5.0f;
	rrect.radiusY = 5.0f;
	ctx->getD2dRenderTarget()->FillRoundedRectangle(&rrect, _pStatusBox_FillBrush);
	ctx->getD2dRenderTarget()->DrawRoundedRectangle(&rrect, _pStatusBox_LineBrush);
#endif

	rect.left   += 5.0f;
	rect.top    += 5.0f;
	rect.right  -= 5.0f;
	rect.bottom -= 5.0f;
	ctx->getD2dRenderTarget()->DrawTextW(
		text.c_str(),
		static_cast<UINT32>(text.length()),
		_pStatus_TextFormat,
		rect,
		_pStatus_TextBrush
	);
}

//===========================================================================
void StatusOverayPanel::calculateFPS(void)
{
	_FrameDrawCount++;


	auto currentDrawTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> elapsed = currentDrawTime - _LastDrawTime;


	if (elapsed.count() >= 1.0f)
	{
		_FPS = _FrameDrawCount / elapsed.count();
		_FrameDrawCount = 0;
		_LastDrawTime = currentDrawTime;
	}
}

void StatusOverayPanel::setMousePosition(std::int64_t mouse_x, std::int64_t mouse_y)
{
	_Mouse_X = mouse_x;
	_Mouse_Y = mouse_y;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




