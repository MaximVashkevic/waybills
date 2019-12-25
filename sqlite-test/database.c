#include "database.h"
#include <stdlib.h>
#include <corecrt_wstring.h>
#include <stdio.h>

#define BUF_SIZE 20

const char* const CREATE_TABLES_QUERY = "BEGIN TRANSACTION;\
		CREATE TABLE Drivers(\
			ID INTEGER PRIMARY KEY NOT NULL,\
			Name TEXT NOT NULL\
		);\
		CREATE TABLE Accounts(\
			ID INTEGER PRIMARY KEY NOT NULL,\
			Account TEXT NOT NULL\
		);\
		CREATE TABLE TKM(\
			ID INTEGER PRIMARY KEY NOT NULL,\
			WaybillID INTEGER NOT NULL,\
			AccountID INTEGER NOT NULL,\
			Amount INTEGER NOT NULL CHECK(Amount >= 0),\
			FOREIGN KEY(AccountID) REFERENCES Accounts(ID),\
			FOREIGN KEY(WaybillID) REFERENCES Waybills(ID)\
		);\
		CREATE TABLE Cars(\
			ID INTEGER PRIMARY KEY NOT NULL,\
			Number TEXT NOT NULL\
		);\
		CREATE TABLE Calendar(\
			ID INTEGER PRIMARY KEY NOT NULL,\
			Date NUMERIC NOT NULL\
		);\
		CREATE TABLE Waybills(\
			ID INTEGER PRIMARY KEY NOT NULL,\
			DriverID INTEGER NOT NULL,\
			DateID INTEGER NOT NULL,\
			Number INTEGER NOT NULL,\
			CarID INTEGER NOT NULL,\
			FOREIGN KEY(DriverID) REFERENCES Drivers(ID),\
			FOREIGN KEY(CarID) REFERENCES Cars(ID),\
			FOREIGN KEY(DateID)	 REFERENCES Calendar(ID)\
		);\
		COMMIT;";

#define AUTOLENGTH -1
const char* const ADD_DRIVER_QUERY = "INSERT INTO Drivers(Name) VALUES(:Name);";
const char* const ADD_CAR_QUERY = "INSERT INTO Cars(Number) VALUES(:Number);";
const char* const ADD_WAYBILL_QUERY = "INSERT INTO Waybills(DriverID, DateID, Number, CarID) VALUES(:DriverID, :DateID, :Number, :CarID);";
const char* const ADD_ACCOUNT_QUERY = "INSERT INTO Accounts(Account) VALUES(:Account);";
const char* const ADD_TKM_QUERY = "INSERT INTO TKM(WaybillID, SubaccountID, Amount) VALUES(:WaybillID, :SubaccountID, :Amount);";

const char* const BEGIN_TRANSACTION_QUERY = "BEGIN TRANSACTION;";
const char* const COMMIT_QUERY = "COMMIT;";
const char* const ROLLBACK_QUERY = "ROLLBACK;";

const char* const GET_DRIVERS_QUERY = "SELECT * FROM Drivers;";
const char* const COUNT_DRIVERS_QUERY = "SELECT COUNT() FROM Drivers;";

const char* const COUNT_ACCOUNTS_QUERY = "SELECT COUNT() FROM Accounts;";
const char* const GET_ACCOUNTS_QUERY = "SELECT * FROM Accounts;";

const char* const COUNT_CARS_QUERY = "SELECT COUNT() FROM Cars;";
const char* const GET_CARS_QUERY = "SELECT * FROM Cars;";

const char* const GET_WAYBILL_QUERY = "SELECT * FROM Waybills WHERE DriverID = :DriverID AND DateID = :DateID;";

const char* const GET_SUM_BY_DRIVER_QUERY = "SELECT TKM.AccountID, SUM(TKM.Amount) from TKM INNER JOIN Waybills ON Waybills.ID = TKM.WaybillID WHERE Waybills.DriverID = :DriverID GROUP BY TKM.AccountID;";
const char* const GET_TOTAL_SUM_BY_DRIVER_QUERY = "SELECT SUM(TKM.Amount) from TKM INNER JOIN Waybills ON Waybills.ID = TKM.WaybillID WHERE Waybills.DriverID = :DriverID;";
const char* const GET_TOTAL_SUM = "SELECT SUM(TKM.Amount) from TKM;";
const char* const DELETE_DRIVER_QUERY = "DELETE FROM Drivers WHERE ID = :ID;";
const char* const DELETE_CAR_QUERY = "DELETE FROM Cars WHERE ID = :ID;";
const char* const DELETE_WAYBILL_QUERY = "DELETE FROM Waybills WHERE ID = :ID;";
const char* const DELETE_ACCOUNT_QUERY = "DELETE FROM Accounts WHERE ID = :ID;";
const char* const DELETE_TKM_QUERY = "DELETE FROM TKM WHERE ID = :ID;";

