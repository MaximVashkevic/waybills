#include "database.h"
#include <stdlib.h>
#include <stdio.h>// temp

const char* const CREATE_TABLES_QUERY = "BEGIN TRANSACTION;\
		CREATE TABLE Drivers(\
			ID INTEGER PRIMARY KEY NOT NULL,\
			Name TEXT NOT NULL\
		);\
		CREATE TABLE Waybills(\
			ID INTEGER PRIMARY KEY NOT NULL,\
			DriverID INTEGER NOT NULL,\
			Date NUMERIC NOT NULL,\
			Number INTEGER NOT NULL,\
			Car TEXT NOT NULL,\
			FOREIGN KEY(DriverID) REFERENCES Drivers(ID)\
		);\
		CREATE TABLE Accounts(\
			ID INTEGER PRIMARY KEY NOT NULL,\
			Account TEXT NOT NULL\
		);\
		CREATE TABLE Subaccounts(\
			ID INTEGER PRIMARY KEY NOT NULL,\
			AccountID INTEGER NOT NULL,\
			Subaccount TEXT NOT NULL,\
			FOREIGN KEY(AccountID) REFERENCES Accounts(ID)\
		);\
		CREATE TABLE TKM(\
			ID INTEGER PRIMARY KEY NOT NULL,\
			WaybillID INTEGER NOT NULL,\
			SubaccountID INTEGER NOT NULL,\
			Amount INTEGER NOT NULL CHECK(Amount >= 0),\
			FOREIGN KEY(SubaccountID) REFERENCES Subaccounts(ID),\
			FOREIGN KEY(WaybillID) REFERENCES Waybills(ID)\
		);\
		COMMIT;";

pconnection openDB(const char* filename)
{
	pconnection pConnection;
	int rc;

	pConnection = (pconnection)malloc(sizeof(connection));
	rc = sqlite3_open(filename, &(pConnection->db));
	if (rc != SQLITE_OK)
	{
		sqlite3_close(pConnection->db);
		return NULL;
	}

	return pConnection;
}

pconnection createDB(const char* filename)
{
	pconnection pConnection;
	int rc;

	pConnection = (pconnection)malloc(sizeof(connection));
	rc = sqlite3_open(filename, &(pConnection->db));
	if (rc != SQLITE_OK)
	{
		sqlite3_close(pConnection->db);
		return NULL;
	}
	
	rc = sqlite3_exec(pConnection->db, CREATE_TABLES_QUERY, NULL, 0, NULL);
	
	if (rc != SQLITE_OK)
	{
		sqlite3_close(pConnection->db);
		return NULL;
	}
	return pConnection;
}

void closeDB(pconnection pConnection)
{
	sqlite3_close(pConnection->db);
	free(pConnection);
}