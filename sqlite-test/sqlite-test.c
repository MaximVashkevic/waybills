#include <stdio.h>
#include <sqlite3.h>
#include "database.h"

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}
//
//int main(int argc, char** argv) {
//	sqlite3* db;
//	char* zErrMsg = 0;
//	int rc;
//
//	rc = sqlite3_open("test.db", &db);
//	if (rc) {
//		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
//		sqlite3_close(db);
//		return 1;
//	}
//	rc = sqlite3_exec(db, "SELECT SQLITE_VERSION()", callback, 0, &zErrMsg);
//	if (rc != SQLITE_OK) {
//		fprintf(stderr, "SQL error: %s\n", zErrMsg);
//		sqlite3_free(zErrMsg);
//	}
//	sqlite3_close(db);
//	return 0;
//
//}

int main(void) {

	//sqlite3* db;
	//char* err_msg = 0;

	//int rc = sqlite3_open("test.db", &db);

	//if (rc != SQLITE_OK) {
	//	fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
	//	sqlite3_close(db);
	//	return 1;
	//}

	//char* sql = 
	//	"BEGIN TRANSACTION;\
	//	CREATE TABLE Drivers(\
	//		ID INTEGER PRIMARY KEY NOT NULL,\
	//		Name TEXT NOT NULL\
	//	);\
	//	CREATE TABLE Waybills(\
	//		ID INTEGER PRIMARY KEY NOT NULL,\
	//		DriverID INTEGER NOT NULL,\
	//		Date NUMERIC NOT NULL,\
	//		Number INTEGER NOT NULL,\
	//		Car TEXT NOT NULL,\
	//		FOREIGN KEY(DriverID) REFERENCES Drivers(ID)\
	//	);\
	//	CREATE TABLE Accounts(\
	//		ID INTEGER PRIMARY KEY NOT NULL,\
	//		Account TEXT NOT NULL\
	//	);\
	//	CREATE TABLE Subaccounts(\
	//		ID INTEGER PRIMARY KEY NOT NULL,\
	//		AccountID INTEGER NOT NULL,\
	//		Subaccount TEXT NOT NULL,\
	//		FOREIGN KEY(AccountID) REFERENCES Accounts(ID)\
	//	);\
	//	CREATE TABLE TKM(\
	//		ID INTEGER PRIMARY KEY NOT NULL,\
	//		WaybillID INTEGER NOT NULL,\
	//		SubaccountID INTEGER NOT NULL,\
	//		Amount INTEGER NOT NULL CHECK(Amount >= 0),\
	//		FOREIGN KEY(SubaccountID) REFERENCES Subaccounts(ID),\
	//		FOREIGN KEY(WaybillID) REFERENCES Waybills(ID)\
	//	);\
	//	COMMIT;";


	//rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

	//if (rc != SQLITE_OK) {

	//	fprintf(stderr, "SQL error 1: %s\n", err_msg);

	//	sqlite3_free(err_msg);
	//	sqlite3_exec(db, "ROLLBACK;", 0, 0, &err_msg);
	//	//sqlite3_close(db);
	//	
	//}
	//sql =
	//	/*"BEGIN TRANSACTION;\
	//	INSERT INTO drivers(Name) VALUES ('Bob');\
	//	INSERT INTO drivers(Name) VALUES('Alice');\
	//	COMMIT;";*/
	//	"select * from asd;";
	//rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	//if (rc != SQLITE_OK) {

	//	fprintf(stderr, "SQL error 2: %s\n", err_msg);

	//	sqlite3_free(err_msg);
	//	sqlite3_close(db);

	//	return 1;
	//}
	//else {
	//	sqlite3_exec(db, "Select * from drivers;", callback, 0, &err_msg);
	//}
	//sqlite3_close(db);
	pconnection pc = createDB("myfile.b");
	if (pc)
	{
		closeDB(pc);
	}
	return 0;
}


//int main(void) {
//
//	sqlite3* db;
//	sqlite3_stmt* res;
//
//	int rc = sqlite3_open(":memory:", &db);
//
//	if (rc != SQLITE_OK) {
//
//		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
//		sqlite3_close(db);
//
//		return 1;
//	}
//
//	rc = sqlite3_prepare_v2(db, "SELECT SQLITE_VERSION()", -1, &res, 0);
//
//	if (rc != SQLITE_OK) {
//
//		fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
//		sqlite3_close(db);
//
//		return 1;
//	}
//
//	rc = sqlite3_step(res);
//
//	if (rc == SQLITE_ROW) {
//		printf("%s\n", sqlite3_column_text(res, 0));
//	}
//
//	sqlite3_finalize(res);
//	sqlite3_close(db);
//
//	return 0;
//}