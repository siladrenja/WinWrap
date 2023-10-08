#pragma once
#include <Windows.h>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <thread>

#include "dependencies/SilaMath/SilaMath.hpp"

namespace Win {
	struct wParam;
	struct lParam;
	namespace {
		template<class T>
		LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wPar, LPARAM lPar)
		{
			switch (msg) {
				case WM_CREATE:
					break;
				case WM_PAINT:
					T::windowHandles.at(hwnd)->onPaint(lPar, wPar);
					break;
				case WM_SIZE:
					T::windowHandles.at(hwnd)->onResize(wPar,lPar);
					break;
				case WM_DESTROY:
					PostQuitMessage(0);
					T::windowHandles.at(hwnd)->onDestroy();
					break;
				case WM_CLOSE:
					DestroyWindow(hwnd);
					break;
				case WM_QUIT:
					
					break;
				case WM_KEYDOWN:
					T::windowHandles.at(hwnd)->onKeyDown(std::bit_cast<lParam>(wPar), std::bit_cast<wParam>(wPar));
					break;
				case WM_KEYUP:
					T::windowHandles.at(hwnd)->onKeyUp(lPar, wPar);
					break;
				default:
					return DefWindowProc(hwnd, msg, wPar, lPar);
			}
		}

	}
	

	enum class ClassStyle {
		defaultStyle = CS_HREDRAW | CS_VREDRAW,
		redrawOnResize = CS_HREDRAW | CS_VREDRAW,
		redrawOnHeight = CS_HREDRAW,
		redrawOnWidth = CS_VREDRAW,
		enableDoubleClick = CS_DBLCLKS,
		ownDeviceContext = CS_OWNDC,
		parentDeviceContext = CS_PARENTDC,
		noClose = CS_NOCLOSE,
		saveAsBitmap = CS_SAVEBITS,
		byteAlignClient = CS_BYTEALIGNCLIENT,
		byteAlignWindow = CS_BYTEALIGNWINDOW,

		globalClass = CS_GLOBALCLASS,
		dropShadow = CS_DROPSHADOW,
		oneDCperClass = CS_CLASSDC
	};
	enum class WindowStyle {
		defaultStyle = WS_OVERLAPPEDWINDOW,			//creates an overlapped window
		overlappedWindow = WS_OVERLAPPEDWINDOW,		//creates an overlapped window
		overlapped = WS_OVERLAPPED,					//creates an overlapped window
		poput = WS_POPUP,							//creates a pop-up window	
		child = WS_CHILD,							//creates a child window	
		visible = WS_VISIBLE,						//
		disabled = WS_DISABLED,						//
		minimize = WS_MINIMIZE,						//
		maximize = WS_MAXIMIZE,						//
		tabstop = WS_TABSTOP,						//
		border = WS_BORDER,							//
		caption = WS_CAPTION,						//
		systemMenu = WS_SYSMENU,					//
		thickframe = WS_THICKFRAME,					//
		minimizeBox = WS_MINIMIZEBOX,				//
		maximizeBox = WS_MAXIMIZEBOX,				//
		firstInControlGroup = WS_GROUP,				//
		clipChildren = WS_CLIPCHILDREN,				//
		clipSiblings = WS_CLIPSIBLINGS,				//
		verticalScroll = WS_VSCROLL,				//
		horizontalScroll = WS_HSCROLL,				//
		sizingBorder = WS_SIZEBOX					//
	};
	enum class ExtendedWindowStyle {
		none = 0,
		defaultExtendedStyle = WS_EX_CLIENTEDGE,
		acceptFiles = WS_EX_ACCEPTFILES,
		appWindowOnTaskbar = WS_EX_APPWINDOW,
		border = WS_EX_CLIENTEDGE,
		composited = WS_EX_COMPOSITED,
		contextHelp = WS_EX_CONTEXTHELP,
		controlParent = WS_EX_CONTROLPARENT,
		doubleBorder = WS_EX_DLGMODALFRAME,
		rightToLeftLayout = WS_EX_LAYOUTRTL,
		leftAllign = WS_EX_LEFT,
		scrollOnLeft = WS_EX_LEFTSCROLLBAR,
		MDIChild = WS_EX_MDICHILD,
		noActivate = WS_EX_NOACTIVATE,
		noInheritLayout = WS_EX_NOINHERITLAYOUT,
		noParentNotify = WS_EX_NOPARENTNOTIFY,
		rightAllign = WS_EX_RIGHT,
		rightScrollbar = WS_EX_RIGHTSCROLLBAR,
		rightToLeftReading = WS_EX_RTLREADING,
		staticBorder = WS_EX_STATICEDGE,
		toolWindow = WS_EX_TOOLWINDOW,
		topMost = WS_EX_TOPMOST,
		transparent = WS_EX_TRANSPARENT,
		thinWindowEdge = WS_EX_WINDOWEDGE



	};
	WindowStyle operator |(const WindowStyle& lSide, const WindowStyle& rSide) {
		return (WindowStyle)((int)lSide | (int)rSide);
	}
	ClassStyle operator |(const ClassStyle& lSide, const ClassStyle& rSide) {
		return (ClassStyle)((int)lSide | (int)rSide);
	}
	ExtendedWindowStyle operator |(const ExtendedWindowStyle& lSide, const ExtendedWindowStyle& rSide) {
		return (ExtendedWindowStyle)((int)lSide | (int)rSide);
	}


	struct wParam {
		WPARAM low : (sizeof(WPARAM) / 2);
		WPARAM high : (sizeof(WPARAM) / 2);

		wParam(WPARAM wparam) : high(HIWORD(wparam)), low(LOWORD(wparam)) {}

		operator WPARAM() {
			return std::bit_cast<WPARAM>(*this);
		}
	};

	struct lParam {
		LPARAM high : (sizeof(LPARAM) / 2);
		LPARAM low : (sizeof(LPARAM) / 2);

		lParam(LPARAM lparam) : high(HIWORD(lparam)), low(LOWORD(lparam)) {}

		operator LPARAM() {
			return std::bit_cast<LPARAM>(*this);
		}
	};

	using WS = WindowStyle;
	using WSEX = ExtendedWindowStyle;


	class DefaultWindowCallbackClass {
		
	};


	template<class CallbackClass>
	class Window;

	class WindowClass {
	public:
		WindowClass(const ClassStyle& classStyle = ClassStyle::defaultStyle) : instance(GetModuleHandle(0)) {
			

			wc.cbSize = sizeof(WNDCLASSEX);
			wc.style = (UINT)classStyle;
			
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = GetModuleHandle(0);
			wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wc.lpszMenuName = NULL;
			wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

			
		}
		~WindowClass() {
		}

		WindowClass(const WindowClass& copyFrom) : wndClassName(copyFrom.wndClassName), wc(copyFrom.wc), instance(copyFrom.instance) {
			
		}

		const std::wstring& GetName() const noexcept {
			return std::wstring(wndClassName);
		}

		const wchar_t* GetNameCstr() const noexcept {
			return wndClassName;
		}
		HINSTANCE GetInstance() const noexcept {
			return instance;
		}

		template<class CallbackClass>
		friend class Window;

	protected:
		wchar_t* wndClassName;
		WNDCLASSEX wc = { 0 };
		const HINSTANCE instance ;
	};

	template<class CallbackClass>
	class Window {
	public:
		Window(WindowClass& winClass, const std::wstring& windowTitle, unsigned int Width = 240, unsigned int Height = 120, const WS& winStyle = WS::defaultStyle, const WSEX& exWinStyle = WSEX::defaultExtendedStyle) {
			std::wstring className = L"WindowClass_" + std::to_wstring((size_t)this);
			winClass.wndClassName = (wchar_t*)malloc(sizeof(wchar_t) * (className.size() + 1));
			if (!winClass.wndClassName)
				throw std::exception("Failed to allocate memory for window class name");

			std::copy(className.begin(), className.end(), winClass.wndClassName);
			winClass.wndClassName[className.size()] = '\0';
			winClass.wc.lpszClassName = winClass.wndClassName;

			winClass.wc.lpfnWndProc = WndProc<Window<CallbackClass>>;


			if (!RegisterClassEx(&winClass.wc))
				throw std::exception("Failed to register window class");
			hwnd = CreateWindowEx(
				(UINT) exWinStyle,
				winClass.GetNameCstr(),
				windowTitle.c_str(),
				(UINT) winStyle,
				CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
				NULL, NULL, winClass.GetInstance(), NULL);

			windowHandles.insert(std::make_pair(hwnd, this));
			free(winClass.wndClassName);

		}

		~Window() {
			DestroyWindow(hwnd);
		}

		void Show() {
			ShowWindow(hwnd, SW_SHOW);
			UpdateWindow(hwnd);
		}

		void Hide() {
			ShowWindow(hwnd, SW_HIDE);
		}

		void MessageLoop() {
			MSG msg;
			while (true) {
				Update();
				if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}

		void MessageLoopAsync() {
			std::thread t(&Window::MessageLoop, this);
			t.detach();
		}

		friend LRESULT CALLBACK Win::WndProc<Window<CallbackClass>>(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		constexpr static bool event_has_onPaint = requires (CallbackClass e, lParam l, wParam w)
		{
			e.onPaint(l, w);
		};
		constexpr static bool event_has_Update = requires (CallbackClass e)
		{
			e.Update();
		};
		constexpr static bool event_has_onKeyDown = requires (CallbackClass e, lParam l, wParam w)
		{
			e.onKeyDown(l, w);
		};
		constexpr static bool event_has_onKeyUp = requires (CallbackClass e, lParam l, wParam w)
		{
			e.onKeyUp(l, w);
		};
		constexpr static bool event_has_onDestroy = requires (CallbackClass e)
		{
			e.onDestroy();
		};
		constexpr static bool event_has_onResize = requires (CallbackClass e, lParam l, wParam w)
		{
			e.onResize(l, w);
		};
		constexpr static bool event_has_onCreate = requires (CallbackClass e, HWND hwnd, const SMath::Rect<int>&rect, CREATESTRUCTA createStruct)
		{
			e.onCreate(hwnd, rect, createStruct);
		};
		
	protected:
		HWND hwnd;
		CallbackClass e;

		static std::unordered_map<HWND, Window<CallbackClass>*> windowHandles;

		//onPaint(lParam,wParam)
		void onPaint(lParam l, wParam w)
			requires event_has_onPaint
		{
			return e.onPaint(l, w);
		}
		void onPaint(lParam l, wParam w)
			requires (not event_has_onPaint)
		{
			DefWindowProc(hwnd, WM_PAINT, w, l);
		}

		//Update()
		void Update()
			requires event_has_Update
		{
			return e.Update();
		}
		void Update()
			requires (not event_has_Update)
		{
			// your default implementation
		}

		//onKeyDown(lParam,wParam)
		void onKeyDown(lParam l, wParam w)
			requires event_has_onKeyDown
		{
			return e.onKeyDown(l, w);
		}
		void onKeyDown(lParam l, wParam w)
			requires (not event_has_onKeyDown)
		{
			DefWindowProc(hwnd, WM_KEYDOWN, w, l);
		}

		//onKeyUp(lParam,wParam)
		void onKeyUp(lParam l, wParam w)
			requires event_has_onKeyUp
		{
			return e.onKeyUp(l, w);
		}
		void onKeyUp(lParam l, wParam w)
			requires (not event_has_onKeyUp)
		{
			DefWindowProc(hwnd, WM_KEYUP, w, l);
		}

		//onDestroy(lParam,wParam)
		void onDestroy()
			requires event_has_onDestroy
		{
			return e.onDestroy();
		}
		void onDestroy()
			requires (not event_has_onDestroy)
		{
			DefWindowProc(hwnd, WM_DESTROY, 0, 0);
		}
		
		//onResize(lParam,wParam)
		void onResize(lParam l, wParam w)
			requires event_has_onResize
		{
			return e.onResize(l, w);
		}
		void onResize(lParam l, wParam w)
			requires (not event_has_onResize)
		{
			DefWindowProc(hwnd, WM_SIZE, w, l);
		}

		//onCreate(HWND, lParam, wParam)
		void onCreate(HWND hwnd, const SMath::Rect<int>& rect, CREATESTRUCTA createStruct)
			requires event_has_onCreate
		{
			return e.onCreate(hwnd, rect, createStruct);
		}
		void onCreate((HWND hwnd, const SMath::Rect<int>& rect, CREATESTRUCTA createStruct)
			requires (not event_has_onCreate)
		{
			//DefWindowProc(hwnd, WM_CREATE, w, l);
		}
	};

	template <typename CallbackClass>
	std::unordered_map<HWND, Window<CallbackClass>*> Window<CallbackClass>::windowHandles;


}