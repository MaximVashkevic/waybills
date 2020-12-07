#include "EntryPoint.h"
#include <CommCtrl.h>
#include <windowsx.h>
#include "resource.h"
#include <strsafe.h>
#include <malloc.h>
extern freeAndNULL(void* p);

void hideControls(PMainWindow pSelf)
{
	ShowWindow(pSelf->hBtnAdd, SW_HIDE);
	ShowWindow(pSelf->hBtnDelete, SW_HIDE);
	ShowWindow(pSelf->hBtnEdit, SW_HIDE);
	ShowWindow(pSelf->hComboBox, SW_HIDE);
	ShowWindow(pSelf->hEdit, SW_HIDE);
}

void afterOpen(PMainWindow pSelf)
{
	hideControls(pSelf);
	pSelf->state = sEmpty;
	if (pSelf->pc)
	{
		insertDatesIfNotExist(pSelf->pc);
	}
}

void onOpenClick(PMainWindow pSelf)
{
	openDatabase(pSelf->hWnd);
	afterOpen(pSelf);
}

void setCombobox(PMainWindow pSelf)
{
	int i;
	loadCars(pSelf);
	SendMessage(pSelf->hComboBox, (UINT)CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
	if (pSelf->cars)
	{
		for (i = 0; i < pSelf->cars->count; i++)
		{
			SendMessage(pSelf->hComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)((PCar)pSelf->cars->data)[i].number);
		}
	}
}
void onCreateClick(PMainWindow pSelf)
{
	createDatabase(pSelf->hWnd);
	afterOpen(pSelf);
}

void onDriversClick(PMainWindow pSelf)
{
	if (pSelf->state != sDrivers)
	{
		pSelf->selection.selected = FALSE;
		pSelf->state = sDrivers;
		if (pSelf->pc)
		{
			loadDrivers(pSelf);
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
			loadAccounts(pSelf);
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
			loadCars(pSelf);
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
			loadReport(pSelf);
			hideControls(pSelf);
		}
	}
}

void onEditDeselect(PMainWindow pSelf, TSelection prevSelection)
{
	WCHAR strBuf[STRBUF_MAX_SIZE];
	if (GetWindowText(pSelf->hEdit, strBuf, STRBUF_MAX_SIZE))
	{
		int waybillID = ((PWaybill*)(pSelf->waybills->data))[prevSelection.row - 1]->id;
		int amount = wcstol(strBuf, NULL, 10);
		int accountID = ((PAccount)pSelf->accounts->data)[prevSelection.col - 2].id;
		addTKM(pSelf->pc, waybillID, accountID, amount);
	}
	SetWindowText(pSelf->hEdit, L"");
	loadTKMData(pSelf);
	ShowWindow(pSelf->hEdit, SW_HIDE);
}

