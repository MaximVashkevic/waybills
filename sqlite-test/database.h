#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite/sqlite3.h"

typedef struct _connection
{
	sqlite3* db;
} connection, * pconnection;

enum tableType {Driver, Car, Waybill, Account, Subaccount, TKM};

typedef struct _driver
{
	int id;
	wchar_t* name;
} driver, *pdriver;
typedef struct _waybill
{
	int id;
	int driverID;
	wchar_t* date;
	int number;
	int carID;
} waybill, *pwaybill;
typedef struct _account
{
	int id;
	wchar_t* account;
} account, *paccount;
typedef struct _subaccount
{
	int id;
	int accountID;
	wchar_t* subaccount;
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
	wchar_t* number;
} car, *pcar;

typedef struct _data
{
	int sum;
	int id;
} data, *pdata;

pconnection openDB(const wchar_t* filename);
pconnection createDB(const wchar_t* filename);
void closeDB(pconnection pc);

int addDriver(pconnection pc, wchar_t* name);
int addCar(pconnection pc, wchar_t* number);
int addWaybill(pconnection pc, int driverID, wchar_t* date, int number, int carID);
int addAccount(pconnection pc, wchar_t* account);
int addSubaccount(pconnection pc, int accountID, wchar_t* subaccount);
int addTKM(pconnection pc, int waybillID, int subaccountID, int amount);

int getDrivers(pconnection pc, pdriver* drivers);
void freeDrivers(pdriver drivers, int num);

int getAccounts(pconnection pc, paccount* accounts);
void freeAccounts(paccount accounts, int num);

int getSubaccounts(pconnection pc, int accountID, psubaccount* subaccounts);
void freeSubaccounts(psubaccount subaccounts, int num);

int getSumBySubaccount(pconnection pc, int subaccountID, pdata* data);
int getSumByAccount(pconnection pc, int accountID, pdata* data);
int getSumByDriver(pconnection pc, int driverID, pdata* data);
void freeData(pdata data);



int deleteFromTable(pconnection pc, enum tableType type, int id);


#endif