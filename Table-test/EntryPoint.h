#pragma once
#include <Windows.h>
#include "../sqlite-test/database.h"
#include "Table.h"

#define FILENAME_BUF_SIZE 260
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

#define DY_TABLE 20

enum state {
	sEmpty, sDrivers, sAccounts, sReport
};
typedef struct _TMainWindow
{
	HWND hWnd;
	HWND hBtnAdd;
	HWND hEdit;
	enum state state;
	pconnection pc;
	ptable tDrivers, tAccounts, tTkm, tSubaccounts;
	pdrivers drivers;
	paccounts accounts;
	psubaccounts subaccounts;
	
	int driverID;
	
} TMainWindow, * PMainWindow;

const WCHAR* const STR_SAVE_TITLE = L"Сохранить как";
const WCHAR* const STR_OPEN_TITLE = L"Открыть";
const WCHAR* const STR_FILTER = L"All\0*.*\0Файл базы данных\0*.dbf\0";
void openDatabase(HWND hWnd);
void createDatabase(HWND hWnd);
PWSTR OpenDialog(HWND hWnd);
PWSTR SaveDialog(HWND hWnd);
void Paint(HWND hWnd);
void LoadDrivers(HWND hWnd);
void LoadAccounts(hWnd);