void onClickEditing(PMainWindow pSelf, LPARAM lParam)
{
	TSelection prevSelection = pSelf->selection;
	pSelf->selection = getTableSelection(pSelf->tTkm, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
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
		pSelf->selection = getTableSelection(pSelf->tDrivers, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}
	case sAccounts:
	{
		pSelf->selection = getTableSelection(pSelf->tAccounts, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}
	case sCars:
	{
		pSelf->selection = getTableSelection(pSelf->tCars, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
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
	loadTKMData(pSelf);
	ShowWindow(pSelf->hComboBox, SW_HIDE);
}

void initializeWindow(PMainWindow pSelf)
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
	LPWSTR value = (LPWSTR)getStringFromDialog((HINSTANCE)GetWindowLong(pSelf->hWnd, GWL_HINSTANCE), pSelf->hWnd, L"");
	if (value)
	{
		switch (pSelf->state)
		{
		case sAccounts:
			addAccount(pSelf->pc, value);
			loadAccounts(pSelf);
			break;
		case sDrivers:
			addDriver(pSelf->pc, value);
			loadDrivers(pSelf);
			break;
		case sCars:
			addCar(pSelf->pc, value);
			loadCars(pSelf);
			break;
		}
		freeAndNULL(&value);
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
			loadAccounts(pSelf);
			break;
		case sDrivers:
			deleteFromTable(pSelf->pc, tDriver, ((PDriver)pSelf->drivers->data)[pSelf->selection.row].id);
			loadDrivers(pSelf);
			break;
		case sCars:
			deleteFromTable(pSelf->pc, tCar, ((PCar)pSelf->cars->data)[pSelf->selection.row].id);
			loadCars(pSelf);
			break;
		}
		pSelf->selection.selected = FALSE;
	}
}

void onEditClick(PMainWindow pSelf)
{
	if (pSelf->selection.selected)
	{
		pSelf->selection.selected = FALSE;
		pSelf->driverID = ((PDriver)pSelf->drivers->data)[pSelf->selection.row].id;
		pSelf->state = sEditing;
		loadTKM(pSelf);
		hideControls(pSelf);
	}
}

void onCommand(PMainWindow pSelf, WPARAM wParam)
{
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
			initializeWindow(pSelf);
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
		onCommand(pSelf, wParam);
		InvalidateRect(pSelf->hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDOWN:
		onLMButtonClick(pSelf, lParam);
		InvalidateRect(pSelf->hWnd, NULL, TRUE);
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
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	RegisterClassEx(&wcex);

	hMainWindow = CreateWindowEx(0, lpszClassName, L"Таблица", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, WIN_SIZE_X, WIN_SIZE_Y, 0, 0, 0, NULL);

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
void initOfn(OPENFILENAME* ofn, HWND hWnd, LPCWSTR title, LPWSTR resBuffer, DWORD flags)
{
	memset(ofn, 0, sizeof(OPENFILENAME));
	ofn->lStructSize = sizeof(OPENFILENAME);
	ofn->hwndOwner = hWnd;
	ofn->lpstrFile = resBuffer;
	ofn->nMaxFile = STRBUF_MAX_SIZE;
	ofn->lpstrFilter = STR_FILTER;//last must be terminated by two \0's
	ofn->nFilterIndex = 1;
	ofn->lpstrFileTitle = NULL;
	ofn->nMaxFileTitle = 0;
	ofn->lpstrInitialDir = NULL;
	ofn->lpstrTitle = title;
	ofn->Flags = flags;
}

PWSTR OpenDialog(HWND hWnd)
{
	OPENFILENAMEW ofn;
	PWSTR lpszFile = (PWSTR)malloc(STRBUF_MAX_SIZE * sizeof(WCHAR));
	if (lpszFile)
	{
		lpszFile[0] = '\0';
		initOfn(&ofn, hWnd, STR_OPEN_TITLE, lpszFile, OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST);
		if (GetOpenFileName(&ofn) != TRUE)
		{
			freeAndNULL(&lpszFile);
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

		initOfn(&sfn, hWnd, STR_SAVE_TITLE, lpszFile, OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT);

		if (GetSaveFileName(&sfn) != TRUE)
		{
			freeAndNULL(&lpszFile);
		}
	}
	return lpszFile;
}

void handleOpenCreate(HWND hWnd, PWSTR(*Dialog)(HWND), PConnection(*DB)(const wchar_t*))
{
	PMainWindow pSelf;
	PWSTR filename;
	pSelf = (PMainWindow)GetWindowLong(hWnd, 0);
	if (pSelf)
	{
		filename = Dialog(hWnd);
		if (filename)
		{
			if (pSelf->pc)
			{
				closeDB(pSelf->pc);
			}
			pSelf->pc = DB(filename);
			freeAndNULL(&filename);
		}
	}	
}

void openDatabase(HWND hWnd)
{
	handleOpenCreate(hWnd, OpenDialog, openDB);
}

void createDatabase(HWND hWnd)
{
	handleOpenCreate(hWnd, SaveDialog, createDB);
}

LPWSTR getTableTextFromInt(PTable table, int row, int col)
{
	LPWSTR result = calloc(STRBUF_MAX_SIZE, sizeof(WCHAR));
	if (result)
	{
		StringCbPrintf(result, STRBUF_MAX_SIZE, L"%d", *((int*)(getData(table, row, col))));
	}
	return result;
}

BOOL isCellSelected(PMainWindow pSelf, int row, int col)
{
	return pSelf->selection.selected && (row == pSelf->selection.row) && (col == pSelf->selection.col);
}

void printCell(PMainWindow pSelf, HDC hdc, PTable table, int row, int col, int x, int y)
{
	size_t charCount;
	LPWSTR strBuf = NULL;
	if (isCellSelected(pSelf, row, col))
	{
		SaveDC(hdc);
		SetBkColor(hdc, BACK_COLOR);
	}
	switch (getDataType(table, row, col))
	{
	case tText:
		strBuf = (LPWSTR)getData(table, row, col);
		break;
	case tInt:
		strBuf = getTableTextFromInt(table, row, col);
	}
	if (StringCchLength(strBuf, STRBUF_MAX_SIZE, &charCount) == S_OK)
	{
		TextOut(hdc, x, y, strBuf, charCount);
	}
	if (isCellSelected(pSelf, row, col))
	{
		RestoreDC(hdc, -1);
	}
	if (getDataType(table, row, col) == tInt && strBuf)
	{
		freeAndNULL(&strBuf);
	}
}

void PrintTable(PMainWindow pSelf, HDC hdc, PTable table)
{
	int x, y;
	if (table)
	{
		x = table->x;
		y = table->y;
		for (int i = 0; i < table->rowCount; i++)
		{
			x = table->x;
			for (int j = 0; j < table->colCount; j++)
			{
				printCell(pSelf, hdc, table, i, j, x, y);
				x += table->colWidths[j];
			}
			y += table->rowHeight;
		}
	}
}

void onPaint(HWND hWnd)
{
	HDC hdc;
	PMainWindow pSelf = (PMainWindow)GetWindowLong(hWnd, 0);
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

void loadDrivers(PMainWindow pSelf)
{
	if (pSelf->drivers)
	{
		freeDrivers(pSelf->drivers);
	}
	pSelf->drivers = getDrivers(pSelf->pc);

	if (pSelf->tDrivers)
	{
		freeTable(pSelf->tDrivers);
	}
	if (pSelf->drivers)
	{
		pSelf->tDrivers = createTable(pSelf->drivers->count, 1, 0, DY_TABLE);

		for (int i = 0; i < pSelf->drivers->count; i++)
		{
			setData(pSelf->tDrivers, i, 0, ((PDriver)pSelf->drivers->data)[i].name, tText);
		}
	}
}

void loadAccounts(PMainWindow pSelf)
{
	if (pSelf->accounts)
	{
		freeAccounts(pSelf->accounts);
	}
	pSelf->accounts = getAccounts(pSelf->pc);

	if (pSelf->tAccounts)
	{
		freeTable(pSelf->tAccounts);
	}
	if (pSelf->accounts)
	{
		pSelf->tAccounts = createTable(pSelf->accounts->count, 1, 0, DY_TABLE);

		for (int i = 0; i < pSelf->accounts->count; i++)
		{
			setData(pSelf->tAccounts, i, 0, ((PAccount)pSelf->accounts->data)[i].name, tText);
		}
	}
}

void loadCars(PMainWindow pSelf)
{
	if (pSelf->cars)
	{
		freeCars(pSelf->cars);
	}
	pSelf->cars = getCars(pSelf->pc);

	if (pSelf->tCars)
	{
		freeTable(pSelf->tCars);
	}
	if (pSelf->cars)
	{
		pSelf->tCars = createTable(pSelf->cars->count, 1, 0, DY_TABLE);

		for (int i = 0; i < pSelf->cars->count; i++)
		{
			setData(pSelf->tCars, i, 0, ((PCar)pSelf->cars->data)[i].number, tText);
		}
	}
}

void freeSums(PMainWindow pSelf)
{
	if (pSelf->sums)
	{
		if (pSelf->sums->matrix)
		{
			for (int i = 0; i < pSelf->sums->count; i++)
			{
				freeData(pSelf->sums->matrix[i]);
			}
			freeAndNULL(&(pSelf->sums->matrix));
		}
		freeAndNULL(&(pSelf->sums));
	}
}

void loadReportInitiate(PMainWindow pSelf)
{
	int i;
	loadAccounts(pSelf);
	loadDrivers(pSelf);
	if (pSelf->tReport)
	{
		freeTable(pSelf->tReport);
	}
	if (pSelf->drivers && pSelf->accounts)
	{
		pSelf->tReport = createTable(pSelf->drivers->count + 1, pSelf->accounts->count + 2, 0, 0);
		setColWidth(pSelf->tReport, 0, 200);

		for (i = 0; i < pSelf->tDrivers->rowCount; i++)
		{
			setData(pSelf->tReport, i + 1, 0, getData(pSelf->tDrivers, i, 0), tText);
		}
		for (i = 0; i < pSelf->tAccounts->rowCount; i++)
		{
			setData(pSelf->tReport, 0, i + 1, getData(pSelf->tAccounts, i, 0), tText);
		}

		if (pSelf->totals)
		{
			freeAndNULL(&(pSelf->totals));
		}
		pSelf->totals = (int*)calloc(pSelf->drivers->count + 1, sizeof(int));

		freeSums(pSelf);
		pSelf->sums = (PMatrix)calloc(1, sizeof(TMatrix));
	}
}

void loadReport(PMainWindow pSelf)
{
	int i, j;
	loadReportInitiate(pSelf);
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
				//setData(pSelf->tReport, i + 1, pSelf->tReport->colCount - 1, &(pSelf->totals[i]), tInt);
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


void freeTKM(PMainWindow pSelf)
{
	if (pSelf->tkm)
	{
		if (pSelf->tkm->matrix)
		{
			for (int i = 0; i < pSelf->tkm->count; i++)
			{
				freeData(pSelf->tkm->matrix[i]);
			}
			freeAndNULL(&(pSelf->tkm->matrix));
		}
		freeAndNULL(&(pSelf->tkm));
	}
}

void freeWaybills(PMainWindow pSelf)
{
	if (pSelf->waybills)
	{
		if (pSelf->waybills->data)
		{
			for (int i = 0; i < pSelf->waybills->count; i++)
			{
				freeAndNULL(&(((PWaybill*)pSelf->waybills->data)[i]));
			}
			freeAndNULL(&(pSelf->waybills->data));
		}
		freeAndNULL(&(pSelf->waybills));
	}
}

void loadTKMDataInitiate(PMainWindow pSelf)
{
	loadAccounts(pSelf);

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

	freeWaybills(pSelf);
	pSelf->waybills = (PArray)calloc(1, sizeof(TArray));
	{
		if (pSelf->waybills)
		{
			pSelf->waybills->count = pSelf->days->count;
			pSelf->waybills->data = (PWaybill*)calloc(pSelf->waybills->count, sizeof(PWaybill));
		}
	}

	freeTKM(pSelf);
	pSelf->tkm = (PMatrix)calloc(1, sizeof(TMatrix));
}

void loadTKMData(PMainWindow pSelf)
{
	int i, j;
	loadTKMDataInitiate(pSelf);
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
					setData(pSelf->tTkm, i + 1, 1, ((PCar)pSelf->cars->data)[id].number, tText);

					pSelf->tkm->matrix[i] = getTKM(pSelf->pc, ((PWaybill*)pSelf->waybills->data)[i]->dateID, pSelf->driverID);

					if (pSelf->tkm->matrix[i])
					{
						for (j = 0; j < pSelf->tkm->matrix[i]->count; j++)
						{
							setData(pSelf->tTkm, i + 1, getIFromAccountID(pSelf, ((PTKM)pSelf->tkm->matrix[i]->data)[j].accountID) + 2, &(((PTKM)pSelf->tkm->matrix[i]->data)[j].amount), tInt);
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

void loadTKM(PMainWindow pSelf)
{
	setCombobox(pSelf);
	loadTKMData(pSelf);
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
				if (!GetDlgItemText(hwndDlg, IDC_DIALOG_EDIT, result, STRBUF_MAX_SIZE))
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

LRESULT getStringFromDialog(HINSTANCE hInstance, HWND hWnd, LPWSTR lpszMessage)
{
	LRESULT result;
	result = DialogBox(hInstance,
		MAKEINTRESOURCE(IDD_DIALOG1),
		hWnd,
		(DLGPROC)DialogProc);
	return result;
}

TSelection getTableSelection(PTable table, int x0, int y0)
{
	int i, j;
	int x, y;
	if (table)
	{
		y = table->y;
		i = (y0 - y) / (table->rowHeight);
		if (i <= table->rowCount)
		{
			j = 0;
			x = table->x;
			while (x <= x0)
			{
				x += table->colWidths[j];
				j++;
			}
			if (j <= table->colCount)
			{
				return (TSelection) { i, j - 1, TRUE };
			}
		}
	}
	return (TSelection) { INV_POS, INV_POS, FALSE };
}
