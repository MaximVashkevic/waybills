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

	sqlite3* db;
	char* err_msg = 0;
	sqlite3_stmt* stmt;

	int rc = sqlite3_open("test.db", &db);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return 1;
	}

	const char* const sql = "SELECT * FROM drivers WHERE id = ?1;";
	if (!sqlite3_prepare_v2(db, sql, -1, &stmt, NULL))
	{
		sqlite3_bind_int(stmt, 1, 3);
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			printf("%i %s\n", sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1));
		}
		sqlite3_finalize(stmt);

	}
	const char* const sql1 = "SELECT * FROM drivers WHERE id = :id;";
	if (!sqlite3_prepare_v2(db, sql1, -1, &stmt, NULL))
	{
		int i = sqlite3_bind_parameter_index(stmt, ":id");
		sqlite3_bind_int(stmt, i, 3);
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			printf("%i %s\n", sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1));
		}
		sqlite3_finalize(stmt);

	}


	sqlite3_close(db);
	pconnection pc = createDB("t2.db");
	if (pc)
	{
		addDriver(pc, "Bob");
		addCar(pc, "8234");
		addAccount(pc, "12//7");
		addSubaccount(pc, 1, "молоко");
		addDriver(pc, "Alice");
		addCar(pc, "9921");
		addAccount(pc, "89//7");
		addSubaccount(pc, 2, "рис");
		addWaybill(pc, 1, 1221312, 1243, 1);
		addWaybill(pc, 2, 1231231, 1423, 2);

		addTKM(pc, 1, 1, 10);
		addTKM(pc, 2, 2, 17);

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