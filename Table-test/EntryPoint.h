#pragma once
#include <Windows.h>
#include "../sqlite-test/database.h"
#include "Table.h"

#define MAX_STRBUF_SIZE 260
#define IDC_BTN_ADD	(HMENU)100
#define IDC_EDIT	(HMENU)101
#define ID_TEXT 200

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

#define INV_POS 0xFFFFFFFF

enum state {
	sEmpty, sDrivers, sAccounts, sReport
};
typedef struct _TMainWindow
{
	HWND hWnd;
	HWND hBtnAdd;
	HWND hBtnDelete;
	HWND hEdit;
	enum state state;
	PConnection pc;
	PTable tDrivers, tAccounts, tTkm, tSubaccounts;
	PDrivers drivers;
	PAccounts accounts;
	PSubaccounts subaccounts;
	int id;
	BOOL selected;
	
} TMainWindow, * PMainWindow;
typedef struct _tpos
{
	int x;
	int y;
} TPos, *PPos;

const WCHAR* const STR_SAVE_TITLE = L"Сохранить как";
const WCHAR* const STR_OPEN_TITLE = L"Открыть";
const WCHAR* const STR_FILTER = L"All\0*.*\0Файл базы данных\0*.dbf\0";
void openDatabase(HWND hWnd);
void createDatabase(HWND hWnd);
PWSTR OpenDialog(HWND hWnd);
PWSTR SaveDialog(HWND hWnd);
void Paint(HWND hWnd);
void LoadDrivers(HWND hWnd);
void LoadAccounts(HWND hWnd);
LRESULT Disp(HINSTANCE hInstance, HWND hWnd, LPWSTR lpszMessage);
TPos getID(PTable t, int x, int y);
