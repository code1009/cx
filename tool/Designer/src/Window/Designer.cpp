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

	_Edit->viewGrid().showGridLine(true);
	_Edit->viewGrid().showCenterLine(true);
	_Edit->viewGrid().showOutline(true);
	_Edit->viewGrid().showCoordinate(true);
	_Edit->viewStatus().show(true);


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
			updateScrollBar();
			invalidate();
		}
	);

	_Edit->modifiedEventListener.attach(
		reinterpret_cast<std::uintptr_t>(this),
		[this](cx::ev::Event& /*event*/)
		{
			//FileSaveButton().IsEnabled(_Edit->editSeed().isModified());
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
					zoomIn(
						static_cast<float>(pt.x),
						static_cast<float>(pt.y)
					);
				}
				else
				{
					zoomOut(
						static_cast<float>(pt.x),
						static_cast<float>(pt.y)
					);
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
	//	using namespace rs::Diagram;


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

	_Catalog->addSpare();
	_Catalog->addLabel(L"테스트");
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

	if (_ShowItemPropertytHandler)
	{
		_ShowItemPropertytHandler();
	}
}

//===========================================================================
void Designer::zoomIn(float px, float py)
{
	//-------------------------------------------------------------------
#if 0
	cx::Widget::Point window0{ px, py };
	cx::Widget::Point view0;
	view0 = _Edit->viewContext().fromWindow(window0);
	CX_RUNTIME_LOG(cxLTrace)
		<< L"zoomIn(): "
		<< std::format(L"window0=({:.2f}, {:.2f}) view0=({:.2f}, {:.2f})",
			window0.X, window0.Y,
			view0.X, view0.Y
		);
#endif


	//-------------------------------------------------------------------
	if (!_Edit->viewContext().zoomIn())
	{
		CX_RUNTIME_LOG(cxLTrace) << L"no changed.";
		return;
	}
	updateScrollBar();
	invalidate();
}

void Designer::zoomOut(float px, float py)
{
	//-------------------------------------------------------------------
#if 0
	cx::Widget::Point window0{ px, py };
	cx::Widget::Point view0;
	view0 = _Edit->viewContext().fromWindow(window0);
	CX_RUNTIME_LOG(cxLTrace)
		<< L"zoomOut(): "
		<< std::format(L"window0=({:.2f}, {:.2f}) view0=({:.2f}, {:.2f})",
			window0.X, window0.Y,
			view0.X, view0.Y
		);
#endif


	//-------------------------------------------------------------------
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
	_Edit->snapToGrid(enable);
}

void Designer::showGrid(bool show)
{
	_Edit->viewGrid().showGridLine(show);
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
	ViewPropertyBox box;

	box.doModal(_Hwnd);
}

//===========================================================================
void Designer::newFile(void)
{

}
void Designer::openFile(void)
{
	cx::wui::OpenFileDialog openFileDialog(
		L"",
		L"",
		L"Design Files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0",
		L"xml",
		L".",
		L"파일 열기"
	);

	std::wstring openFilePath;
	if (openFileDialog.doModal(_Hwnd, openFilePath))
	{
		CX_RUNTIME_LOG(cxLInfo)
			<< L"open file: "
			<< openFilePath
			;
		_FilePath = openFilePath;
	}
}
void Designer::saveFile(void)
{
	
}

void Designer::saveFileAs(void)
{
	cx::wui::SaveFileDialog openFileDialog(
		L"",
		L"",
		L"Design Files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0",
		L"xml",
		L".",
		L"파일 저장"
	);

	std::wstring openFilePath;
	if (openFileDialog.doModal(_Hwnd, openFilePath))
	{
		CX_RUNTIME_LOG(cxLInfo)
			<< L"open file: "
			<< openFilePath
			;
		_FilePath = openFilePath;
	}
}

//===========================================================================
void Designer::onFile_New              (void){ CX_RUNTIME_LOG(cxLInfo) << L"onFile_New              "; newFile(); }
void Designer::onFile_Open             (void){ CX_RUNTIME_LOG(cxLInfo) << L"onFile_Open             "; openFile(); }
void Designer::onFile_Save             (void){ CX_RUNTIME_LOG(cxLInfo) << L"onFile_Save             "; saveFile(); }
void Designer::onFile_SaveAs           (void){ CX_RUNTIME_LOG(cxLInfo) << L"onFile_SaveAs           "; saveFileAs(); }
void Designer::onEdit_Undo             (void){ CX_RUNTIME_LOG(cxLInfo) << L"onEdit_Undo             "; undo(); }
void Designer::onEdit_Redo             (void){ CX_RUNTIME_LOG(cxLInfo) << L"onEdit_Redo             "; redo(); }
void Designer::onEdit_Cut              (void){ CX_RUNTIME_LOG(cxLInfo) << L"onEdit_Cut              "; cut(); }
void Designer::onEdit_Copy             (void){ CX_RUNTIME_LOG(cxLInfo) << L"onEdit_Copy             "; copy(); }
void Designer::onEdit_Paste            (void){ CX_RUNTIME_LOG(cxLInfo) << L"onEdit_Paste            "; paste(); }
void Designer::onEdit_SelectAll        (void){ CX_RUNTIME_LOG(cxLInfo) << L"onEdit_SelectAll        "; selectAll(); }
void Designer::onEdit_DeselectAll      (void){ CX_RUNTIME_LOG(cxLInfo) << L"onEdit_DeselectAll      "; deselectAll(); }
void Designer::onEdit_Delete           (void){ CX_RUNTIME_LOG(cxLInfo) << L"onEdit_Delete           "; erase(); }
void Designer::onDesign_ViewCommand    (void){ CX_RUNTIME_LOG(cxLInfo) << L"onDesign_ViewCommand    "; }
void Designer::onDesign_ViewProperty   (void){ CX_RUNTIME_LOG(cxLInfo) << L"onDesign_ViewProperty   "; }
void Designer::onDesign_ZoomIn         (void){ CX_RUNTIME_LOG(cxLInfo) << L"onDesign_ZoomIn         "; zoomIn(0,0);  }
void Designer::onDesign_ZoomOut        (void){ CX_RUNTIME_LOG(cxLInfo) << L"onDesign_ZoomOut        "; zoomOut(0,0); }
void Designer::onDesign_BringToFront   (void){ CX_RUNTIME_LOG(cxLInfo) << L"onDesign_BringToFront   "; bringToFront();}
void Designer::onDesign_SendToBack     (void){ CX_RUNTIME_LOG(cxLInfo) << L"onDesign_SendToBack     "; sendToBack();}
void Designer::onDesign_BringToTop     (void){ CX_RUNTIME_LOG(cxLInfo) << L"onDesign_BringToTop     "; bringToTop();}
void Designer::onDesign_SendToBottom   (void){ CX_RUNTIME_LOG(cxLInfo) << L"onDesign_SendToBottom   "; sendToBottom();}
void Designer::onDesign_FileProperties(void) { CX_RUNTIME_LOG(cxLInfo) << L"onDesign_FileProperties "; setViewProperties(); }
void Designer::onDesign_SnapToGrid     (void){ CX_RUNTIME_LOG(cxLInfo) << L"onDesign_SnapToGrid     "; static bool v=true; v=!v; snapToGrid   (v); }
void Designer::onDesign_ShowGrid       (void){ CX_RUNTIME_LOG(cxLInfo) << L"onDesign_ShowGrid       "; static bool v=true; v=!v; showGrid     (v); }
void Designer::onDesign_ShowGridCoord  (void){ CX_RUNTIME_LOG(cxLInfo) << L"onDesign_ShowGridCoord  "; static bool v=true; v=!v; showGridCoord(v); }
void Designer::onDesign_ShowStatus     (void){ CX_RUNTIME_LOG(cxLInfo) << L"onDesign_ShowStatus     "; static bool v=true; v=!v; showStatus   (v); }

