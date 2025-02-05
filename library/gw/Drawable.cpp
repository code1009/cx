/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "gw.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace gw
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
DocumentGrid::~DocumentGrid()
{
	destroyDeviceResources();
}

//===========================================================================
bool DocumentGrid::createDeviceResources(Window* w)
{
	//-----------------------------------------------------------------------
	HRESULT hr; 
	
	
	//-----------------------------------------------------------------------
	if (!_pDocumentGridLine0Brush)
	{
		hr = w->getDRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.9f, 0.9f, 0.9f),
			&_pDocumentGridLine0Brush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pDocumentGridLine1Brush)
	{
		hr = w->getDRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.5f, 0.5f, 0.5f),
			&_pDocumentGridLine1Brush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pDocumentGridLine2Brush)
	{
		hr = w->getDRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.5f, 0.5f, 1.0f),
			&_pDocumentGridLine2Brush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	//-----------------------------------------------------------------------
	if (!_pCoordTextBrush)
	{
		hr = w->getDRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.5f, 0.5f, 0.5f, 0.5f),
			&_pCoordTextBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pCoordTextFormat)
	{
		hr = w->getDWriteFactory()->CreateTextFormat(
			L"Arial",
			nullptr,
			DWRITE_FONT_WEIGHT_THIN,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_ULTRA_CONDENSED,
			9.0f,
			L"ko-kr",
			&_pCoordTextFormat
		);
		if (FAILED(hr))
		{
			return false;
		}
		_pCoordTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		_pCoordTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	}

	return true;
}

void DocumentGrid::destroyDeviceResources(void)
{
	//-----------------------------------------------------------------------
	if (_pDocumentGridLine0Brush)
	{
		_pDocumentGridLine0Brush->Release();
		_pDocumentGridLine0Brush = nullptr;
	}
	if (_pDocumentGridLine1Brush)
	{
		_pDocumentGridLine1Brush->Release();
		_pDocumentGridLine1Brush = nullptr;
	}
	if (_pDocumentGridLine2Brush)
	{
		_pDocumentGridLine2Brush->Release();
		_pDocumentGridLine2Brush = nullptr;
	}

	//-----------------------------------------------------------------------
	if (_pCoordTextBrush)
	{
		_pCoordTextBrush->Release();
		_pCoordTextBrush = nullptr;
	}
	if (_pCoordTextFormat)
	{
		_pCoordTextFormat->Release();
		_pCoordTextFormat = nullptr;
	}
}

//===========================================================================
void DocumentGrid::draw(Window* w)
{
	if (getVisible())
	{
		drawDocumentGrid(w);
	}
}

