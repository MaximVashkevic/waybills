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
typedef struct _waybill
{
	int id;
	int driverID;
	int dateID;
	int number;
	int carID;
} TWaybill, *PWaybill;
typedef struct _account
{
	int id;
	wchar_t* name;
} TAccount, *PAccount;
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
typedef struct _datum
{
	int sum;
	int id;
} TDatum, *PDatum;
typedef struct _array
{
	void * data;
	int count;
} TArray, *PArray;
typedef struct _matrix
{
	PArray* matrix;
	int count;
} TMatrix, *PMatrix;
typedef struct _date
{
	int id;
	int day;
	wchar_t* date;
} TDate, *PDate;
//typedef struct 

PConnection openDB(const wchar_t* filename);
PConnection createDB(const wchar_t* filename);
void closeDB(PConnection pc);

void insertDates(PConnection pc);
int addDriver(PConnection pc, wchar_t* name);
int addCar(PConnection pc, wchar_t* number);
int addWaybill(PConnection pc, int driverID, int dateID, int number, int carID);
int addAccount(PConnection pc, wchar_t* TAccount);
int addTKM(PConnection pc, int waybillID, int subaccountID, int amount);

PArray getDrivers(PConnection pc);
void freeDrivers(PArray drivers);

PArray getAccounts(PConnection pc);
void freeAccounts(PArray accounts);

PArray getCars(PConnection pc);
void freeCars(PArray cars);

PArray getDays(PConnection pc);
void freeDays(PArray days);
PArray getSumByDriver(PConnection pc, int driverID);
int getTotalSumByDriver(PConnection pc, int driverID);
int getTotalSum(PConnection pc);
void freeData(PArray data);

PMatrix getTKM(PConnection pc);
void freeTKM(PConnection pc, PMatrix tkm);

PWaybill getWaybill(PConnection pc, int driverID, int dayID);
void freeWaybills(PC);

int deleteFromTable(PConnection pc, enum tableType type, int id);


#endif