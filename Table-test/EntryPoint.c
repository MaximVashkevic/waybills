#include "EntryPoint.h"
#include "../sqlite-test/database.h"
#include "Table.h"
#include <CommCtrl.h>
#define IDC_BUTTON	(HMENU)100
#define IDC_EDIT	(HMENU)101
#define ID_TEXT 200;

#define DLG_X 10
#define DLG_Y 10
#define DLG_CX 100
#define DLG_CY 60

#define DLG_EDIT_X 10
#define DLG_EDIT_Y 10

#define DLG_EDIT_CX 80
#define DLG_EDIT_CY 15

#define DLG_BUTTON_Y 35
#define DLG_BUTTON_CY 15
#define DLG_BUTTON_CX 30

#define DLG_OK_X 10
#define DLG_CANCEL_X 60

ptable t;
const wchar_t* const lpszClassName = L"MainWindowClass";

typedef struct _TMainWindow
{
	HWND hWnd;
	HWND hBtnAddDriver;
	HWND hEdit;
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
		if (pSelf)
		{
			pSelf->hWnd = hWnd;
			pSelf->hBtnAddDriver = CreateWindowEx(0, L"BUTTON", L"Hide/Show", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				0, 0, 100, 20, hWnd, IDC_BUTTON, 0, NULL);
			pSelf->hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
				120, 0, 150, 40, hWnd, IDC_EDIT, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
			SetWindowLong(hWnd, 0, (LONG)pSelf);
		}
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
	case WM_PAINT:
		Paint(hWnd);
	case WM_COMMAND:
		if ((HMENU)LOWORD(wParam) == IDC_BUTTON && HIWORD(wParam) == BN_CLICKED)
		{
			/*if (IsWindowEnabled(pSelf->hEdit))
				EnableWindow(pSelf->hEdit, FALSE);
			else
				EnableWindow(pSelf->hEdit, TRUE);*/
			Disp((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), hWnd, L"sdlfkj");
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wcex;
	HWND hMainWindow;
	MSG msg;


	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2); // fix blurry displaying
	memset(&wcex, 0, sizeof wcex);
	wcex.cbSize = sizeof wcex;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = lpszClassName;
	wcex.lpfnWndProc = WindowProc;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.cbWndExtra = 4;
	RegisterClassEx(&wcex);


	hMainWindow = CreateWindowEx(0, lpszClassName, L"My Table", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 800, 600, 0, 0, 0, NULL);

	/*pconnection pc = openDB(OpenDialog(hMainWindow));
	pdriver d;
	int n = getDrivers(pc, &d);
	t = createTable(n, 2, 100, 100);
	for (int j = 0; j < n; j++)
	{
		setData(t, j, 1, d[j].name, tText);
	}
	closeDB(pc);
	InvalidateRect(hMainWindow, NULL, 1);*/
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	//freeDrivers(d, n);

	return 0;
}

LPWORD lpwAlign(LPWORD lpIn)
{
	ULONG ul;

	ul = (ULONG)lpIn;
	ul++;
	ul >>= 1;
	ul <<= 1;
	return (LPWORD)ul;
}

BOOL DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:

		case IDCANCEL:
			EndDialog(hwndDlg, wParam);
			return TRUE;
		}
	}
	return FALSE;
}

