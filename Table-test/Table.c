#include "Table.h"
#include <stdlib.h>
#include <stdarg.h>

extern freeAndNULL(void* p);

PTable createTable(int rows, int columns, int x, int y)
{
	int i;
	PTable table;
	table = (PTable)calloc(1, sizeof(TTable));
	if (table)
	{
		table->rowCount = rows;
		table->colCount = columns;
		table->x = x;
		table->y = y;
		table->rowHeight = ROW_HEIGTH;
		if (columns * rows > 0)
			table->cells = (PCell)calloc(rows * columns, sizeof(TCell));
		else
			table->cells = NULL;
		if (columns > 0)
			table->colWidths = (int*)calloc(columns, sizeof(int));
		else
			table->cells = NULL;
		if (table->colWidths)
		{
			for (i = 0; i < columns; i++)
				table->colWidths[i] = COL_WIDTH;
		}
		else
		{
			if (table->cells)
				freeAndNULL(&(table->cells));
			if (table)
				freeAndNULL(&table);
			table = NULL;
		}
	}
	return table;
}

void freeTable(PTable table)
{
	if (table)
	{
		if (table->colWidths)
			freeAndNULL(&(table->colWidths));
		if (table->cells)
			freeAndNULL(&(table->cells));
		freeAndNULL(&table);
	}
}

void setColWidth(PTable table, int column, int width)
{
	table->colWidths[column] = width;
}

void setData(PTable TTable, int row, int column, const void* data, enum type type)
{
	if (row < TTable->rowCount && column < TTable->colCount)
	{
		TTable->cells[row * (TTable->colCount) + column].type = type;
		TTable->cells[row * (TTable->colCount) + column].data = data;
	}
}
const void* getData(PTable TTable, int row, int column)
{
	return TTable->cells[row * (TTable->colCount) + column].data;
}

enum type getDataType(PTable TTable, int row, int column)
{
	return TTable->cells[row * (TTable->colCount) + column].type;
}