const char* const GET_CUR_MONTH_DATES = "select id, strftime(\"%d\", calendar.Date), strftime(\"%d/%m\", calendar.Date) from calendar where strftime(\"%m/%Y\", date()) = strftime(\"%m/%Y\",calendar.Date);";
const char* const GET_CUR_MONTH_DAYS_NUM = "SELECT strftime(\"%d\", date('now','start of month','+1 month','-1 day'));";
const char* const INSERT_DATE = "INSERT INTO Calendar(Date) VALUES(DATE('now', 'start of month', :Days));";

const char* const DRIVER_ID_NAME = ":DriverID";
const char* const DAYS_NAME = ":Days";
const char* const DATE_NAME = ":DateID";
const char* const NUMBER_NAME = ":Number";
const char* const CAR_ID_NAME = ":CarID";
const char* const ACCOUNT_NAME = ":Account";
const char* const ACCOUNT_ID_NAME = ":AccountID";
const char* const SUBACCOUNT_NAME = ":Subaccount";
const char* const WAYBILL_ID_NAME = ":WaybillID";
const char* const SUBACCOUNT_ID_NAME = ":SubaccountID";
const char* const AMOUNT_NAME = ":Amount";
const char* const NAME_NAME = ":Name";
const char* const ID_NAME = ":ID";

freeAndNULL(void* p)
{
	free(p);
	p = NULL;
}

PConnection openDB(const wchar_t* filename)
{
	PConnection pConnection;
	int rc;

	pConnection = (PConnection)calloc(1, sizeof(TConnection));
	if (pConnection)
	{
		rc = sqlite3_open16(filename, &(pConnection->db));
		if (rc != SQLITE_OK)
		{
			sqlite3_close(pConnection->db);
			return NULL;
		}
	}
	return pConnection;
}

PConnection createDB(const wchar_t* filename)
{
	PConnection pConnection;
	int rc;

	pConnection = (PConnection)calloc(1, sizeof(TConnection));
	if (pConnection)
	{
		rc = sqlite3_open16(filename, &(pConnection->db));
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

void closeDB(PConnection pc)
{
	sqlite3_close(pc->db);
	freeAndNULL(pc);
}

void insertDates(PConnection pc)
{
	sqlite3_stmt* stmt, * daysStmt, * insertStmt;
	int days;
	char buf[BUF_SIZE];
	if (pc)
	{
		if (sqlite3_prepare_v2(pc->db, GET_CUR_MONTH_DATES, AUTOLENGTH, &stmt, NULL) == SQLITE_OK)
		{
			if (sqlite3_step(stmt) == SQLITE_DONE)
			{
				if (sqlite3_prepare_v2(pc->db, GET_CUR_MONTH_DAYS_NUM, AUTOLENGTH, &daysStmt, NULL) == SQLITE_OK)
				{
					if (sqlite3_step(daysStmt) == SQLITE_ROW)
					{
						days = sqlite3_column_int(daysStmt, 0);
						if (sqlite3_prepare_v2(pc->db, INSERT_DATE, AUTOLENGTH, &insertStmt, NULL) == SQLITE_OK)
						{
							for (int i = 0; i < days; i++)
							{
								sprintf_s(buf, BUF_SIZE, "+%i day", i);
								sqlite3_bind_text(insertStmt, sqlite3_bind_parameter_index(insertStmt, DAYS_NAME), buf, AUTOLENGTH, SQLITE_TRANSIENT);
								sqlite3_step(insertStmt);
								sqlite3_reset(insertStmt);
							}
						}
						sqlite3_finalize(insertStmt);
					}
				}
				sqlite3_finalize(daysStmt);
			}
		}
		sqlite3_finalize(stmt);
	}
}

int addDriver(PConnection pc, wchar_t* name)
{
	sqlite3_stmt* stmt;
	int result;
	result = 0;
	if (pc)
	{
		if (sqlite3_prepare_v2(pc->db, ADD_DRIVER_QUERY, AUTOLENGTH, &stmt, NULL) == SQLITE_OK)
		{
			sqlite3_bind_text16(stmt, sqlite3_bind_parameter_index(stmt, NAME_NAME), name, AUTOLENGTH, SQLITE_TRANSIENT);
			result = sqlite3_step(stmt) == SQLITE_DONE;
		}
		sqlite3_finalize(stmt);
	}
	return result;
}

int addCar(PConnection pc, wchar_t* number)
{
	sqlite3_stmt* stmt;
	int result;
	result = 0;
	if (pc)
	{
		if (sqlite3_prepare_v2(pc->db, ADD_CAR_QUERY, AUTOLENGTH, &stmt, NULL) == SQLITE_OK)
		{
			sqlite3_bind_text16(stmt, sqlite3_bind_parameter_index(stmt, NUMBER_NAME), number, AUTOLENGTH, SQLITE_TRANSIENT);
			result = sqlite3_step(stmt) == SQLITE_DONE;
		}
		sqlite3_finalize(stmt);
	}
	return result;
}

int addWaybill(PConnection pc, int driverID, int dateID, int number, int carID)
{
	sqlite3_stmt* stmt;
	int result;
	result = 0;
	if (pc)
	{
		if (sqlite3_prepare_v2(pc->db, ADD_WAYBILL_QUERY, AUTOLENGTH, &stmt, NULL) == SQLITE_OK)
		{
			sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, DRIVER_ID_NAME), driverID);
			sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, DATE_NAME), dateID);
			sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, NUMBER_NAME), number);
			sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, CAR_ID_NAME), carID);
			result = sqlite3_step(stmt) == SQLITE_DONE;
		}
		sqlite3_finalize(stmt);
	}
	return result;
}

