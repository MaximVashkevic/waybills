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
} cell ,*pcell;

typedef struct _table
{
	int rowCount;
	int colCount;
	int rowHeight;
	int* colWidths;
	pcell cells;
	int x, y;
}table, *ptable;

ptable createTable(int rows, int columns, int x, int y);
void freeTable(ptable table);
wchar_t* getData(ptable table, int row, int column, enum type type);
void setData(ptable table, int row, int column, void * data, enum type type);
