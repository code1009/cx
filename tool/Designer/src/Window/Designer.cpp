/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include <cx/d2d/cx-d2d.hpp>
#include <cx/d2dw/cx-Widget.hpp>

//===========================================================================
#include "../../res/resource.h"

//===========================================================================
#include "../WindowHandler/WindowHandler.hpp"

//===========================================================================
#include "Catalog.hpp"
#include "Designer.hpp"
#include "View.hpp"
#include "ViewPropertyBox.hpp"

#include "FontFamilies.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static std::wstring std_uint8_t_vector_to_std_wstring(std::vector<std::uint8_t> const& dataBuffer)
{
	std::wstring result;

	if (dataBuffer.size() < 2 || dataBuffer.size() % 2 != 0)
	{
		return result;
	}

	result.resize(dataBuffer.size() / 2);

	for (std::size_t i = 0; i < result.size(); ++i)
	{
		std::size_t byteIndex = i * 2;

		wchar_t ch = 
			static_cast<wchar_t>(dataBuffer[byteIndex]) |
			(static_cast<wchar_t>(dataBuffer[byteIndex + 1]) << 8);

		result[i] = ch;
	}

	return result;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Designer::Designer(HWND hwnd) :
	_Hwnd(hwnd)
{
	//-----------------------------------------------------------------------
	setupCanvasView();
	setupMouseHandlerHandler();
	setupScrollHandler();
	setupDropTargetHandler();


	//-----------------------------------------------------------------------
	loadCatalog();
	loadFontFamilies();
}

Designer::~Designer()
{
	_Edit->viewPropertiesChangedEventListener.detach(
		reinterpret_cast<std::uintptr_t>(this)
	);
	_Edit->modifiedEventListener.detach(
		reinterpret_cast<std::uintptr_t>(this)
	);
	_Edit->invalidatedEventListener.detach(
		reinterpret_cast<std::uintptr_t>(this)
	);
	_Edit->itemSelectionChangedEventListener.detach(
		reinterpret_cast<std::uintptr_t>(this)
	);
	_PropertiesManipulator->propertyChangedEventListener.detach(
		reinterpret_cast<std::uintptr_t>(this)
	);
}

//===========================================================================
void Designer::setupCanvasView(void)
{
	//-----------------------------------------------------------------------
	_Edit = std::make_unique<cx::Widget::Edit>(cx::Widget::DefaultViewWidth, cx::Widget::DefaultViewHeight);


	//-----------------------------------------------------------------------
	cx::d2d::Factory factory;
	_Canvas = std::make_unique<cx::d2d::Canvas>(&factory, _Hwnd);
	_Canvas->drawingHandler =
		[this](cx::d2d::DrawingSession* drawingSession)
		{
#if 0
			CX_RUNTIME_LOG(cxLInfo) << L"drawing";
#endif
			drawingSession->Clear(cx::d2d::Colors::AliceBlue());
			_Edit->draw(*drawingSession);
		}
	;


	//-----------------------------------------------------------------------
	_Edit->viewPropertiesChangedEventListener.attach(
		reinterpret_cast<std::uintptr_t>(this),
		[this](cx::ev::Event& /*event*/)
		{
			bool rv = _Edit->viewContext().update();
			if (!rv)
			{
				CX_RUNTIME_LOG(cxLTrace) << L"not changed.";
			}

			updateScrollBar();
			invalidate();
		}
	);

	_Edit->modifiedEventListener.attach(
		reinterpret_cast<std::uintptr_t>(this),
		[this](cx::ev::Event& /*event*/)
		{
			if (modifiedHandler)
			{
				modifiedHandler();
			}
		}
	);

	_Edit->invalidatedEventListener.attach(
		reinterpret_cast<std::uintptr_t>(this),
		[this](cx::ev::Event& /*event*/)
		{
			invalidate();
		}
	);

	_Edit->itemSelectionChangedEventListener.attach(
		reinterpret_cast<std::uintptr_t>(this),
		[this](cx::ev::Event& /*event*/)
		{
			auto selectedItems = _Edit->getSelectedItems();
			std::size_t count = selectedItems.size();
			if (1 == count)
			{
				auto selectedItem = selectedItems.at(0);
				showItemProperty(selectedItem);
			}
			else
			{
				showItemProperty(nullptr);
			}
		}
	);


	//-----------------------------------------------------------------------
	_PropertiesManipulator = std::make_unique<cx::Widget::PropertiesManipulator>(*_Edit);
	_PropertiesManipulator->propertyChangedEventListener.attach(
		reinterpret_cast<std::uintptr_t>(this),
		[this](cx::ev::Event& /*event*/)
		{
			invalidate();
		}
	);
}

void Designer::setupMouseHandlerHandler(void)
{
	//-----------------------------------------------------------------------
	_MouseHandler = std::make_unique<cx::wui::WindowMouseHandler>(_Hwnd);


	//-----------------------------------------------------------------------
	_MouseHandler->mouseWheelHandler =
		[this](cx::wui::WindowMessage& windowMessage) -> bool
		{
			cx::wui::WM_MOUSEWHEEL_WindowMessageCrack wm{ windowMessage };

			POINT pt = wm.pt();
			short delta = wm.zDelta();
			UINT flag = wm.nFlags();

			bool controlKeyPressed = (flag & MK_CONTROL) != 0;
			//bool shiftKeyPressed = (flag & MK_SHIFT) != 0;

			//bool lButtonPressed = (flag & MK_LBUTTON) != 0;
			//bool rButtonPressed = (flag & MK_RBUTTON) != 0;
			//bool mButtonPressed = (flag & MK_MBUTTON) != 0;
			//bool x1ButtonPressed = (flag & MK_XBUTTON1) != 0;
			//bool x2ButtonPressed = (flag & MK_XBUTTON2) != 0;

			if (controlKeyPressed)
			{
				if (delta > 0)
				{
					zoomIn();
				}
				else
				{
					zoomOut();
				}
			}
			else
			{
				if (delta > 0)
				{
					_ScrollHandler->YScroll_LineUp();
				}
				else
				{
					_ScrollHandler->YScroll_LineDown();
				}
			}
			return true;
		}
	;
	_MouseHandler->mouseMoveHandler =
		[this](cx::wui::WindowMessage& windowMessage) -> bool
		{
			cx::wui::WM_MOUSEMOVE_WindowMessageCrack wm{ windowMessage };

			POINT pt = wm.point();
			UINT flag = wm.nFlags();
			bool controlKeyPressed = (flag & MK_CONTROL) != 0;
			bool shiftKeyPressed = (flag & MK_SHIFT) != 0;

			_Edit->eventGenerator().pointerMoved(
				static_cast<float>(pt.x),
				static_cast<float>(pt.y),
				controlKeyPressed,
				shiftKeyPressed
			);

			if (_Edit->eventGenerator().isPointerCaptured())
			{
				invalidate();
			}

			return true;
		}
	;
	_MouseHandler->mouseLButtonDownHandler =
		[this](cx::wui::WindowMessage& windowMessage) -> bool
		{
			cx::wui::WM_MOUSEMOVE_WindowMessageCrack wm{ windowMessage };

			POINT pt = wm.point();
			UINT flag = wm.nFlags();
			bool controlKeyPressed = (flag & MK_CONTROL) != 0;
			bool shiftKeyPressed = (flag & MK_SHIFT) != 0;

			_Edit->eventGenerator().pointerPressed(
				static_cast<float>(pt.x),
				static_cast<float>(pt.y),
				controlKeyPressed,
				shiftKeyPressed
			);

			_MouseHandler->setMouseCapture();
			invalidate();
			return true;
		}
	;
	_MouseHandler->mouseLButtonUpHandler =
		[this](cx::wui::WindowMessage& windowMessage) -> bool
		{
			cx::wui::WM_MOUSEMOVE_WindowMessageCrack wm{ windowMessage };

			POINT pt = wm.point();
			UINT flag = wm.nFlags();
			bool controlKeyPressed = (flag & MK_CONTROL) != 0;
			bool shiftKeyPressed = (flag & MK_SHIFT) != 0;

			_Edit->eventGenerator().pointerReleased(
				static_cast<float>(pt.x),
				static_cast<float>(pt.y),
				controlKeyPressed,
				shiftKeyPressed
			);

			_MouseHandler->releaseMouseCapture();
			invalidate();
			return true;
		}
	;
}

void Designer::setupScrollHandler(void)
{
	//-----------------------------------------------------------------------
	_ScrollHandler = std::make_unique<cx::wui::WindowScrollHandler>(_Hwnd);


	//-----------------------------------------------------------------------
	_ScrollHandler->scrollChangedHandler =
		[this](bool byScrollBar, std::int64_t x, std::int64_t y)
		{
			if (byScrollBar)
			{
				cx::Widget::Point scrollOffset;
				scrollOffset.X = static_cast<float>(x);
				scrollOffset.Y = static_cast<float>(y);
				_Edit->viewContext().setWindowScrollOffset(scrollOffset);

				invalidate();
			}
		}
	;
}

void Designer::setupDropTargetHandler(void)
{
	//-----------------------------------------------------------------------
	_DropTargetHandler = std::make_unique<cx::wui::dragdrop::WindowDropTargetHandler>(
		_Hwnd,
		cx::wui::dragdrop::getWindowDragDropClipboardFormat()->getClipboardFormat()
	);


	//-----------------------------------------------------------------------
	_DropTargetHandler->dragEnterHandler =
		[this](std::uint32_t seq, std::uint32_t flags, std::int32_t x, std::int32_t y, cx::wui::dragdrop::WindowDropTargetData const& data)
		{
			std::wstring name = std_uint8_t_vector_to_std_wstring(data._DataBuffer);

			CX_RUNTIME_LOG(cxLInfo)
				<< L"dragEnter : "
				<< L"seq=" << seq
				<< L" "
				<< L"x=" << x
				<< L" "
				<< L"y=" << y
				<< L" "
				<< L"name=" << name
				;

			auto item = _Edit->makeNewItemByFriendlyName(name);
			if (!item)
			{
				_Edit->setNewItem(nullptr);
				invalidate();
				return;
			}
			_Edit->dragOverNewItem(static_cast<float>(x), static_cast<float>(y));
		}
	;

	_DropTargetHandler->dragOverHandler =
		[this](std::uint32_t seq, std::uint32_t flags, std::int32_t x, std::int32_t y)
		{
			/*
			CX_RUNTIME_LOG(cxLInfo)
				<< L"dragOver : "
				<< L"seq=" << seq
				<< L" "
				<< L"x=" << x
				<< L" "
				<< L"y=" << y
				;
				*/

			_Edit->dragOverNewItem(static_cast<float>(x), static_cast<float>(y));
		}
	;
	_DropTargetHandler->dragLeaveHandler =
		[this](std::uint32_t seq)
		{
			CX_RUNTIME_LOG(cxLInfo)
				<< L"dragLeave : "
				<< L"seq=" << seq
				;

			_Edit->setNewItem(nullptr);
			invalidate();
		}
	;
	_DropTargetHandler->dropHandler =
		[this](std::uint32_t seq, std::uint32_t flags, std::int32_t x, std::int32_t y)
		{
			CX_RUNTIME_LOG(cxLInfo)
				<< L"drop : "
				<< L"seq=" << seq
				<< L" "
				<< L"x=" << x
				<< L" "
				<< L"y=" << y
				;

			_Edit->dropNewItem(static_cast<float>(x), static_cast<float>(y));
		}
	;

}

void Designer::loadCatalog(void)
{
	//-------------------------------------------------------------------
	_Catalog = std::make_unique<Catalog>(this);


	//-------------------------------------------------------------------
	using namespace cx::Widget;
	using namespace cx::Widget::Shape;


	//-------------------------------------------------------------------
	_Edit->factory().clear();


	//-------------------------------------------------------------------
	_Catalog->addLabel(L"도형");
	//-------------------------------------------------------------------
	_Catalog->addItem(std::make_shared<TextDesign         >(), makeProperties_TextDesign, L"글");
	_Catalog->addItem(std::make_shared<LineDesign         >(), makeProperties_LineDesign, L"선");
	//_Catalog->addItem(std::make_shared<ArrowDesign        >(), makeProperties_ArrowDesign, L"화살표");
	//_Catalog->addItem(std::make_shared<HalfArrowDesign    >(), makeProperties_HalfArrowDesign, L"반쪽화살표");
	_Catalog->addItem(std::make_shared<RectangleDesign    >(), makeProperties_RectangleDesign, L"사각형");
	_Catalog->addItem(std::make_shared<EllipseDesign      >(), makeProperties_BaseDesign, L"타원");


	//-------------------------------------------------------------------
}

void Designer::loadFontFamilies(void)
{
	_FontFamilies = getFontFamilies();
}

//===========================================================================
void Designer::resize(std::uint32_t cx, std::uint32_t cy)
{
	_Canvas->resize(cx, cy);


	bool rv;
	rv = _Edit->viewContext().setWindowSize(static_cast<float>(cx), static_cast<float>(cy));
	if (!rv)
	{
#if 0
		CX_RUNTIME_LOG(cxLTrace) << L"no changed.";
#endif
	}

	updateScrollBar();
	invalidate();
}

//===========================================================================
void Designer::invalidate(void)
{
	InvalidateRect(_Hwnd, nullptr, FALSE);
}

void Designer::updateScrollBar(void)
{
	//-------------------------------------------------------------------
	std::int64_t windowWidth = static_cast<std::int64_t>(_Edit->viewContext().windowWidth());
	std::int64_t windowHeight = static_cast<std::int64_t>(_Edit->viewContext().windowHeight());

	std::int64_t scaledWidth = static_cast<std::int64_t>(_Edit->viewContext().scaledWidth());
	std::int64_t scaledHeight = static_cast<std::int64_t>(_Edit->viewContext().scaledHeight());

	std::int64_t xScrollPos = static_cast<std::int64_t>(_Edit->viewContext().windowScrollOffset().X);
	std::int64_t yScrollPos = static_cast<std::int64_t>(_Edit->viewContext().windowScrollOffset().Y);

	//std::int64_t maxXScrollOffset = scaledWidth - windowWidth;
	//std::int64_t maxYScrollOffset = scaledHeight - windowHeight;

	_ScrollHandler->setXYScroll(
		scaledWidth, windowWidth, xScrollPos,
		scaledHeight, windowHeight, yScrollPos
	);
}

//===========================================================================
bool Designer::onWindowMessage(cx::wui::WindowMessage& windowMessage)
{
	bool handled;
	

	handled = _MouseHandler->onWindowMessage(windowMessage);
	if (handled)
	{
		return true;
	}

	handled = _ScrollHandler->onWindowMessage(windowMessage);
	if (handled)
	{
		return true;
	}

	return false;
}

//===========================================================================
void Designer::showItemProperty(std::shared_ptr<cx::Widget::Item> const& item)
{
	/*
	if (item == nullptr)
	{
		CX_RUNTIME_LOG(cxLTrace)
			<< L"showItemProperty: nullptr"
			;
	}
	else
	{
		CX_RUNTIME_LOG(cxLTrace)
			<< L"showItemProperty: "
			<< std::format(L"{}", item->className())
			;
	}
	*/

	_PropertiesManipulator->select(item);

	if (showItemPropertytHandler)
	{
		showItemPropertytHandler();
	}
}

//===========================================================================
void Designer::zoomIn(void)
{
	if (!_Edit->viewContext().zoomIn())
	{
		CX_RUNTIME_LOG(cxLTrace) << L"no changed.";
		return;
	}
	updateScrollBar();
	invalidate();
}

void Designer::zoomOut(void)
{
	if (!_Edit->viewContext().zoomOut())
	{
		CX_RUNTIME_LOG(cxLTrace) << L"no changed.";
		return;
	}
	updateScrollBar();
	invalidate();
}

//===========================================================================
void Designer::snapToGrid(bool enable)
{
	//_Edit->snapToGrid(enable);
	if (enable)
	{
		_Edit->setGridSize(5, 5);
	}
	else
	{
		_Edit->setGridSize(20, 20);
	}
}

void Designer::showGrid(bool show)
{
	_Edit->viewGrid().showGridLine(show);
	_Edit->viewGrid().showCenterLine(show);
	_Edit->viewGrid().showOutline(show);

	invalidate();
}

void Designer::showGridCoord(bool show)
{
	_Edit->viewGrid().showCoordinate(show);
	invalidate();
}

void Designer::showStatus(bool show)
{
	_Edit->viewStatus().show(show);
	invalidate();
}

void Designer::bringToFront(void)
{
	_Edit->bringToFront();
}

void Designer::sendToBack(void)
{
	_Edit->sendToBack();
}

void Designer::bringToTop(void)
{
	_Edit->bringToTop();
}

void Designer::sendToBottom(void)
{
	_Edit->sendToBottom();
}

void Designer::undo(void)
{
	_Edit->undo();
}

void Designer::redo(void)
{
	_Edit->redo();
}

void Designer::cut(void)
{
	_Edit->cut();
}

void Designer::copy(void)
{
	_Edit->copy();
}

void Designer::paste(void)
{
	_Edit->paste();
}

void Designer::selectAll(void)
{
	_Edit->selectAllItem();
}

void Designer::deselectAll(void)
{
	_Edit->deselectItem();
}

void Designer::erase(void)
{
	_Edit->removeSelectedItem();
}

//===========================================================================
void Designer::setViewProperties(void)
{
	//-------------------------------------------------------------------
	using namespace cx::Widget;


	//-------------------------------------------------------------------
	auto cx = static_cast<std::uint32_t>(_Edit->viewContext().width());
	auto cy = static_cast<std::uint32_t>(_Edit->viewContext().height());


	//-------------------------------------------------------------------
	auto color = _Edit->viewBackground().fillStyle().fillColor();
	auto R8 = getColorR8(color);
	auto G8 = getColorG8(color);
	auto B8 = getColorB8(color);


	//-------------------------------------------------------------------
	ViewPropertyBox box;
	box._CX = cx;
	box._CY = cy;
	box._ColorR8 = R8;
	box._ColorG8 = G8;
	box._ColorB8 = B8;
	if (IDOK == box.doModal(_Hwnd))
	{
		Color boxColor = Color(255, box._ColorR8, box._ColorG8, box._ColorB8);
		if ( (box._CX == cx && box._CY == cy) && (boxColor == color) )
		{
			CX_RUNTIME_LOG(cxLTrace) << L"no changed.";
			return;
		}


		auto viewWidth = static_cast<Coord>(box._CX);
		auto viewHeight = static_cast<Coord>(box._CY);
		auto viewBackgroundFillColor = Color(255, box._ColorR8, box._ColorG8, box._ColorB8);
		ViewProperties viewProperties;
		viewProperties.width(viewWidth);
		viewProperties.height(viewHeight);
		viewProperties.backgroundFillColor(viewBackgroundFillColor);
		_Edit->setViewProperties(viewProperties);


		_Edit->viewContext().setWindowScrollOffset({ 0.0f, 0.0f });
		_Edit->viewContext().update();
		updateScrollBar();
		invalidate();
	}
}

//===========================================================================
void Designer::newFile(void)
{
	//-----------------------------------------------------------------------
	if (_Edit->editSeed().isModified())
	{
		int ret = MessageBoxW(
			_Hwnd,
			L"현재 편집중인 내용이 저장되지 않았습니다.\n새로운 파일을 만드시겠습니까?",
			L"새 파일",
			MB_YESNO | MB_ICONWARNING
		);
		if (ret != IDYES)
		{
			return;
		}
	}


	//-----------------------------------------------------------------------
	_FilePath.clear();
	_Edit->reset();


	//-----------------------------------------------------------------------
	_Edit->viewContext().setWindowScrollOffset({ 0.0f, 0.0f });
	_Edit->viewContext().update();
	updateScrollBar();
	invalidate();
}

void Designer::openFile(void)
{
	//-----------------------------------------------------------------------
	if (_Edit->editSeed().isModified())
	{
		int ret = MessageBoxW(
			_Hwnd,
			L"현재 편집중인 내용이 저장되지 않았습니다.\n파일을 저장하시겠습니까?",
			L"파일 열기",
			MB_YESNO | MB_ICONWARNING
		);
		if (ret == IDYES)
		{
			saveFile();
		}
	}


	//-----------------------------------------------------------------------
	bool rv;


	//-----------------------------------------------------------------------
	std::wstring openFilePath;
	cx::wui::OpenFileDialog openFileDialog(
		L"",
		L"",
		L"Design Files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0",
		L"xml",
		L".",
		L"파일 열기"
	);
	rv = openFileDialog.doModal(_Hwnd, openFilePath);
	if (!rv)
	{
		return;
	}


	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLInfo)
		<< L"open file: "
		<< openFilePath
		;


	//-----------------------------------------------------------------------
	if (!std::filesystem::exists(openFilePath))
	{
		return;
	}


	//-----------------------------------------------------------------------
	rv = _Edit->loadFile(openFilePath);
	if (!rv)
	{
		CX_RUNTIME_LOG(cxLError) << L"_Edit->loadFile() failed";
		std::wstring message = std::format(L"파일을 열 수 없습니다.\n대상 파일: {})", openFilePath);
		MessageBoxW(
			_Hwnd,
			message.c_str(),
			L"파일 열기 오류",
			MB_OK | MB_ICONERROR
		);
	}
	_FilePath = openFilePath;


	//-----------------------------------------------------------------------
	_Edit->viewContext().setWindowScrollOffset({ 0.0f, 0.0f });
	_Edit->viewContext().update();
	updateScrollBar();
	invalidate();
}

