#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

typedef struct _connection
{
	sqlite3* db;
} connection, * pconnection;

pconnection openDB(const char* filename);
pconnection createDB(const char* filename);
void closeDB(pconnection pc);

int addDriver(pconnection pc, char* name);
int addCar(pconnection pc, char * number);
int addWaybill(pconnection pc, int driverID, int date, int number, int carID);
int addAccount(pconnection pc, char* account);
int addSubaccount(pconnection pc, int accountID, char* subaccount);
int addTKM(pconnection pc, int waybillID, int subaccountID, int amount);

int deleteDriver(pconnection pc, int id);
int deleteCar(pconnection pc, int id);
int deleteWaybill(pconnection pc, int id);
int deleteAccount(pconnection pc, int id);
int deleteSubaccount(pconnection pc, int id);
int deleteTKM(pconnection pc, int id);



#endif