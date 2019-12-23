#pragma once
#include <Windows.h>
#define COL_WIDTH 100
#define ROW_HEIGTH 20
#define MAX_STRBUF_SIZE 256
enum type {
	tText, tInt, tFloat
};
typedef struct _cell
{
	void* data;
	enum type type;
} TCell ,*PCell;

typedef struct _table
{
	int rowCount;
	int colCount;
	int rowHeight;
	int* colWidths;
	PCell cells;
	int x, y;
}TTable, *PTable;

PTable createTable(int rows, int columns, int x, int y);
void freeTable(PTable TTable);
wchar_t* getData(PTable TTable, int row, int column, enum type type);
void setData(PTable TTable, int row, int column, void * data, enum type type);
