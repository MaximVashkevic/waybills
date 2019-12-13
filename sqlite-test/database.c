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
			CarID INTEGER NOT NULL,\
			FOREIGN KEY(DriverID) REFERENCES Drivers(ID),\
			FOREIGN KEY(CarID) REFERENCES Cars(ID)\
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
		CREATE TABLE Cars(\
			ID INTEGER PRIMARY KEY NOT NULL,\
			Number TEXT NOT NULL\
		);\
		COMMIT;";

#define AUTOLENGTH -1
const char* const ADD_DRIVER_QUERY = "INSERT INTO Drivers(Name) VALUES(:Name);";
const char* const ADD_CAR_QUERY = "INSERT INTO Cars(Number) VALUES(:Number);";
const char* const ADD_WAYBILL_QUERY = "INSERT INTO Waybills(DriverID, Date, Number, CarID) VALUES(:DriverID, :Date, :Number, :CarID);";
const char* const ADD_ACCOUNT_QUERY = "INSERT INTO Accounts(Account) VALUES(:Account);";
const char* const ADD_SUBACCOUNT_QUERY = "INSERT INTO Subaccounts(AccountID, Subaccount) VALUES(:AccountID, :Subaccount);";
const char* const ADD_TKM_QUERY = "INSERT INTO TKM(WaybillID, SubaccountID, Amount) VALUES(:WaybillID, :SubaccountID, :Amount);";
const char* const DRIVER_ID_NAME = ":DriverID";
const char* const DATE_NAME = ":Date";
const char* const NUMBER_NAME = ":Number";
const char* const CAR_ID_NAME = ":CarID";
const char* const ACCOUNT_NAME = ":Account";
const char* const ACCOUNT_ID_NAME = ":AccountID";
const char* const SUBACCOUNT_NAME = ":Subaccount";
const char* const WAYBILL_ID_NAME = ":WaybillID";
const char* const SUBACCOUNT_ID_NAME = ":SubaccountID";
const char* const AMOUNT_NAME = ":Amount";
const char* const NAME_NAME = ":Name";

pconnection openDB(const char* filename)
{
	pconnection pConnection;
	int rc;

	pConnection = (pconnection)malloc(sizeof(connection));
	if (pConnection)
	{
		rc = sqlite3_open(filename, &(pConnection->db));
		if (rc != SQLITE_OK)
		{
			sqlite3_close(pConnection->db);
			return NULL;
		}
	}

	return pConnection;
}

pconnection createDB(const char* filename)
{
	pconnection pConnection;
	int rc;

	pConnection = (pconnection)malloc(sizeof(connection));
	if (pConnection)
	{
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
	}
	return pConnection;
}

void closeDB(pconnection pc)
{
	sqlite3_close(pc->db);
	free(pc);
}

int addDriver(pconnection pc, char* name)
{
	sqlite3_stmt* stmt;
	int result;
	result = 0;
	if (pc)
	{
		if (!sqlite3_prepare_v2(pc->db, ADD_DRIVER_QUERY, AUTOLENGTH, &stmt, NULL))
		{
			sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, NAME_NAME), name, AUTOLENGTH, SQLITE_TRANSIENT);
			result = sqlite3_step(stmt) == SQLITE_DONE;
		}
		sqlite3_finalize(stmt);
	}
	return result;
}

int addCar(pconnection pc, char* number)
{
	sqlite3_stmt* stmt;
	int result;
	result = 0;
	if (pc)
	{
		if (!sqlite3_prepare_v2(pc->db, ADD_CAR_QUERY, AUTOLENGTH, &stmt, NULL))
		{
			sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, NUMBER_NAME), number, AUTOLENGTH, SQLITE_TRANSIENT);
			result = sqlite3_step(stmt) == SQLITE_DONE;
		}
		sqlite3_finalize(stmt);
	}
	return result;
}

int addWaybill(pconnection pc, int driverID, int date, int number, int carID)
{
	sqlite3_stmt* stmt;
	int result;
	result = 0;
	if (pc)
	{
		if (!sqlite3_prepare_v2(pc->db, ADD_WAYBILL_QUERY, AUTOLENGTH, &stmt, NULL))
		{
			sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, DRIVER_ID_NAME), driverID);
			sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, DATE_NAME), date);
			sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, NUMBER_NAME), number);
			sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, CAR_ID_NAME), carID);
			result = sqlite3_step(stmt) == SQLITE_DONE;
		}
		sqlite3_finalize(stmt);
	}
	return result;
}

int addAccount(pconnection pc, char* account)
{
	sqlite3_stmt* stmt;
	int result;
	result = 0;
	if (pc)
	{
		if (!sqlite3_prepare_v2(pc->db, ADD_ACCOUNT_QUERY, AUTOLENGTH, &stmt, NULL))
		{
			sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ACCOUNT_NAME), account, AUTOLENGTH, SQLITE_TRANSIENT);
			result = sqlite3_step(stmt) == SQLITE_DONE;
		}
		sqlite3_finalize(stmt);
	}
	return result;
}

int addSubaccount(pconnection pc, int accountID, char* subaccount)
{
	sqlite3_stmt* stmt;
	int result;
	result = 0;
	if (pc)
	{
		if (!sqlite3_prepare_v2(pc->db, ADD_SUBACCOUNT_QUERY, AUTOLENGTH, &stmt, NULL))
		{
			sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ACCOUNT_ID_NAME), accountID);
			sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, SUBACCOUNT_NAME), subaccount, AUTOLENGTH, SQLITE_TRANSIENT);
			result = sqlite3_step(stmt) == SQLITE_DONE;
		}
		sqlite3_finalize(stmt);
	}
	return result;
}

int addTKM(pconnection pc, int waybillID, int subaccountID, int amount)
{
	sqlite3_stmt* stmt;
	int result;
	result = 0;
	if (pc)
	{
		if (!sqlite3_prepare_v2(pc->db, ADD_TKM_QUERY, AUTOLENGTH, &stmt, NULL))
		{
			sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, WAYBILL_ID_NAME), waybillID);
			sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, SUBACCOUNT_ID_NAME), subaccountID);
			sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, AMOUNT_NAME), amount);
			result = sqlite3_step(stmt) == SQLITE_DONE;
		}
		sqlite3_finalize(stmt);
	}
	return result;
}

int deleteDriver(pconnection pc, int id)
{
	return 0;
}