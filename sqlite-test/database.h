#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite/sqlite3.h"

typedef struct _connection
{
	sqlite3* db;
} TConnection, * PConnection;

enum tableType {tDriver, tCar, tWaybill, tAccount, tTKM};

typedef struct _driver
{
	int id;
	wchar_t* name;
} TDriver, *PDriver;

typedef struct _drivers
{
	PDriver data;
	int count;
}TDrivers, *PDrivers;
typedef struct _waybill
{
	int id;
	int driverID;
	wchar_t* date;
	int number;
	int carID;
} TWaybill, *PWaybill;
typedef struct _account
{
	int id;
	wchar_t* name;
} TAccount, *PAccount;
typedef struct _accounts
{
	PAccount data;
	int count;
} TAccounts, * PAccounts;

typedef struct _TKM
{
	int id;
	int waybillID;
	int subaccountID;
	int amount;
} TTKM, *PTKM;
typedef struct _car
{
	int id;
	wchar_t* number;
} TCar, *PCar;
typedef struct _cars
{
	PCar data;
	int count;
} TCars, *PCars;
typedef struct _datum
{
	int sum;
	int id;
} TDatum, *PDatum;
typedef struct _data
{
	PDatum data;
	int count;
} TData, *PData;

PConnection openDB(const wchar_t* filename);
PConnection createDB(const wchar_t* filename);
void closeDB(PConnection pc);

int addDriver(PConnection pc, wchar_t* name);
int addCar(PConnection pc, wchar_t* number);
int addWaybill(PConnection pc, int driverID, wchar_t* date, int number, int carID);
int addAccount(PConnection pc, wchar_t* TAccount);
int addTKM(PConnection pc, int waybillID, int subaccountID, int amount);

PDrivers getDrivers(PConnection pc);
void freeDrivers(PDrivers drivers);

PAccounts getAccounts(PConnection pc);
void freeAccounts(PAccounts accounts);

PCars getCars(PConnection pc);
void freeCars(PCars cars);


PData* getSumByDriver(PConnection pc, int driverID);
void freeData(PData data);



int deleteFromTable(PConnection pc, enum tableType type, int id);


#endif