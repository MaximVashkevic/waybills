#include <Windows.h>
#include "EntryPoint.h"

const wchar_t* const lpszClassName = L"MainWindowClass";

typedef struct _TMainWindow
{
	HWND hWnd;
	HWND hListView;
} TMainWindow, * PMainWindow;


LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	PMainWindow pSelf;
	if (uMsg == WM_CREATE) {
		pSelf = (PMainWindow)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(TMainWindow));
		pSelf->hWnd = hWnd;
		SetWindowLong(hWnd, 0 , (LONG) pSelf); 

		pSelf->hListView = CreateWindowEx();
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	pSelf = (PMainWindow)GetWindowLong(hWnd, 0);
	if (!pSelf)
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		HeapFree(GetProcessHeap(), 0, pSelf);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) 
{
	WNDCLASSEX wcex;
	HWND hMainWindow;
	MSG msg;

	memset(&wcex, 0, sizeof wcex);
	wcex.cbSize = sizeof wcex;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = lpszClassName;
	wcex.lpfnWndProc = WindowProc;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.cbWndExtra = 4;
	RegisterClassEx(&wcex);

	
	hMainWindow = CreateWindowEx(0, lpszClassName, L"My Table", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 400, 300, 0, 0, 0, NULL);


	while (GetMessage(&msg, 0, 0, 0))
	{
		DispatchMessage(&msg);
	}

	return 0;
}

void PrintError()
{
	DWORD dw;
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	dw = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, L"info", MB_OK);
}