LRESULT Disp(HINSTANCE hInstance, HWND hWnd, LPWSTR lpszMessage)
{
	HGLOBAL hgbl;
	LPDLGTEMPLATE lpdt;
	LPDLGITEMTEMPLATE lpdit;
	LPWORD lpw;
	LPWSTR lpwsz;
	LRESULT ret;
	int nchar;

	hgbl = GlobalAlloc(GMEM_ZEROINIT, 1024);
	if (!hgbl)
		return -1;

	lpdt = (LPDLGTEMPLATE)GlobalLock(hgbl);

	// Define a dialog box.

	lpdt->style = WS_POPUP | WS_BORDER | WS_SYSMENU | DS_MODALFRAME | WS_CAPTION;
	lpdt->cdit = 3;         // Number of controls
	lpdt->x = DLG_X;  lpdt->y = DLG_Y;
	lpdt->cx = DLG_CX; lpdt->cy = DLG_CY;

	lpw = (LPWORD)(lpdt + 1);
	*lpw++ = 0;             // No menu
	*lpw++ = 0;             // Predefined dialog box class (by default)

	lpwsz = (LPWSTR)lpw;
	StringCchCopy(lpwsz, 100, L"Моё окно");
	StringCchLength(lpwsz, 100, &nchar);
	lpw += nchar + 1;

	//-----------------------
	// Define an OK button.
	//-----------------------
	lpw = lpwAlign(lpw);    // Align DLGITEMTEMPLATE on DWORD boundary
	lpdit = (LPDLGITEMTEMPLATE)lpw;
	lpdit->x = DLG_OK_X; lpdit->y = DLG_BUTTON_Y;
	lpdit->cx = DLG_BUTTON_CX; lpdit->cy = DLG_BUTTON_CY;
	lpdit->id = IDOK;       // OK button identifier
	lpdit->style = WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON;

	lpw = (LPWORD)(lpdit + 1);
	*lpw++ = 0xFFFF;
	*lpw++ = 0x0080;        // Button class

	lpwsz = (LPWSTR)lpw;
	nchar = 1 + MultiByteToWideChar(CP_ACP, 0, "OK", -1, lpwsz, 50);
	lpw += nchar;
	*lpw++ = 0;             // No creation data

	//-----------------------
	// Define a cancel button.
	//-----------------------
	lpw = lpwAlign(lpw);    // Align DLGITEMTEMPLATE on DWORD boundary
	lpdit = (LPDLGITEMTEMPLATE)lpw;
	lpdit->x = DLG_CANCEL_X; lpdit->y = DLG_BUTTON_Y;
	lpdit->cx = DLG_BUTTON_CX; lpdit->cy = DLG_BUTTON_CY;
	lpdit->id = IDCANCEL;    // cancel button identifier
	lpdit->style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;

	lpw = (LPWORD)(lpdit + 1);
	*lpw++ = 0xFFFF;
	*lpw++ = 0x0080;        // Button class atom

	lpwsz = (LPWSTR)lpw;
	StringCchCopy(lpwsz, 100, L"Отмена");
	StringCchLength(lpwsz, 100, &nchar);
	lpw += nchar + 2;
	*lpw++ = 0;             // No creation data


	//-----------------------
	// Define a static text control.
	//-----------------------
	lpw = lpwAlign(lpw);    // Align DLGITEMTEMPLATE on DWORD boundary
	lpdit = (LPDLGITEMTEMPLATE)lpw;
	lpdit->x = DLG_EDIT_X; lpdit->y = DLG_EDIT_Y;
	lpdit->cx = DLG_EDIT_CX; lpdit->cy = DLG_EDIT_CY;
	lpdit->id = ID_TEXT;    // Text identifier
	lpdit->style = WS_CHILD | WS_VISIBLE | SS_LEFT;

	lpw = (LPWORD)(lpdit + 1);
	*lpw++ = 0xFFFF;
	*lpw++ = 0x0081;        // Edit class

	for (lpwsz = (LPWSTR)lpw; *lpwsz++ = (WCHAR)*lpszMessage++;);
	lpw = (LPWORD)lpwsz;
	*lpw++ = 0;             // No creation data


	GlobalUnlock(hgbl);
	ret = DialogBoxIndirect(hInstance,
		(LPDLGTEMPLATE)hgbl,
		hWnd,
		(DLGPROC)DialogProc);
	GlobalFree(hgbl);
	return ret;
}


PWSTR OpenDialog(HWND hWnd)
{
#define bufsize 260
	OPENFILENAMEW ofn;
	PWSTR lpszFile;

	lpszFile = (PWSTR)malloc(bufsize * sizeof(WCHAR));
	if (lpszFile)
	{
		lpszFile[0] = '\0';

		memset(&ofn, 0, sizeof ofn);
		ofn.lStructSize = sizeof ofn;
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = lpszFile;
		ofn.nMaxFile = bufsize;
		ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";//last must be terminated by two \0's
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn) != TRUE)
		{
			free(lpszFile);
			lpszFile = NULL;
		}
	}
	return lpszFile;
}

void Paint(HWND hWnd)
{
	HDC hdc;
	RECT rc;
	HFONT hfnt, hfntPrev;
	LPWSTR lpszRotate = L"Hello";
	HRESULT hr;
	size_t* pcch = malloc(sizeof(size_t));
	PAINTSTRUCT ps;
	hdc = BeginPaint(hWnd, &ps);

	/*PLOGFONT plf = (PLOGFONT)LocalAlloc(LPTR, sizeof(LOGFONT));

	hr = StringCchCopy(plf->lfFaceName, 6, L"Arial");

	plf->lfWeight = FW_NORMAL;

	GetClientRect(hWnd, &rc);

	SetBkMode(hdc, TRANSPARENT);

	plf->lfEscapement = 2700;
	hfnt = CreateFontIndirect(plf);
	hfntPrev = SelectObject(hdc, hfnt);
	hr = StringCchLength(lpszRotate, 22, pcch);
	TextOut(hdc, rc.right, rc.top, lpszRotate, *pcch);
	SelectObject(hdc, hfntPrev);
	DeleteObject(hfnt);
	LocalFree((LOCALHANDLE)plf);*/
	WCHAR s[10];
	if (t)
	{
		for (int i = 0; i < t->rowCount; i++)
		{
			int x = 0;
			for (int j = 0; j < t->colCount; j++)
			{
				hr = StringCchLength(getData(t, i, j, tText), 30, pcch);
				TextOut(hdc, x, i * t->rowHeight, getData(t, i, j, tText), *pcch);
				x += t->colWidths[j];
			}
		}
	}
	EndPaint(hWnd, &ps);
	free(pcch);

}

void PrintError()
{
	DWORD dw;
	LPVOID lpMsgBuf;
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

