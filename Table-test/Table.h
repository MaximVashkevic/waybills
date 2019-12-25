#pragma once
#include <Windows.h>
#define COL_WIDTH 100
#define ROW_HEIGTH 25
#define STRBUF_MAX_SIZE 260
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
void freeTable(PTable table);
void setColWidth(PTable table, int column, int width);
void* getData(PTable TTable, int row, int column);
enum type getDataType(PTable TTable, int row, int column);
void setData(PTable TTable, int row, int column, void * data, enum type type);
