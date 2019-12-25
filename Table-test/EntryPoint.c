#include "EntryPoint.h"
#include <CommCtrl.h>
#include <windowsx.h>
#include "resource.h"
#include <strsafe.h>
#include <malloc.h>
extern freeAndNULL(void* p);

void ShowEdit(int x, int y, HWND hWnd)
{
	SetWindowPos(hWnd, HWND_TOP, x, y, 100, 20, SWP_SHOWWINDOW);
}

void hideControls(PMainWindow pSelf)
{
	ShowWindow(pSelf->hBtnAdd, SW_HIDE);
	ShowWindow(pSelf->hBtnDelete, SW_HIDE);
	ShowWindow(pSelf->hBtnEdit, SW_HIDE);
	ShowWindow(pSelf->hComboBox, SW_HIDE);
	ShowWindow(pSelf->hEdit, SW_HIDE);
}

void onOpenClick(PMainWindow pSelf)
{
	openDatabase(pSelf->hWnd);
	hideControls(pSelf);
	pSelf->state = sEmpty;
	if (pSelf->pc)
	{
		insertDates(pSelf->pc);
	}
}

void setCombobox(PMainWindow pSelf)
{
	int i;
	LoadCars(pSelf);
	SendMessage(pSelf->hComboBox, (UINT)CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
	for (i = 0; i < pSelf->cars->count; i++)
	{
		SendMessage(pSelf->hComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)((PCar)pSelf->cars->data)[i].number);
	}
}
void onCreateClick(PMainWindow pSelf)
{
	createDatabase(pSelf->hWnd);
	hideControls(pSelf);
	pSelf->state = sEmpty;
	if (pSelf->pc)
	{
		insertDates(pSelf->pc);
	}
}

void onDriversClick(PMainWindow pSelf)
{
	if (pSelf->state != sDrivers)
	{
		pSelf->selection.selected = FALSE;
		pSelf->state = sDrivers;
		if (pSelf->pc)
		{
			LoadDrivers(pSelf);
			hideControls(pSelf);
			ShowWindow(pSelf->hBtnAdd, SW_SHOW);
			ShowWindow(pSelf->hBtnDelete, SW_SHOW);
			ShowWindow(pSelf->hBtnEdit, SW_SHOW);
		}
	}
}

void onAccountsClick(PMainWindow pSelf)
{
	if (pSelf->state != sAccounts)
	{
		pSelf->selection.selected = FALSE;
		pSelf->state = sAccounts;
		if (pSelf->pc)
		{
			LoadAccounts(pSelf);
			hideControls(pSelf);
			ShowWindow(pSelf->hBtnAdd, SW_SHOW);
			ShowWindow(pSelf->hBtnDelete, SW_SHOW);
		}
	}
}

void onCarsClick(PMainWindow pSelf)
{
	if (pSelf->state != sCars)
	{
		pSelf->selection.selected = FALSE;
		pSelf->state = sCars;
		if (pSelf->pc)
		{
			LoadCars(pSelf);
			hideControls(pSelf);
			ShowWindow(pSelf->hBtnAdd, SW_SHOW);
			ShowWindow(pSelf->hBtnDelete, SW_SHOW);
		}
	}
}

void onReportClick(PMainWindow pSelf)
{
	if (pSelf->state != sReport)
	{
		pSelf->selection.selected = FALSE;
		pSelf->state = sReport;
		if (pSelf->pc)
		{
			LoadReport(pSelf);
			hideControls(pSelf);
		}
	}
}