void Designer::saveFile(void)
{
	//-----------------------------------------------------------------------
	if (_FilePath.empty())
	{
		saveFileAs();
		return;
	}


	//-----------------------------------------------------------------------
	bool rv = _Edit->saveFile(_FilePath);
	if (!rv)
	{
		CX_RUNTIME_LOG(cxLError) << L"_Edit->saveFile() failed";
		std::wstring message = std::format(L"파일 저장을 실패 했습니다.\n대상 파일: {})", _FilePath);
		MessageBoxW(
			_Hwnd,
			message.c_str(),
			L"파일 열기 오류",
			MB_OK | MB_ICONERROR
		);
	}
}

void Designer::saveFileAs(void)
{
	//-----------------------------------------------------------------------
	bool rv;


	//-----------------------------------------------------------------------
	std::wstring saveFilePath;
	cx::wui::SaveFileDialog saveFileDialog(
		L"",
		L"",
		L"Design Files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0",
		L"xml",
		L".",
		L"파일 저장"
	);
	rv = saveFileDialog.doModal(_Hwnd, saveFilePath);
	if (!rv)
	{
		return;
	}

	
	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLInfo)
			<< L"save file: "
			<< saveFilePath
			;
	_FilePath = saveFilePath;


	//-----------------------------------------------------------------------
	rv = _Edit->saveFile(_FilePath);
	if (!rv)
	{
		CX_RUNTIME_LOG(cxLError) << L"_Edit->saveFile() failed";
		std::wstring message = std::format(L"파일 저장을 실패 했습니다.\n대상 파일: {})", _FilePath);
		MessageBoxW(
			_Hwnd,
			message.c_str(),
			L"파일 열기 오류",
			MB_OK | MB_ICONERROR
		);
	}
}
