#include <Windows.h>
#include <Richedit.h>
#include "Notepad.h"

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

HWND hEdit;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	WNDCLASSW wc = { 0 };
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"MainWndClass";
	wc.lpfnWndProc = MainWindowProc;

	if (!RegisterClassW(&wc))
		return -1;

	CreateWindowW(
		L"MainWndClass", L"Notepad",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		400, 200, 500, 250,
		NULL, NULL, NULL, NULL
	);

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

void loadlibs() {
	HINSTANCE relib = LoadLibrary("msftedit.dll");  //Make sure the dll is under C:\Windows\SysWow64 or in the path of your project .exe
	if (relib == NULL) {
		MessageBox(NULL, "Missing msftedit.dll", "Error", MB_ICONEXCLAMATION | MB_OK);
		PostQuitMessage(0);
	}
}

void Menubar(HWND hWnd) {
	HMENU MainMenu = CreateMenu();
	HMENU FileMenu = CreateMenu();
	HMENU EditMenu = CreateMenu();
	HMENU FormatMenu = CreateMenu();
	HMENU Format_ColorMenu = CreateMenu();
	HMENU Format_BackColorMenu = CreateMenu();
	HMENU HelpMenu = CreateMenu();

	AppendMenu(MainMenu, MF_POPUP, (UINT_PTR)FileMenu, "File");
		AppendMenu(FileMenu, MF_STRING, MB_NEW, "New");
		AppendMenu(FileMenu, MF_STRING, MB_OPEN, "Open");
		AppendMenu(FileMenu, MF_SEPARATOR, NULL, NULL);
		AppendMenu(FileMenu, MF_STRING, MB_SAVE, "Save");
		AppendMenu(FileMenu, MF_STRING, MB_SAVEAS, "Save as");
		AppendMenu(FileMenu, MF_SEPARATOR, NULL, NULL);
		AppendMenu(FileMenu, MF_STRING, MB_CLOSE, "Close");
		AppendMenu(FileMenu, MF_SEPARATOR, NULL, NULL);
		AppendMenu(FileMenu, MF_STRING, MB_EXIT, "Exit");

	AppendMenu(MainMenu, MF_POPUP, (UINT_PTR)EditMenu, "Edit");
		AppendMenu(EditMenu, MF_STRING, MB_UNDO, "Undo");
		AppendMenu(EditMenu, MF_STRING, MB_REDO, "Redo");
		AppendMenu(EditMenu, MF_SEPARATOR, NULL, NULL);
		AppendMenu(EditMenu, MF_STRING, MB_CUT, "Cut");
		AppendMenu(EditMenu, MF_STRING, MB_COPY, "Copy");
		AppendMenu(EditMenu, MF_STRING, MB_PASTE, "Paste");
		AppendMenu(EditMenu, MF_SEPARATOR, NULL, NULL);
		AppendMenu(EditMenu, MF_STRING, MB_SELALL, "Select all");

	AppendMenu(MainMenu, MF_POPUP, (UINT_PTR)FormatMenu, "Format");
		AppendMenu(FormatMenu, MF_POPUP, (UINT_PTR)Format_ColorMenu, "Color");
			AppendMenu(Format_ColorMenu, MF_STRING, MB_COLOR_RED, "Red");
			AppendMenu(Format_ColorMenu, MF_STRING, MB_COLOR_GREEN, "Green");
			AppendMenu(Format_ColorMenu, MF_STRING, MB_COLOR_BLUE, "Blue");
			AppendMenu(Format_ColorMenu, MF_STRING, MB_COLOR_BLACK, "Black");
		AppendMenu(FormatMenu, MF_POPUP, (UINT_PTR)Format_BackColorMenu, "Backcolor");
			AppendMenu(Format_BackColorMenu, MF_STRING, MB_BACK_RED, "Red");
			AppendMenu(Format_BackColorMenu, MF_STRING, MB_BACK_GREEN, "Green");
			AppendMenu(Format_BackColorMenu, MF_STRING, MB_BACK_BLUE, "Blue");
			AppendMenu(Format_BackColorMenu, MF_STRING, MB_BACK_WHITE, "White");
			AppendMenu(FormatMenu, MF_SEPARATOR, 0, 0);
		AppendMenu(FormatMenu, MF_STRING, MB_BOLD, "Bold");
		AppendMenu(FormatMenu, MF_STRING, MB_ITALIC, "Italic");
		AppendMenu(FormatMenu, MF_STRING, MB_UNDERLINED, "Underlined");
		AppendMenu(FormatMenu, MF_STRING, MB_RESETFORMATTINGS, "Clear formattings");

	SetMenu(hWnd, MainMenu);
}

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg)
	{
	case WM_CREATE: {
		loadlibs();
		Menubar(hWnd);

		hEdit = CreateWindow("RICHEDIT50W", "",
			WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_VSCROLL | WS_HSCROLL,
			0, 0, 0, 0, hWnd, NULL, NULL, NULL);

		break;
	}
	case WM_COMMAND: {
		switch (wp) {
		case MB_UNDO: {
			SendMessage(hEdit, EM_UNDO, 0, 0);
			break;
		}
		case MB_REDO: {
			SendMessage(hEdit, EM_REDO, 0, 0);
			break;
		}
		case MB_CUT: {
			SendMessage(hEdit, WM_CUT, 0, 0);
			break;
		}
		case MB_PASTE: {
			SendMessage(hEdit, WM_PASTE, 0, 0);
			break;
		}
		case MB_COPY: {
			SendMessage(hEdit, WM_COPY, 0, 0);
			break;
		}
		case MB_SELALL: {
			SendMessage(hEdit, EM_SETSEL, 0, -1);
			break;
		}
		}
		break;
	}
	case WM_CLOSE: {
		PostQuitMessage(0);
		break;
	}
	case WM_SIZE:{
		MoveWindow(hEdit, 0, 0, LOWORD(lp), HIWORD(lp), FALSE);
		break;
				   }

	}
	return DefWindowProcW(hWnd, msg, wp, lp);
}