void onEditDeselect(PMainWindow pSelf, TSelection prevSelection)
{	
	WCHAR s[STRBUF_MAX_SIZE];
	if (GetWindowText(pSelf->hEdit, s, STRBUF_MAX_SIZE))
	{
		int waybillID = ((PWaybill*)(pSelf->waybills->data))[prevSelection.row - 1]->id;
		int amount = wcstol(s, NULL, 10);
		int accountID = ((PAccount)pSelf->accounts->data)[prevSelection.col - 2].id;
		addTKM(pSelf->pc, waybillID, accountID, amount);
	}
	SetWindowText(pSelf->hEdit, L"");
	LoadTKMData(pSelf);
	ShowWindow(pSelf->hEdit, SW_HIDE);
}
void onClickEditing(PMainWindow pSelf, LPARAM lParam)
{
	TSelection prevSelection = pSelf->selection;
	pSelf->selection = getSelection(pSelf->tTkm, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	if (pSelf->selection.selected)
	{
		if (pSelf->selection.col == 0)
		{
			ShowWindow(pSelf->hComboBox, SW_HIDE);
			onComboboxDeselect(pSelf, prevSelection);
			onEditDeselect(pSelf, prevSelection);

		}
		else if (pSelf->selection.col == 1)
		{
			SetWindowPos(pSelf->hComboBox, HWND_TOP, 100, pSelf->selection.row * ROW_HEIGTH, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
			ShowWindow(pSelf->hEdit, SW_HIDE);
			onEditDeselect(pSelf, prevSelection);

		}
		else
		{
			onComboboxDeselect(pSelf, prevSelection);
			onEditDeselect(pSelf, prevSelection);
			if (((PWaybill*)pSelf->waybills->data)[pSelf->selection.row - 1])
			{
				SetWindowPos(pSelf->hEdit, HWND_TOP, 100 + (pSelf->selection.col - 1) * COL_WIDTH, pSelf->selection.row * ROW_HEIGTH, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
			}
		}
	}
	else
	{
		ShowWindow(pSelf->hComboBox, SW_HIDE);
		ShowWindow(pSelf->hEdit, SW_HIDE);
		onComboboxDeselect(pSelf, prevSelection);
		onEditDeselect(pSelf, prevSelection);
	}

}

void onLMButtonClick(PMainWindow pSelf, LPARAM lParam)
{
	switch (pSelf->state)
	{
	case sDrivers:
	{
		pSelf->selection = getSelection(pSelf->tDrivers, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}
	case sAccounts:
	{
		pSelf->selection = getSelection(pSelf->tAccounts, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}
	case sCars:
	{
		pSelf->selection = getSelection(pSelf->tCars, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}
	case sEditing:
		onClickEditing(pSelf, lParam);
		break;
	}
}

void onComboboxDeselect(PMainWindow pSelf, TSelection prevSelection)
{
	int i = SendMessage(pSelf->hComboBox, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (i != CB_ERR && prevSelection.selected)
	{
		int carid = ((PCar)pSelf->cars->data)[i].id;
		addWaybill(pSelf->pc, pSelf->driverID, ((PDate)pSelf->days->data)[prevSelection.row - 1].id, 0, carid);
	}
	SendMessage(pSelf->hComboBox, (UINT)CB_SETCURSEL, (WPARAM)-1, (LPARAM)0);
	LoadTKMData(pSelf);
	ShowWindow(pSelf->hComboBox, SW_HIDE);
}

void InitializeWindow(PMainWindow pSelf)
{
	pSelf->hBtnAdd = CreateWindowEx(0, L"BUTTON", L"Добавить", WS_CHILD | BS_PUSHBUTTON,
		0, 0, 100, 20, pSelf->hWnd, IDC_BTN_ADD, 0, NULL);
	pSelf->hBtnDelete = CreateWindowEx(0, L"BUTTON", L"Удалить", WS_CHILD | BS_PUSHBUTTON,
		105, 0, 100, 20, pSelf->hWnd, IDC_BTN_DELETE, 0, NULL);
	pSelf->hComboBox = CreateWindowEx(0, WC_COMBOBOX, L"", CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED, 0, 0, COL_WIDTH, 200,
		pSelf->hWnd, IDC_COMBOBOX, 0, NULL);
	pSelf->hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | ES_AUTOHSCROLL,
		120, 0, COL_WIDTH, ROW_HEIGTH, pSelf->hWnd, IDC_EDIT, 0, NULL);
	pSelf->hBtnEdit = CreateWindowEx(0, L"BUTTON", L"Редактировать", WS_CHILD | BS_PUSHBUTTON,
		210, 0, 150, 20, pSelf->hWnd, IDC_BTN_EDIT, 0, NULL);
	pSelf->state = sEmpty;
}

void onAddClick(PMainWindow pSelf)
{
	LPWSTR s = (LPWSTR)Disp((HINSTANCE)GetWindowLong(pSelf->hWnd, GWL_HINSTANCE), pSelf->hWnd, L"");
	if (s)
	{
		switch (pSelf->state)
		{
		case sAccounts:
			addAccount(pSelf->pc, s);
			LoadAccounts(pSelf);
			break;
		case sDrivers:
			addDriver(pSelf->pc, s);
			LoadDrivers(pSelf);
			break;
		case sCars:
			addCar(pSelf->pc, s);
			LoadCars(pSelf);
			break;
		}
		freeAndNULL(&s);
	}
}

void onDeleteClick(PMainWindow pSelf)
{
	if (pSelf->selection.selected)
	{
		switch (pSelf->state)
		{
		case sAccounts:
			deleteFromTable(pSelf->pc, tAccount, ((PAccount)pSelf->accounts->data)[pSelf->selection.row].id);
			LoadAccounts(pSelf);
			pSelf->selection.selected = FALSE;
			break;
		case sDrivers:
			deleteFromTable(pSelf->pc, tDriver, ((PDriver)pSelf->drivers->data)[pSelf->selection.row].id);
			LoadDrivers(pSelf);
			pSelf->selection.selected = FALSE;
			break;
		case sCars:
			deleteFromTable(pSelf->pc, tCar, ((PCar)pSelf->cars->data)[pSelf->selection.row].id);
			LoadCars(pSelf);
			pSelf->selection.selected = FALSE;
			break;
		}
	}
}

void onEditClick(PMainWindow pSelf)
{
	if (pSelf->selection.selected)
	{
		pSelf->selection.selected = FALSE;
		pSelf->driverID = ((PDriver)pSelf->drivers->data)[pSelf->selection.row].id;
		pSelf->state = sEditing;
		LoadTKM(pSelf);
		ShowWindow(pSelf->hBtnAdd, SW_HIDE);
		ShowWindow(pSelf->hBtnDelete, SW_HIDE);
		ShowWindow(pSelf->hBtnEdit, SW_HIDE);
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
			SetWindowLong(hWnd, 0, (LONG)pSelf);
			InitializeWindow(pSelf);

			/*s afsldkkf asdkf jdsa
			!!!!!!!!!!!!!!
			*/
			pSelf->pc = openDB(L"C:\\Users\\DELL\\Desktop\\sql\\1.dbf");
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
		onPaint(hWnd);
		break;
	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED)
		{
			switch (LOWORD(wParam))
			{
			case IDC_BTN_ADD:
			{
				onAddClick(pSelf);
				break;
			}
			case IDC_BTN_DELETE:
				onDeleteClick(pSelf);
				break;
			case IDC_BTN_EDIT:
				onEditClick(pSelf);
				break;
			}
		}
		if (HIWORD(wParam) == IDC_COMBOBOX && LOWORD(wParam) == CBN_KILLFOCUS)
		{
			onComboboxDeselect(pSelf, pSelf->selection);
		}
		switch (LOWORD(wParam))
		{
		case IDM_OPEN:
			onOpenClick(pSelf);
			break;
		case IDM_CREATE:
			onCreateClick(pSelf);
			break;
		case IDM_DRIVERS:
			onDriversClick(pSelf);
			break;
		case IDM_ACCOUNTS:
			onAccountsClick(pSelf);
			break;
		case IDM_CARS:
			onCarsClick(pSelf);
			break;
		case IDM_REPORT:
			onReportClick(pSelf);
			break;
		}
		InvalidateRect(pSelf->hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDOWN:
		onLMButtonClick(pSelf, lParam);
		InvalidateRect(hWnd, NULL, TRUE);
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


	hMainWindow = CreateWindowEx(0, lpszClassName, L"Таблица", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, WIN_SIZE_X, WIN_SIZE_Y, 0, 0, 0, NULL);

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
			freeAndNULL(&lpszFile);
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
			freeAndNULL(&lpszFile);
			lpszFile = NULL;
		}
	}
	return lpszFile;
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
			freeAndNULL(&filename);
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
			freeAndNULL(&filename);
		}
	}
}

void PrintTable(PMainWindow pSelf, HDC hdc, PTable t)
{
	int x, y;
	size_t* pcch = calloc(1, sizeof(size_t));
	LPWSTR s = NULL;
	int* num;
	if (t)
	{
		x = t->x;
		y = t->y;
		for (int i = 0; i < t->rowCount; i++)
		{
			x = t->x;
			for (int j = 0; j < t->colCount; j++)
			{
				if (pSelf->selection.selected && (i == pSelf->selection.row) && (j == pSelf->selection.col))
				{
					SaveDC(hdc);
					SetBkColor(hdc, BACK_COLOR);
				}
				switch (getDataType(t, i, j))
				{
				case tText:
					s = (LPWSTR)getData(t, i, j);
					break;
				case tInt:
					s = calloc(STRBUF_MAX_SIZE, sizeof(WCHAR));
					if (s)
					{
						num = (int*)(getData(t, i, j));
						int n = *num;
						StringCbPrintf(s, STRBUF_MAX_SIZE, L"%d", n);
					}
				}
				StringCchLength(s, STRBUF_MAX_SIZE, pcch);
				if (pcch)
				{
					TextOut(hdc, x, y, s, *pcch);
				}
				if (pSelf->selection.selected && (i == pSelf->selection.row) && (j == pSelf->selection.col))
				{
					RestoreDC(hdc, -1);
				}
				if (getDataType(t, i, j) == tInt && s)
				{
					freeAndNULL(&s);
				}
				x += t->colWidths[j];
			}
			y += t->rowHeight;
		}
	}
	freeAndNULL(&pcch);
}

void onPaint(HWND hWnd)
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
		PrintTable(pSelf, hdc, pSelf->tReport);
		break;
	case sEditing:
		PrintTable(pSelf, hdc, pSelf->tTkm);
		break;
	}
	EndPaint(hWnd, &ps);
}

void LoadDrivers(PMainWindow pSelf)
{
	int i;
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
		setData(pSelf->tDrivers, i, 0, ((PDriver)pSelf->drivers->data)[i].name, tText);
	}
}

