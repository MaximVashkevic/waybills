#include "Table.h"
#include <stdlib.h>
#include <stdarg.h>

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
				free(t->cells);
			if (t)
				free(t);
			t = NULL;
		}
	}
	return t;
}

void freeTable(PTable TTable)
{
	if (TTable)
	{
		if (TTable->colWidths)
			free(TTable->colWidths);
		if (TTable->cells)
			free(TTable->cells);
		free(TTable);
	}
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