int addAccount(PConnection pc, wchar_t* TAccount)
{
	sqlite3_stmt* stmt;
	int result;
	result = 0;
	if (pc)
	{
		if (sqlite3_prepare_v2(pc->db, ADD_ACCOUNT_QUERY, AUTOLENGTH, &stmt, NULL) == SQLITE_OK)
		{
			sqlite3_bind_text16(stmt, sqlite3_bind_parameter_index(stmt, ACCOUNT_NAME), TAccount, AUTOLENGTH, SQLITE_TRANSIENT);
			result = sqlite3_step(stmt) == SQLITE_DONE;
		}
		sqlite3_finalize(stmt);
	}
	return result;
}

int addTKM(PConnection pc, int waybillID, int subaccountID, int amount)
{
	sqlite3_stmt* stmt;
	int result;
	result = 0;
	if (pc)
	{
		if (sqlite3_prepare_v2(pc->db, ADD_TKM_QUERY, AUTOLENGTH, &stmt, NULL) == SQLITE_OK)
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

PArray getDrivers(PConnection pc)
{
	sqlite3_stmt* stmtCount, * stmtGet;
	int count;
	int i;
	const wchar_t* name;
	PArray result;
	result = (PArray)calloc(1, sizeof(TArray));
	if (result)
	{
		count = 0;
		sqlite3_exec(pc->db, BEGIN_TRANSACTION_QUERY, NULL, 0, NULL);
		if (sqlite3_prepare_v2(pc->db, COUNT_DRIVERS_QUERY, -1, &stmtCount, NULL) == SQLITE_OK)
		{
			if (sqlite3_step(stmtCount) == SQLITE_ROW)
			{
				count = sqlite3_column_int(stmtCount, 0);
				result->count = count;
				result->data = (PDriver)calloc(count, sizeof(TDriver));
				if (result->data != NULL) {
					if (sqlite3_prepare_v2(pc->db, GET_DRIVERS_QUERY, -1, &stmtGet, NULL) == SQLITE_OK)
					{
						i = 0;
						while (i < count)
						{
							if (sqlite3_step(stmtGet) == SQLITE_ROW)
							{
								name = sqlite3_column_text16(stmtGet, 1);
								((PDriver)(result->data))[i].id = sqlite3_column_int(stmtGet, 0);
								((PDriver)(result->data))[i].name = _wcsdup(name);
								wprintf(L"%s\n", name);
							}
							i++;
						}
					}
					sqlite3_finalize(stmtGet);
				}
				else
				{
					freeAndNULL(result);
					result = NULL;
				}
			}
		}
		sqlite3_finalize(stmtCount);
		sqlite3_exec(pc->db, COMMIT_QUERY, NULL, 0, NULL);
	}
	return result;
}

void freeDrivers(PArray drivers)
{
	int i;
	if (drivers)
	{
		for (i = 0; i < drivers->count; i++)
		{
			freeAndNULL(((PDriver)(drivers->data))[i].name);
		}
		if (drivers->data)
			freeAndNULL(drivers->data);
	}
	freeAndNULL(drivers);
}

PArray getAccounts(PConnection pc)
{
	sqlite3_stmt* stmtCount, * stmtGet;
	int count;
	int i;
	const wchar_t* name;
	PArray result;

	result = (PArray)calloc(1, sizeof(TArray));
	if (result)
	{
		count = 0;
		sqlite3_exec(pc->db, BEGIN_TRANSACTION_QUERY, NULL, 0, NULL);
		if (sqlite3_prepare_v2(pc->db, COUNT_ACCOUNTS_QUERY, -1, &stmtCount, NULL) == SQLITE_OK)
		{
			if (sqlite3_step(stmtCount) == SQLITE_ROW)
			{
				count = sqlite3_column_int(stmtCount, 0);
				result->count = count;
				result->data = (PAccount)calloc(count, sizeof(TAccount));

				if (result->data != NULL) {
					if (sqlite3_prepare_v2(pc->db, GET_ACCOUNTS_QUERY, -1, &stmtGet, NULL) == SQLITE_OK)
					{
						i = 0;
						while (i < count)
						{
							if (sqlite3_step(stmtGet) == SQLITE_ROW)
							{
								name = sqlite3_column_text16(stmtGet, 1);
								((PAccount)(result->data))[i].id = sqlite3_column_int(stmtGet, 0);
								((PAccount)(result->data))[i].name = _wcsdup(name);
								wprintf(L"%s\n", name);
							}
							i++;
						}
					}
					sqlite3_finalize(stmtGet);
				}
				else
				{
					freeAndNULL(result);
				}
			}
		}
		sqlite3_finalize(stmtCount);
		sqlite3_exec(pc->db, COMMIT_QUERY, NULL, 0, NULL);
	}
	return result;
}

PArray getCars(PConnection pc)
{
	sqlite3_stmt* stmtCount, * stmtGet;
	int count;
	int i;
	wchar_t* name;
	PArray result;

	result = (PArray)calloc(1, sizeof(TArray));
	if (result)
	{
		count = 0;
		sqlite3_exec(pc->db, BEGIN_TRANSACTION_QUERY, NULL, 0, NULL);
		if (sqlite3_prepare_v2(pc->db, COUNT_CARS_QUERY, -1, &stmtCount, NULL) == SQLITE_OK)
		{
			if (sqlite3_step(stmtCount) == SQLITE_ROW)
			{
				count = sqlite3_column_int(stmtCount, 0);
				result->count = count;
				result->data = (PCar)calloc(count, sizeof(TCar));

				if (result->data != NULL) {
					if (sqlite3_prepare_v2(pc->db, GET_CARS_QUERY, -1, &stmtGet, NULL) == SQLITE_OK)
					{
						i = 0;
						while (i < count)
						{
							if (sqlite3_step(stmtGet) == SQLITE_ROW)
							{
								name = sqlite3_column_text16(stmtGet, 1);
								((PCar)(result->data))[i].id = sqlite3_column_int(stmtGet, 0);
								((PCar)(result->data))[i].number = _wcsdup(name);
								wprintf(L"%s\n", name);
							}
							i++;
						}
					}
					sqlite3_finalize(stmtGet);
				}
				else
				{
					freeAndNULL(result);
					result = NULL;
				}
			}
		}
		sqlite3_finalize(stmtCount);
		sqlite3_exec(pc->db, COMMIT_QUERY, NULL, 0, NULL);
	}
	return result;
}

void freeCars(PArray cars)
{
	int i;
	if (cars)
	{
		for (i = 0; i < cars->count; i++)
		{
			freeAndNULL(((PCar)(cars->data))[i].number);
		}
		if (cars->data)
			freeAndNULL(cars->data);
	}
	freeAndNULL(cars);
}

void freeAccounts(PArray accounts)
{
	int i;
	if (accounts)
	{
		for (i = 0; i < accounts->count; i++)
		{
			freeAndNULL(((PAccount)(accounts->data))[i].name);
		}
		if (accounts->data)
			freeAndNULL(accounts->data);
	}
	freeAndNULL(accounts);
}

PArray getDays(PConnection pc)
{
	sqlite3_stmt* stmtCount, * stmtGet;
	PArray result;
	int count;
	wchar_t date[BUF_SIZE];
	int i;
	int chars;
	result = (PArray)calloc(1, sizeof(TArray));
	if (pc && result)
	{
		if (sqlite3_prepare_v2(pc->db, GET_CUR_MONTH_DAYS_NUM, AUTOLENGTH, &stmtCount, NULL) == SQLITE_OK)
		{
			if (sqlite3_step(stmtCount) == SQLITE_ROW)
			{
				count = sqlite3_column_int(stmtCount, 0);
				result->count = count;
				result->data = (PDate)calloc(count, sizeof(TDate));
				if (result->data)
				{
					if (sqlite3_prepare_v2(pc->db, GET_CUR_MONTH_DATES, AUTOLENGTH, &stmtGet, NULL) == SQLITE_OK)
					{
						i = 0;
						while (i < count)
						{
							if (sqlite3_step(stmtGet) == SQLITE_ROW)
							{
								((PDate)result->data)[i].id = sqlite3_column_int(stmtGet, 0);
								((PDate)result->data)[i].day = sqlite3_column_int(stmtGet, 1);
								mbstowcs_s(&chars, date, BUF_SIZE, sqlite3_column_text(stmtGet, 2), _TRUNCATE);
								((PDate)result->data)[i].date = _wcsdup(date);
							}
							i++;
						}

					}
					sqlite3_finalize(stmtGet);
				}
				else
				{
					freeAndNULL(result);
				}

			}
		}
		sqlite3_finalize(stmtCount);
	}
	return result;
}

void freeDays(PArray days)
{
	int i;
	if (days)
	{
		for (i = 0; i < days->count; i++)
		{
			freeAndNULL(((PDate)(days->data))[i].date);
		}
		if (days->data)
			freeAndNULL(days->data);
	}
	freeAndNULL(days);
}

PArray getSumByDriver(PConnection pc, int driverID)
{
	sqlite3_stmt* stmtCount, * stmtGet;
	int count;
	int i;
	const wchar_t* name;
	PArray result;

	result = (PArray)calloc(1, sizeof(TArray));
	if (result)
	{
		count = 0;
		sqlite3_exec(pc->db, BEGIN_TRANSACTION_QUERY, NULL, 0, NULL);
		if (sqlite3_prepare_v2(pc->db, COUNT_ACCOUNTS_QUERY, -1, &stmtCount, NULL) == SQLITE_OK)
		{
			if (sqlite3_step(stmtCount) == SQLITE_ROW)
			{
				count = sqlite3_column_int(stmtCount, 0);
				result->count = count;
				result->data = (PDatum)calloc(count, sizeof(TDatum));

				if (result->data != NULL) {
					if (sqlite3_prepare_v2(pc->db, GET_SUM_BY_DRIVER_QUERY, -1, &stmtGet, NULL) == SQLITE_OK)
					{
						i = 0;
						while (i < count)
						{
							if (sqlite3_step(stmtGet) == SQLITE_ROW)
							{
								((PDatum)(result->data))[i].id = sqlite3_column_int(stmtGet, 0);
								((PDatum)(result->data))[i].sum = sqlite3_column_int(stmtGet, 1);
							}
							i++;
						}
					}
					sqlite3_finalize(stmtGet);
				}
				else
				{
					freeAndNULL(result);
					result = NULL;
				}
			}
		}
		sqlite3_finalize(stmtCount);
		sqlite3_exec(pc->db, COMMIT_QUERY, NULL, 0, NULL);
	}
	return result;
}

int getTotalSumByDriver(PConnection pc, int driverID)
{
	sqlite3_stmt* stmt;
	int result;
	result = 0;
	if (pc)
	{
		if (sqlite3_prepare_v2(pc->db, GET_TOTAL_SUM_BY_DRIVER_QUERY, AUTOLENGTH, &stmt, NULL) == SQLITE_OK)
		{
			if (sqlite3_step(stmt) == SQLITE_ROW)
				result = sqlite3_column_int(stmt, 0);
		}
		sqlite3_finalize(stmt);
	}
	return result;
}

int getTotalSum(PConnection pc)
{
	sqlite3_stmt* stmt;
	int result;
	result = 0;
	if (pc)
	{
		if (sqlite3_prepare_v2(pc->db, GET_TOTAL_SUM, AUTOLENGTH, &stmt, NULL) == SQLITE_OK)
		{
			if (sqlite3_step(stmt) == SQLITE_ROW)
				result = sqlite3_column_int(stmt, 0);
		}
		sqlite3_finalize(stmt);
	}
	return result;
}

void freeData(PArray data)
{
	if (data)
	{
		if (data->data)
			freeAndNULL(data->data);
		freeAndNULL(data);
	}
}

/*PMatrix getTKM(PConnection pc)
{
	sqlite3_stmt* stmtCount, * stmtGet;
	int count;
	int i;
	const wchar_t* name;
	PMatrix result;

	result = (PArray)calloc(1, sizeof(TArray));
	if (result)
	{
		count = 0;
		sqlite3_exec(pc->db, BEGIN_TRANSACTION_QUERY, NULL, 0, NULL);
		if (sqlite3_prepare_v2(pc->db, COUNT_ACCOUNTS_QUERY, -1, &stmtCount, NULL) == SQLITE_OK)
		{
			if (sqlite3_step(stmtCount) == SQLITE_ROW)
			{
				count = sqlite3_column_int(stmtCount, 0);
				result->count = count;
				result->data = (PDatum)calloc(count, sizeof(TDatum));

				if (result->data != NULL) {
					if (sqlite3_prepare_v2(pc->db, GET_SUM_BY_DRIVER_QUERY, -1, &stmtGet, NULL) == SQLITE_OK)
					{
						i = 0;
						while (i < count)
						{
							if (sqlite3_step(stmtGet) == SQLITE_ROW)
							{
								((PDatum)(result->data))[i].id = sqlite3_column_int(stmtGet, 0);
								((PDatum)(result->data))[i].sum = sqlite3_column_int(stmtGet, 1);
							}
							i++;
						}
					}
					sqlite3_finalize(stmtGet);
				}
				else
				{
					freeAndNULL(result);
					result = NULL;
				}
			}
		}
		sqlite3_finalize(stmtCount);
		sqlite3_exec(pc->db, COMMIT_QUERY, NULL, 0, NULL);
	}
	return result;
}
*/
PWaybill getWaybill(PConnection pc, int driverID, int dayID)
{
	sqlite3_stmt* stmtGet;
	int count;
	int i;
	const wchar_t* name;
	PWaybill result;
	result = NULL;

	if (sqlite3_prepare_v2(pc->db, GET_WAYBILL_QUERY, AUTOLENGTH, &stmtGet, NULL) == SQLITE_OK)
	{
		sqlite3_bind_int(stmtGet, sqlite3_bind_parameter_index(stmtGet, DRIVER_ID_NAME), driverID);
		sqlite3_bind_int(stmtGet, sqlite3_bind_parameter_index(stmtGet, DATE_NAME), dayID);
		if (sqlite3_step(stmtGet) == SQLITE_ROW)
		{
			result = (PWaybill)calloc(1, sizeof(TWaybill));
			if (result)
			{
				result->id = sqlite3_column_int(stmtGet, 0);
				result->driverID = sqlite3_column_int(stmtGet, 1);
				result->dateID = sqlite3_column_int(stmtGet, 2);
				result->number = sqlite3_column_int(stmtGet, 3);
				result->carID = sqlite3_column_int(stmtGet, 4);
			}
		}
	}
	sqlite3_finalize(stmtGet);
	return result;
}

PArray getWaybills(PConnection pc)
{

}

int deleteFromTable(PConnection pc, enum tableType type, int id)
{
	const char* sql;
	sqlite3_stmt* stmt;
	int result;
	sql = "";
	result = 0;
	if (pc)
	{
		switch (type)
		{
		case tDriver:
			sql = DELETE_DRIVER_QUERY;
			break;
		case tCar:
			sql = DELETE_CAR_QUERY;
			break;
		case tWaybill:
			sql = DELETE_WAYBILL_QUERY;
			break;
		case tTKM:
			sql = DELETE_TKM_QUERY;
			break;
		case tAccount:
			sql = DELETE_ACCOUNT_QUERY;
			break;
		}
		if (sqlite3_prepare_v2(pc->db, sql, -1, &stmt, NULL) == SQLITE_OK)
		{
			sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ID_NAME), id);
			result = sqlite3_step(stmt) == SQLITE_DONE;
		}
		sqlite3_finalize(stmt);
	}
	return result;
}