void LoadAccounts(PMainWindow pSelf)
{
	int i;
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
		setData(pSelf->tAccounts, i, 0, ((PAccount)pSelf->accounts->data)[i].name, tText);
	}
}

void LoadCars(PMainWindow pSelf)
{
	int i;
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
		setData(pSelf->tCars, i, 0, ((PCar)pSelf->cars->data)[i].number, tText);
	}
}

void LoadReport(PMainWindow pSelf)
{
	int i, j;
	LoadAccounts(pSelf);
	LoadDrivers(pSelf);
	if (pSelf->tReport)
	{
		freeTable(pSelf->tReport);
	}
	pSelf->tReport = createTable(pSelf->drivers->count + 1, pSelf->accounts->count + 2, 0, 0); // + 1 + 2
	setColWidth(pSelf->tReport, 0, 200);
	for (i = 0; i < pSelf->tDrivers->rowCount; i++)
	{
		setData(pSelf->tReport, i + 1, 0, getData(pSelf->tDrivers, i, 0), tText);
	}
	for (i = 0; i < pSelf->tAccounts->rowCount; i++)
	{
		setData(pSelf->tReport, 0, i + 1, getData(pSelf->tAccounts, i, 0), tText);
	}
	if (pSelf->sums)
	{
		if (pSelf->sums->matrix)
		{
			for (i = 0; i < pSelf->sums->count; i++)
			{
				freeData(pSelf->sums->matrix[i]);
			}
			freeAndNULL(&(pSelf->sums->matrix));
		}
		freeAndNULL(&(pSelf->sums));
	}
	if (pSelf->totals)
	{
		freeAndNULL(&(pSelf->totals));
	}
	pSelf->totals = (int*)calloc(pSelf->drivers->count + 1, sizeof(int));
	pSelf->sums = (PMatrix)calloc(1, sizeof(TMatrix));
	if (pSelf->sums)
	{
		pSelf->sums->count = pSelf->drivers->count;
		pSelf->sums->matrix = (PArray*)calloc(pSelf->sums->count, sizeof(PArray));
		if (pSelf->sums->matrix && pSelf->totals)
		{
			pSelf->totals[pSelf->drivers->count] = getTotalSum(pSelf->pc);
			setData(pSelf->tReport, 0, pSelf->tReport->colCount - 1, &(pSelf->totals[pSelf->drivers->count]), tInt);
			for (i = 0; i < pSelf->drivers->count; i++)
			{
				pSelf->sums->matrix[i] = getSumByDriver(pSelf->pc, ((PDriver)pSelf->drivers->data)[i].id);
				for (j = 0; j < pSelf->sums->matrix[i]->count; j++)
				{
					setData(pSelf->tReport, i + 1, j + 1, &(((PDatum)(pSelf->sums->matrix[i]->data))[j].sum), tInt);
				}
				pSelf->totals[i] = getTotalSumByDriver(pSelf->pc, ((PDriver)pSelf->drivers->data)[i].id);
				setData(pSelf->tReport, i + 1, pSelf->tReport->colCount - 1, &(pSelf->totals[i]), tInt);

			}
		}
	}
}

