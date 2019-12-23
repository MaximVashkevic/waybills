#include "EntryPoint.h"
#include <CommCtrl.h>
#include <windowsx.h>
#include "resource.h"
#include <strsafe.h>

const wchar_t* const lpszClassName = L"MainWindowClass";

void ShowEdit(int x, int y, HWND hWnd)
{
	SetWindowPos(hWnd, HWND_TOP, x, y, 100, 20, SWP_SHOWWINDOW);
}

void Select(PMainWindow pSelf, PTable t, int x, int y)
{
	TPos pos;
	pos = getID(t, x, y);
	if (pos.i != INV_POS)
	{
		pSelf->selected = TRUE;
		pSelf->selection = pos;
	}
}

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
			pSelf->hBtnAdd = CreateWindowEx(0, L"BUTTON", L"Добавить", WS_CHILD | BS_PUSHBUTTON,
				0, 0, 100, 20, hWnd, IDC_BTN_ADD, 0, NULL);
			pSelf->hBtnDelete = CreateWindowEx(0, L"BUTTON", L"Удалить", WS_CHILD | BS_PUSHBUTTON,
				105, 0, 100, 20, hWnd, IDC_BTN_DELETE, 0, NULL);
			//pSelf->hComboBox = CreateWindowEx()
			pSelf->hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | ES_AUTOHSCROLL,
				120, 0, 150, 40, hWnd, IDC_EDIT, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
			pSelf->state = sEmpty;
			pSelf->pc = NULL;
			SetWindowLong(hWnd, 0, (LONG)pSelf);

			/*s afsldkkf asdkf jdsa
			!!!!!!!!!!!!!!
			*/
			pSelf->pc = openDB(L"D:\\projects\\course2\\3sem\\coursework\\sqlite-test\\t2.db");
			/*
			*/
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	pSelf = (PMainWindow)GetWindowLong(hWnd, 0);
	if (!pSelf)
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	switch (uMsg)
	{
	case WM_DESTROY:
		if (pSelf->pc)
		{
			closeDB(pSelf->pc);
		}
		PostQuitMessage(0);
		HeapFree(GetProcessHeap(), 0, pSelf);
		return 0;
	case WM_PAINT:
		Paint(hWnd);
		break;
	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED)
		{//(HMENU)LOWORD(wParam) == IDC_BTN_ADD &&
			switch (LOWORD(wParam))
			{
			case IDC_BTN_ADD:
			{
				LPWSTR s = (LPWSTR)Disp((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), hWnd, L"");
				if (s)
				{
					switch (pSelf->state)
					{
					case sAccounts:
						addAccount(pSelf->pc, s);
						LoadAccounts(hWnd);
						break;
					case sDrivers:
						addDriver(pSelf->pc, s);
						LoadDrivers(hWnd);
						break;
					case sCars:
						addCar(pSelf->pc, s);
						LoadCars(hWnd);
						break;
					}
					free(s);
				}
				break;
			}
			case IDC_BTN_DELETE:
				if (pSelf->selected)
				{
					switch (pSelf->state)
					{
					case sAccounts:
						deleteFromTable(pSelf->pc, tAccount, pSelf->accounts->data[pSelf->selection.i].id);
						LoadAccounts(hWnd);
						pSelf->selected = FALSE;
						break;
					case sDrivers:
						deleteFromTable(pSelf->pc, tDriver, pSelf->drivers->data[pSelf->selection.i].id);
						LoadDrivers(hWnd);
						pSelf->selected = FALSE;
						break;
					case sCars:
						deleteFromTable(pSelf->pc, tCar, pSelf->cars->data[pSelf->selection.i].id);
						LoadCars(hWnd);
						pSelf->selected = FALSE;
						break;
					}
				}
				
			}
		}
		switch (LOWORD(wParam))
		{
		case IDM_OPEN:
			openDatabase(pSelf->hWnd);
			break;
		case IDM_CREATE:
			createDatabase(pSelf->hWnd);
			break;
		case IDM_DRIVERS:
			if (pSelf->state != sDrivers)
			{
				pSelf->selected = FALSE;
				pSelf->state = sDrivers;
				if (pSelf->pc)
					LoadDrivers(hWnd);
				if (!IsWindowVisible(pSelf->hBtnAdd))
				{
					ShowWindow(pSelf->hBtnAdd, SW_SHOW);
					ShowWindow(pSelf->hBtnDelete, SW_SHOW);

				}
			}
			break;
		case IDM_ACCOUNTS:
			if (pSelf->state != sAccounts)
			{
				pSelf->selected = FALSE;
				pSelf->state = sAccounts;
				if (pSelf->pc)
					LoadAccounts(hWnd);
				if (!IsWindowVisible(pSelf->hBtnAdd))
				{
					ShowWindow(pSelf->hBtnAdd, SW_SHOW);
					ShowWindow(pSelf->hBtnDelete, SW_SHOW);
				}
			}
			break;
		case IDM_CARS:
			if (pSelf->state != sCars)
			{
				pSelf->selected = FALSE;
				pSelf->state = sCars;
				if (pSelf->pc)
					LoadCars(hWnd);
				if (!IsWindowVisible(pSelf->hBtnAdd))
				{
					ShowWindow(pSelf->hBtnAdd, SW_SHOW);
					ShowWindow(pSelf->hBtnDelete, SW_SHOW);
				}
			}
			break;
		case IDM_REPORT:
			pSelf->state = sReport;
			ShowWindow(pSelf->hBtnAdd, SW_HIDE);
			ShowWindow(pSelf->hBtnDelete, SW_HIDE);

			break;
		}
		InvalidateRect(pSelf->hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDOWN:

		switch (pSelf->state)
		{
		case sDrivers:
		{
			Select(pSelf, pSelf->tDrivers, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		}
		case sAccounts:
		{
			Select(pSelf, pSelf->tAccounts, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		}
		case sCars:
			Select(pSelf, pSelf->tCars, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		//ShowEdit(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), pSelf->hEdit);
		break;

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
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
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1); ;
	RegisterClassEx(&wcex);


	hMainWindow = CreateWindowEx(0, lpszClassName, L"My Table", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 800, 600, 0, 0, 0, NULL);

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

PWSTR OpenDialog(HWND hWnd)
{

	OPENFILENAMEW ofn;
	PWSTR lpszFile;

	lpszFile = (PWSTR)malloc(STRBUF_MAX_SIZE * sizeof(WCHAR));
	if (lpszFile)
	{
		lpszFile[0] = '\0';

		memset(&ofn, 0, sizeof ofn);
		ofn.lStructSize = sizeof ofn;
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = lpszFile;
		ofn.nMaxFile = STRBUF_MAX_SIZE;
		ofn.lpstrFilter = STR_FILTER;//last must be terminated by two \0's
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.lpstrTitle = STR_OPEN_TITLE;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		if (GetOpenFileName(&ofn) != TRUE)
		{
			free(lpszFile);
			lpszFile = NULL;
		}
	}
	return lpszFile;
}

PWSTR SaveDialog(HWND hWnd)
{
	OPENFILENAME sfn;
	PWSTR lpszFile;

	lpszFile = (PWSTR)malloc(STRBUF_MAX_SIZE * sizeof(WCHAR));
	if (lpszFile)
	{
		lpszFile[0] = '\0';

		memset(&sfn, 0, sizeof sfn);
		sfn.lStructSize = sizeof sfn;
		sfn.hwndOwner = hWnd;
		sfn.lpstrFile = lpszFile;
		sfn.nMaxFile = STRBUF_MAX_SIZE;
		sfn.lpstrFilter = STR_FILTER;//last must be terminated by two \0's
		sfn.nFilterIndex = 1;
		sfn.lpstrFileTitle = NULL;
		sfn.nMaxFileTitle = 0;
		sfn.lpstrInitialDir = NULL;
		sfn.lpstrTitle = STR_SAVE_TITLE;
		sfn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

		if (GetSaveFileName(&sfn) != TRUE)
		{
			free(lpszFile);
			lpszFile = NULL;
		}
	}
	return lpszFile;
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

void openDatabase(HWND hWnd)
{
	PMainWindow pSelf;
	PWSTR filename;
	pSelf = (PMainWindow)GetWindowLong(hWnd, 0);
	if (pSelf)
	{
		filename = OpenDialog(hWnd);
		if (filename)
		{
			if (pSelf->pc)
			{
				closeDB(pSelf->pc);
			}
			pSelf->pc = openDB(filename);
			free(filename);
		}
	}
}

void createDatabase(HWND hWnd)
{
	PMainWindow pSelf;
	PWSTR filename;
	pSelf = (PMainWindow)GetWindowLong(hWnd, 0);
	if (pSelf)
	{
		filename = SaveDialog(hWnd);
		if (filename)
		{
			if (pSelf->pc)
			{
				closeDB(pSelf->pc);
			}
			pSelf->pc = createDB(filename);
			free(filename);
		}
	}
}

void PrintTable(PMainWindow pSelf, HDC hdc, PTable t)
{
	int x, y;
	size_t* pcch = malloc(sizeof(size_t));
	LPWSTR s = NULL;
	int* num;
	if (t)
	{
		x = t->x;
		y = t->y;
		for (int i = 0; i < t->rowCount; i++)
		{
			if (pSelf->selected && (i == pSelf->selection.i))
			{
				SaveDC(hdc);
				SetBkColor(hdc, BACK_COLOR);
			}
			switch (getDataType(t, i, 0))
			{
			case tText:
				s = (LPWSTR)getData(t, i, 0);
				break;
			case tInt:
				s = malloc(STRBUF_MAX_SIZE);
				if (s)
				{
					num = (int*)(getData(t, i, 0));
					wsprintf("%i", s, *num);
				}
			}
			StringCchLength(s, STRBUF_MAX_SIZE, pcch);
			if (pcch)
			{
				TextOut(hdc, x, y, s, *pcch);
			}
			if (pSelf->selected && (i == pSelf->selection.i))
			{
				RestoreDC(hdc, -1);
			}
			if (getDataType(t, i, 0) == tInt && s)
			{
				free(s);
			}
			y += t->rowHeight;
		}

	}
	free(pcch);
}

void Paint(HWND hWnd)
{
	HDC hdc;
	RECT rc;
	HFONT hfnt, hfntPrev;
	HRESULT hr;
	PMainWindow pSelf = GetWindowLong(hWnd, 0);
	PAINTSTRUCT ps;
	hdc = BeginPaint(hWnd, &ps);
	
	switch (pSelf->state) {
	case sDrivers:
		PrintTable(pSelf, hdc, pSelf->tDrivers);		
		break;
	case sAccounts:
		PrintTable(pSelf, hdc, pSelf->tAccounts);
		break;
	case sCars: 
		PrintTable(pSelf, hdc, pSelf->tCars);
		break;
	case sReport:
		PrintTable(pSelf, hdc, pSelf->drivers);
	}
	EndPaint(hWnd, &ps);
}

void LoadDrivers(HWND hWnd)
{
	PMainWindow pSelf;
	int i;
	pSelf = (PMainWindow)GetWindowLong(hWnd, 0);
	if (pSelf->drivers)
	{
		freeDrivers(pSelf->drivers);
	}
	pSelf->drivers = getDrivers(pSelf->pc);
	if (pSelf->tDrivers)
	{
		freeTable(pSelf->tDrivers);
	}
	pSelf->tDrivers = createTable(pSelf->drivers->count, 1, 0, DY_TABLE);

	for (int i = 0; i < pSelf->drivers->count; i++)
	{
		setData(pSelf->tDrivers, i, 0, (pSelf->drivers->data)[i].name, tText);
	}
}

void LoadAccounts(HWND hWnd)
{
	PMainWindow pSelf;
	int i;
	pSelf = (PMainWindow)GetWindowLong(hWnd, 0);
	if (pSelf->accounts)
	{
		freeAccounts(pSelf->accounts);
	}
	pSelf->accounts = getAccounts(pSelf->pc);
	if (pSelf->tAccounts)
	{
		freeTable(pSelf->tAccounts);
	}
	pSelf->tAccounts = createTable(pSelf->accounts->count, 1, 0, DY_TABLE);

	for (int i = 0; i < pSelf->accounts->count; i++)
	{
		setData(pSelf->tAccounts, i, 0, (pSelf->accounts->data)[i].name, tText);
	}
}

void LoadCars(HWND hWnd)
{
	PMainWindow pSelf;
	int i;
	pSelf = (PMainWindow)GetWindowLong(hWnd, 0);
	if (pSelf->cars)
	{
		freeCars(pSelf->cars);
	}
	pSelf->cars = getCars(pSelf->pc);
	if (pSelf->tCars)
	{
		freeTable(pSelf->tCars);
	}
	pSelf->tCars = createTable(pSelf->cars->count, 1, 0, DY_TABLE);

	for (int i = 0; i < pSelf->cars->count; i++)
	{
		setData(pSelf->tCars, i, 0, (pSelf->cars->data)[i].number, tText);
	}
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
	LPWSTR result = 0;
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			result = malloc(STRBUF_MAX_SIZE * sizeof(wchar_t));
			if (result)
			{
				if (!GetDlgItemText(hwndDlg, ID_TEXT, result, STRBUF_MAX_SIZE))
				{
					free(result);
					result = 0;
				}
			}
		case IDCANCEL:
			EndDialog(hwndDlg, (INT_PTR)result);
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
	if (lpdt)
	{

		// Define a dialog box.

		lpdt->style = WS_POPUP | WS_BORDER | WS_SYSMENU | DS_MODALFRAME | WS_CAPTION;
		lpdt->cdit = 3;         // Number of controls
		lpdt->x = DLG_X;  lpdt->y = DLG_Y;
		lpdt->cx = DLG_CX; lpdt->cy = DLG_CY;

		lpw = (LPWORD)(lpdt + 1);
		*lpw++ = 0;             // No menu
		*lpw++ = 0;             // Predefined dialog box class (by default)

		lpwsz = (LPWSTR)lpw;
		StringCchCopy(lpwsz, 100, ADD_TEXT);
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
		StringCchCopy(lpwsz, 100, OK_TEXT);
		StringCchLength(lpwsz, 100, &nchar);
		lpw += nchar + 2;
		*lpw++ = 0;              // No creation data

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
		StringCchCopy(lpwsz, 100, CANCEL_TEXT);
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
	}

	GlobalUnlock(hgbl);
	ret = DialogBoxIndirect(hInstance,
		(LPDLGTEMPLATE)hgbl,
		hWnd,
		(DLGPROC)DialogProc);
	GlobalFree(hgbl);
	return ret;
}

TPos getID(PTable t, int x0, int y0)
{
	int i, j;
	int x, y;
	if (t)
	{
		y = t->y;
		i = (y0 - y) / (t->rowHeight);
		if (i <= t->rowCount)
		{
			j = 0;
			x = t->x;
			while (x <= x0)
			{
				x += t->colWidths[j];
				j++;
			}
			if (j <= t->colCount)
			{
				return (TPos) { i, j - 1 };
			}
		}
	}
	return (TPos) { INV_POS, INV_POS };
}