//===========================================================================
void DocumentGrid::drawDocumentGrid(Window* w)
{
	//-----------------------------------------------------------------------
	double document_cx;
	double document_cy;


	w->getViewport()->getDocumentSize(document_cx, document_cy);


	//-----------------------------------------------------------------------
	double viewport_x;
	double viewport_y;
	double viewport_cx;
	double viewport_cy;


	w->getViewport()->getDocumentViewportPoint(viewport_x, viewport_y);
	w->getViewport()->getDocumentViewportSize(viewport_cx, viewport_cy);


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
		w->getDRenderTarget()->DrawLine(
			D2D1::Point2F(ci, ob),
			D2D1::Point2F(ci, oe),
			_pDocumentGridLine0Brush,
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
		w->getDRenderTarget()->DrawLine(
			D2D1::Point2F(ob, ci),
			D2D1::Point2F(oe, ci),
			_pDocumentGridLine0Brush,
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
		w->getDRenderTarget()->DrawLine(
			D2D1::Point2F(ci, ob),
			D2D1::Point2F(ci, oe),
			_pDocumentGridLine1Brush,
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
		w->getDRenderTarget()->DrawLine(
			D2D1::Point2F(ob, ci),
			D2D1::Point2F(oe, ci),
			_pDocumentGridLine1Brush,
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
	w->getDRenderTarget()->DrawLine(
		D2D1::Point2F(ci, ob),
		D2D1::Point2F(ci, oe),
		_pDocumentGridLine2Brush,
		2.0f
	);

	cb = static_cast<float>(0.0f);
	ce = static_cast<float>(document_cy);
	ob = static_cast<float>(0.0f);
	oe = static_cast<float>(document_cx);
	ci = (ce - cb) / 2.0f;
	w->getDRenderTarget()->DrawLine(
		D2D1::Point2F(ob, ci),
		D2D1::Point2F(oe, ci),
		_pDocumentGridLine2Brush,
		2.0f
	);


	//-----------------------------------------------------------------------
	// Draw the outline of a rectangle.
	D2D1_RECT_F rect;


	rect.left   = static_cast<float>(0.0f);
	rect.top    = static_cast<float>(0.0f);
	rect.right  = static_cast<float>(document_cx);
	rect.bottom = static_cast<float>(document_cy);
	w->getDRenderTarget()->DrawRectangle(
		&rect,
		_pDocumentGridLine2Brush,
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

			w->getDRenderTarget()->DrawTextW(
				text,
				static_cast<UINT32>(lstrlen(text)),
				_pCoordTextFormat,
				rect,
				_pCoordTextBrush
			);
		}
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
StatusOverayPanel::StatusOverayPanel()
{
	_LastDrawTime = std::chrono::steady_clock::now();
}

//===========================================================================
StatusOverayPanel::~StatusOverayPanel()
{
	destroyDeviceResources();
}

//===========================================================================
bool StatusOverayPanel::createDeviceResources(Window* w)
{
	//-----------------------------------------------------------------------
	HRESULT hr; 
	
	
	//-----------------------------------------------------------------------
	if (!_pStatusBoxFillBrush)
	{
		hr = w->getDRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.75f, 0.75f, 0.75f, 0.25f),
			&_pStatusBoxFillBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pStatusBoxLineBrush)
	{
		hr = w->getDRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.5f, 0.5f, 0.5f, 0.5f),
			&_pStatusBoxLineBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	//-----------------------------------------------------------------------
	if (!_pStatusTextBrush)
	{
		hr = w->getDRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.5f, 0.5f, 0.5f, 1.0f),
			&_pStatusTextBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pStatusTextFormat)
	{
		hr = w->getDWriteFactory()->CreateTextFormat(
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
			&_pStatusTextFormat
		);
		if (FAILED(hr))
		{
			return false;
		}
		//_pStatusTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER); // hcenter
		//_pStatusTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER); // vcenter
	}

	return true;
}

void StatusOverayPanel::destroyDeviceResources(void)
{
	//-----------------------------------------------------------------------
	if (_pStatusBoxFillBrush)
	{
		_pStatusBoxFillBrush->Release();
		_pStatusBoxFillBrush = nullptr;
	}
	if (_pStatusBoxLineBrush)
	{
		_pStatusBoxLineBrush->Release();
		_pStatusBoxLineBrush = nullptr;
	}

	//-----------------------------------------------------------------------
	if (_pStatusTextBrush)
	{
		_pStatusTextBrush->Release();
		_pStatusTextBrush = nullptr;
	}
	if (_pStatusTextFormat)
	{
		_pStatusTextFormat->Release();
		_pStatusTextFormat = nullptr;
	}
}

//===========================================================================
void StatusOverayPanel::draw(Window* w)
{
	//-----------------------------------------------------------------------
	calculateFPS();


	//-----------------------------------------------------------------------
	D2D1::Matrix3x2F matrix;


	matrix = D2D1::Matrix3x2F::Identity();
	w->getDRenderTarget()->SetTransform(matrix);


	//-----------------------------------------------------------------------
	if (getVisible())
	{
		drawStatusOverayPanel(w);
	}
}

void StatusOverayPanel::drawStatusOverayPanel(Window* w)
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


	w->getViewport()->getScale(scale);


	wchar_t scaleText[256];

	
	swprintf_s(scaleText, L"배율: %.2f",
		scale
	);


	//-----------------------------------------------------------------------
	double _document_mouse_x;
	double _document_mouse_y;
	std::int64_t _window_mouse_x;
	std::int64_t _window_mouse_y;

	w->getViewport()->WindowToDocument(_MouseX, _MouseY, _document_mouse_x, _document_mouse_y);
	w->getViewport()->DocumentToWindow(_document_mouse_x, _document_mouse_y, _window_mouse_x, _window_mouse_y);


	wchar_t mouseText[256];


	swprintf_s(mouseText, L"마우스: (%d,%d)->(%.0f,%.0f)->(%d,%d)",
		static_cast<int>(_MouseX),
		static_cast<int>(_MouseY),
		_document_mouse_x,
		_document_mouse_y,
		static_cast<int>(_window_mouse_x),
		static_cast<int>(_window_mouse_y)
		);


	//-----------------------------------------------------------------------
	std::wstring text;


	text  = timeText ; text += L"\n";
	text += fpsText  ; text += L"\n";
	text += frameText; text += L"\n";
	text += scaleText; text += L"\n";
	text += mouseText; text += L"\n";


	//-----------------------------------------------------------------------
	D2D1_SIZE_F size = w->getDRenderTarget()->GetSize();

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
	w->getDRenderTarget()->FillRectangle(&rect, _pStatusBoxFillBrush);
	w->getDRenderTarget()->DrawRectangle(&rect, _pStatusBoxLineBrush);
#else
	D2D1_ROUNDED_RECT rrect;
	

	rrect.rect.left   = rect.left  ;
	rrect.rect.top    = rect.top   ;
	rrect.rect.right  = rect.right ;
	rrect.rect.bottom = rect.bottom;
	rrect.radiusX = 5.0f;
	rrect.radiusY = 5.0f;
	w->getDRenderTarget()->FillRoundedRectangle(&rrect, _pStatusBoxFillBrush);
	w->getDRenderTarget()->DrawRoundedRectangle(&rrect, _pStatusBoxLineBrush);
#endif

	rect.left   += 5.0f;
	rect.top    += 5.0f;
	rect.right  -= 5.0f;
	rect.bottom -= 5.0f;
	w->getDRenderTarget()->DrawTextW(
		text.c_str(),
		static_cast<UINT32>(text.length()),
		_pStatusTextFormat,
		rect,
		_pStatusTextBrush
	);
}

//===========================================================================
void StatusOverayPanel::calculateFPS(void)
{
	_FrameDrawCount++;


	auto current_draw_time = std::chrono::steady_clock::now();
	std::chrono::duration<float> elapsed = current_draw_time - _LastDrawTime;


	if (elapsed.count() >= 1.0f)
	{
		_FPS = _FrameDrawCount / elapsed.count();
		_FrameDrawCount = 0;
		_LastDrawTime = current_draw_time;
	}
}

void StatusOverayPanel::setMousePosition(std::int64_t mouse_x, std::int64_t mouse_y)
{
	_MouseX = mouse_x;
	_MouseY = mouse_y;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




