#include "Table.h"
#include <stdlib.h>
#include <stdarg.h>

ptable createTable(int rows, int columns, int x, int y)
{
	int i, j;
	ptable t;
	t = (ptable)calloc(1, sizeof(table));
	if (t)
	{
		t->rowCount = rows;
		t->colCount = columns;
		t->x = x;
		t->y = y;
		t->rowHeight = ROW_HEIGTH;
		if (columns * rows > 0)
			t->cells = (pcell)calloc(rows * columns, sizeof(cell));
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
			if(t)
				free(t);
			t = NULL;
		}
	}
	return t;
}

void freeTable(ptable table)
{
	if (table)
	{
		if (table->colWidths)
			free(table->colWidths);
		if (table->cells)
			free(table->cells);
		free(table);
	}
}

void setData(ptable table, int row, int column, void* data, enum type type)
{
	if (row < table->rowCount && column < table->colCount)
	{
		table->cells[row * (table->colCount) + column].type = type;
		table->cells[row * (table->colCount) + column].data = data;
	}
}
wchar_t* getData(ptable table, int row, int column)
{
	wchar_t* temp;
	switch (table->cells[row * (table->colCount) + column].type)
	{
	case tInt:
		//temp = malloc(MAX_BUF_SIZE * sizeof(wchar_t));
		



	case tText:
		return (wchar_t*)table->cells[row * (table->colCount) + column].data;
	}
}