#pragma once
#include <Windows.h>
#include "../sqlite-test/database.h"
#include "Table.h"

#define STRBUF_MAX_SIZE 260
#define IDC_BTN_ADD	(HMENU)100
#define IDC_BTN_DELETE (HMENU)101
#define IDC_BTN_EDIT	(HMENU)102
#define IDC_EDIT	(HMENU)103
#define IDC_COMBOBOX (HMENU)104
#define ID_TEXT 200

#define BACK_COLOR RGB(173, 216, 230)

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

#define DY_TABLE 20

#define WIN_SIZE_X 1280
#define WIN_SIZE_Y 1280

#define INV_POS 0xFFFFFFFF

const LPWSTR const ADD_TEXT = L"Добавить";
const LPWSTR const OK_TEXT = L"OK";
const LPWSTR const CANCEL_TEXT = L"Отмена";
const LPWSTR const lpszClassName = L"MainWindowClass";
const LPWSTR const CarLabel = L"Машина";
const LPWSTR const DateLabel = L"Дата";

enum state {
	sEmpty, sDrivers, sAccounts, sCars, sReport, sEditing
};
typedef struct _tselection
{
	int row, col;
	BOOL selected;
} TSelection, *PSelection;
typedef struct _TMainWindow
{
	HWND hWnd;
	HWND hBtnAdd;
	HWND hBtnDelete;
	HWND hBtnEdit;
	HWND hEdit;
	HWND hComboBox;
	PConnection pc;
	PArray drivers, accounts, cars, days, waybills;
	PMatrix sums, tkm;
	int* totals;
	enum state state;
	PTable tDrivers, tAccounts, tTkm, tCars, tReport;
	TSelection selection;
	int driverID;
} TMainWindow, * PMainWindow;

const WCHAR* const STR_SAVE_TITLE = L"Сохранить как";
const WCHAR* const STR_OPEN_TITLE = L"Открыть";
const WCHAR* const STR_FILTER = L"All\0*.*\0Файл базы данных\0*.dbf\0";
void openDatabase(HWND hWnd);
void createDatabase(HWND hWnd);
PWSTR OpenDialog(HWND hWnd);
PWSTR SaveDialog(HWND hWnd);
void onPaint(HWND hWnd);
void LoadDrivers(PMainWindow pSelf);
void LoadAccounts(PMainWindow pSelf);
void LoadCars(PMainWindow pSelf);
void LoadReport(PMainWindow pSelf);
void LoadTKM(PMainWindow pSelf);
void LoadTKMData(PMainWindow pSelf);
LRESULT Disp(HINSTANCE hInstance, HWND hWnd, LPWSTR lpszMessage);
TSelection getSelection(PTable t, int x, int y);

int getIFromCarID(PMainWindow pSelf, int carID);
void onComboboxDeselect(PMainWindow pSelf);