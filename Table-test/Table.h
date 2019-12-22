#pragma once
#include <Windows.h>
#define COL_WIDTH 20
#define ROW_HEIGTH 100
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
}table, *ptable;

ptable createTable(int rows,int columns, ...);
void freeTable(ptable table);
wchar_t* getData(ptable table, int row, int column, enum type type);
void setData(ptable table, int row, int column, void * data, enum type type);
