#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

typedef struct _connection
{
	sqlite3* db;
} connection, * pconnection;

enum tableType {Driver, Car, Waybill, Account, Subaccount, TKM};

typedef struct _driver
{
	int id;
	char* name;
} driver, *pdriver;
typedef struct _waybill
{
	int id;
	int driverID;
	char* date;
	int number;
	int carID;
} waybill, *pwaybill;
typedef struct _account
{
	int id;
	char* account;
} account, *paccount;
typedef struct _subaccount
{
	int id;
	int accountID;
	char* subaccount;
} subaccount, *psubaccount;
typedef struct _TKM
{
	int id;
	int waybillID;
	int subaccountID;
	int amount;
} tkm, *ptkm;
typedef struct _car
{
	int id;
	char* number;
} car, *pcar;

pconnection openDB(const char* filename);
pconnection createDB(const char* filename);
void closeDB(pconnection pc);

int addDriver(pconnection pc, char* name);
int addCar(pconnection pc, char * number);
int addWaybill(pconnection pc, int driverID, char * date, int number, int carID);
int addAccount(pconnection pc, char* account);
int addSubaccount(pconnection pc, int accountID, char* subaccount);
int addTKM(pconnection pc, int waybillID, int subaccountID, int amount);

int deleteFromTable(pconnection pc, enum tableType type, int id);


#endif