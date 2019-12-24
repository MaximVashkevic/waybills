#include "Table.h"
#include <stdlib.h>
#include <stdarg.h>

extern freeAndNULL(void* p);

PTable createTable(int rows, int columns, int x, int y)
{
	int i, j;
	PTable t;
	t = (PTable)calloc(1, sizeof(TTable));
	if (t)
	{
		t->rowCount = rows;
		t->colCount = columns;
		t->x = x;
		t->y = y;
		t->rowHeight = ROW_HEIGTH;
		if (columns * rows > 0)
			t->cells = (PCell)calloc(rows * columns, sizeof(TCell));
		else
			t->cells = NULL;
		if (columns > 0)
			t->colWidths = (int*)calloc(columns, sizeof(int));
		else
			t->cells = NULL;
		if (t->colWidths)
		{
			for (i = 0; i < columns; i++)
				t->colWidths[i] = COL_WIDTH;
		}
		else
		{
			if (t->cells)
				freeAndNULL(t->cells);
			if (t)
				freeAndNULL(t);
			t = NULL;
		}
	}
	return t;
}

void freeTable(PTable table)
{
	if (table)
	{
		if (table->colWidths)
			freeAndNULL(table->colWidths);
		if (table->cells)
			freeAndNULL(table->cells);
		freeAndNULL(table);
	}
}

void setColWidth(PTable table, int column, int width)
{
	table->colWidths[column] = width;
}

void setData(PTable TTable, int row, int column, void* data, enum type type)
{
	if (row < TTable->rowCount && column < TTable->colCount)
	{
		TTable->cells[row * (TTable->colCount) + column].type = type;
		TTable->cells[row * (TTable->colCount) + column].data = data;
	}
}
void* getData(PTable TTable, int row, int column)
{
	return TTable->cells[row * (TTable->colCount) + column].data;
}

enum type getDataType(PTable TTable, int row, int column)
{
	return TTable->cells[row * (TTable->colCount) + column].type;
}