int getIFromCarID(PMainWindow pSelf, int carID)
{
	for (int i = 0; i < pSelf->cars->count; i++)
	{
		if (((PCar)pSelf->cars->data)[i].id == carID)
		{
			return i;
		}
	}
	return 0xFFFFFFFF;
}

int getIFromAccountID(PMainWindow pSelf, int accountID)
{
	for (int i = 0; i < pSelf->accounts->count; i++)
	{
		if (((PAccount)pSelf->accounts->data)[i].id == accountID)
		{
			return i;
		}
	}
	return 0xFFFFFFFF;
}

void LoadTKMData(PMainWindow pSelf)
{
	int i, j;
	LoadAccounts(pSelf);
	if (pSelf->days)
	{
		freeDays(pSelf->days);
	}
	pSelf->days = getDays(pSelf->pc);
	if (pSelf->tTkm)
	{
		freeTable(pSelf->tTkm);
	}

	pSelf->tTkm = createTable(pSelf->days->count + 1, pSelf->accounts->count + 2, 0, 0);
	setData(pSelf->tTkm, 0, 0, DateLabel, tText);
	setData(pSelf->tTkm, 0, 1, CarLabel, tText);
	if (pSelf->waybills)
	{
		if (pSelf->waybills->data)
		{
			for (i = 0; i < pSelf->waybills->count; i++)
			{
				freeAndNULL(&(((PWaybill*)pSelf->waybills->data)[i]));
			}
			freeAndNULL(&(pSelf->waybills->data));
		}
		freeAndNULL(&(pSelf->waybills));

	}
	pSelf->waybills = (PArray)calloc(1, sizeof(TArray));
	{
		if (pSelf->waybills)
		{
			pSelf->waybills->count = pSelf->days->count;
			pSelf->waybills->data = (PWaybill*)calloc(pSelf->waybills->count, sizeof(PWaybill));
		}
	}
	if (pSelf->tkm)
	{
		if (pSelf->tkm->matrix)
		{
			for (i = 0; i < pSelf->tkm->count; i++)
			{
				freeData(pSelf->tkm->matrix[i]);
			}
			freeAndNULL(&(pSelf->tkm->matrix));
		}
		freeAndNULL(&(pSelf->tkm));
	}

	pSelf->tkm = (PMatrix)calloc(1, sizeof(TMatrix));
	if (pSelf->tkm)
	{
		pSelf->tkm->count = pSelf->days->count;
		pSelf->tkm->matrix = (PArray*)calloc(pSelf->tkm->count, sizeof(PArray));
		if (pSelf->tkm->matrix && pSelf->waybills)
		{
			for (i = 0; i < pSelf->days->count; i++) {
				setData(pSelf->tTkm, i + 1, 0, ((PDate)pSelf->days->data)[i].date, tText);
				((PWaybill*)pSelf->waybills->data)[i] = getWaybill(pSelf->pc, pSelf->driverID, ((PDate)pSelf->days->data)[i].id);
				if (((PWaybill*)pSelf->waybills->data)[i])
				{
					int carID = ((PWaybill*)pSelf->waybills->data)[i]->carID;
					int id = getIFromCarID(pSelf, carID);
					LPWSTR s = ((PCar)pSelf->cars->data)[id].number;
					setData(pSelf->tTkm, i + 1, 1, s, tText);
					
					pSelf->tkm->matrix[i] = getTKM(pSelf->pc, ((PWaybill*)pSelf->waybills->data)[i]->dateID, pSelf->driverID);
					
					if (pSelf->tkm->matrix[i])
					{
						for (j = 0; j < pSelf->tkm->matrix[i]->count; j++)
						{
							int accountID = ((PTKM)pSelf->tkm->matrix[i]->data)[j].accountID;
							int id = getIFromAccountID(pSelf, accountID) + 2;
							int* in = &(((PTKM)pSelf->tkm->matrix[i]->data)[j].amount);
							setData(pSelf->tTkm, i + 1, id, in, tInt);
						}
					}
					
				}
			}
			for (i = 0; i < pSelf->accounts->count; i++)
			{
				setData(pSelf->tTkm, 0, i + 2, ((PAccount)pSelf->accounts->data)[i].name, tText);
			}

		}

	}

}
void LoadTKM(PMainWindow pSelf)
{
	setCombobox(pSelf);
	LoadTKMData(pSelf);
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
					freeAndNULL(&result);
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

TSelection getSelection(PTable t, int x0, int y0)
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
				return (TSelection) { i, j - 1, TRUE };
			}
		}
	}
	return (TSelection) { INV_POS, INV_POS, FALSE };
}
