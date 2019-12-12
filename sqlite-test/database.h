#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

typedef struct _connection
{
	sqlite3* db;
} connection, * pconnection;

pconnection openDB(char* filename);
pconnection createDB(char* filename);
void closeDB(pconnection pConnection);


#endif