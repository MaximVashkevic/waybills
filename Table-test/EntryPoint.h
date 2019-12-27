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

const WCHAR* const ADD_TEXT = L"Добавить";
const WCHAR* const OK_TEXT = L"OK";
const WCHAR* const CANCEL_TEXT = L"Отмена";
const WCHAR* const lpszClassName = L"MainWindowClass";
const WCHAR* const CarLabel = L"Машина";
const WCHAR* const DateLabel = L"Дата";

const WCHAR* const STR_SAVE_TITLE = L"Сохранить как";
const WCHAR* const STR_OPEN_TITLE = L"Открыть";
const WCHAR* const STR_FILTER = L"All\0*.*\0Файл базы данных\0*.dbf\0";

enum state {
	sEmpty, sDrivers, sAccounts, sCars, sReport, sEditing
};
typedef struct _tselection
{
	int row, col;
	BOOL selected;
} TSelection, * PSelection;
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

void openDatabase(HWND hWnd);
void createDatabase(HWND hWnd);
LPWSTR getTableTextFromInt(PTable table, int row, int col);
BOOL isCellSelected(PMainWindow pSelf, int row, int col);
void printCell(PMainWindow pSelf, HDC hdc, PTable table, int row, int col, int x, int y);
void PrintTable(PMainWindow pSelf, HDC hdc, PTable table);
PWSTR OpenDialog(HWND hWnd);
PWSTR SaveDialog(HWND hWnd);
void handleOpenCreate(HWND hWnd, PWSTR(*Dialog)(HWND), PConnection(*DB)(const wchar_t*));
void onPaint(HWND hWnd);
void loadDrivers(PMainWindow pSelf);
void loadAccounts(PMainWindow pSelf);
void loadCars(PMainWindow pSelf);
void loadReport(PMainWindow pSelf);
void loadTKM(PMainWindow pSelf);
void loadTKMData(PMainWindow pSelf);
LRESULT getStringFromDialog(HINSTANCE hInstance, HWND hWnd, LPWSTR lpszMessage);
TSelection getTableSelection(PTable t, int x, int y);

int getIFromCarID(PMainWindow pSelf, int carID);
int getIFromAccountID(PMainWindow pSelf, int accountID);
void onComboboxDeselect(PMainWindow pSelf, TSelection prevSelection);

void initializeWindow(PMainWindow pSelf);

void onAddClick(PMainWindow pSelf);
void onDeleteClick(PMainWindow pSelf);
void onEditClick(PMainWindow pSelf);

void onCommand(PMainWindow pSelf, WPARAM wParam);

void hideControls(PMainWindow pSelf);

void initOfn(OPENFILENAME* ofn, HWND hWnd, LPCWSTR title, LPWSTR resBuffer, DWORD flags);
void afterOpen(PMainWindow pSelf);
void onOpenClick(PMainWindow pSelf);

void setCombobox(PMainWindow pSelf);

void onCreateClick(PMainWindow pSelf);
void onDriversClick(PMainWindow pSelf);
void onAccountsClick(PMainWindow pSelf);
void onCarsClick(PMainWindow pSelf);
void onReportClick(PMainWindow pSelf);

void onEditDeselect(PMainWindow pSelf, TSelection prevSelection);
void onClickEditing(PMainWindow pSelf, LPARAM lParam);
void onLMButtonClick(PMainWindow pSelf, LPARAM lParam);


