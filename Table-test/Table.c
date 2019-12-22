#include "Table.h"
#include <stdlib.h>
#include <stdarg.h>

ptable createTable(int rows, int columns, ...)
{
	int i, j;
	ptable table = (ptable)calloc(1, sizeof(table));
	if (table)
	{
		table->rowCount = rows;
		table->colCount = columns;
		table->rowHeight = 20;
		table->cells = (pcell)calloc(rows * columns, sizeof(cell));
		table->colWidths = (int*)malloc(columns * sizeof(int));
		if (table->colWidths)
		{
			va_list args;
			va_start(args, columns);
			for (i = 0; i < columns; i++)
			{
				table->colWidths[i] = va_arg(args, int);
			}
			va_end(args);
		}
		else
		{
			free(table->cells);
			free(table);
			table = NULL;
		}
	}
	return table;
}

void freeTable(ptable table)
{
	free(table->colWidths);
	free(table->cells);
}

void setData(ptable table, int row, int column, void * data, enum type type)
{
	if (row < table->rowCount && column < table->colCount)
	{
		table->cells[row * (table->colCount) + column].type = type;
		table->cells[row * (table->colCount) + column].data = data;
	}
}
wchar_t* getData(ptable table, int row, int column)
{
	WCHAR temp[255];
	switch (table->cells[row * (table->colCount) + column].type)
	{
	case tInt:
		

				
	case tText:
		return (wchar_t*)table->cells[row * (table->colCount) + column].data;
	}
}