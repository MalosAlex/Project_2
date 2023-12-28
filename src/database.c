#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "/home/ghaster/Proiecte/Project_2/include/database.h"


void createUsersTable(){
    const char *sql = "CREATE TABLE IF NOT EXISTS Users ("
                      "UserID INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "Username TEXT NOT NULL,"
                      "Password TEXT NOT NULL,"
    executeSQL(sql);
}

void createCustomersTable(){
    const char *sql = "CREATE TABLE IF NOT EXISTS Customers ("
                      "CustomerID INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "UserID INTEGER NOT NULL,"
                      "FirstName TEXT NOT NULL,"
                      "LastName TEXT NOT NULL,"
                      "Address TEXT NOT NULL,"
                      "Email TEXT NOT NULL,"
                      "Phone TEXT NOT NULL,"
                      "FOREIGN KEY (UserID) REFERENCES Users(UserID))"
    executeSQL(sql);
}

void createFinancialTransactionsTable(){
    const char *sql = "CREATE TABLE IF NOT EXISTS Customers ("
                      "TransactionID INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "UserID INTEGER NOT NULL,"
                      "CustomerID INTEGER NOT NULL,"
                      "TransactionType TEXT NOT NULL,"
                      "Amount REAL NOT NULL,"
                      "Date TEXT NOT NULL,"
                      "FOREIGN KEY (UserID) REFERENCES Users(UserID),"
                      "FOREIGN KEY (CustomerID) REFERENCES Customers(CustomerID))"
    executeSQL(sql);
}


void DisplayData()
{
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, "SELECT * FROM Users", -1, &stmt, NULL);
    while((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        printf("%d %s %s\n", sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1), sqlite3_column_text(stmt, 2));
    }
    sqlite_finalize(stmt);
}

void sqlInit(){
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("banking_app_database.db", &db);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    createUsersTable();
    createCustomersTable();
    createFinancialTransactionsTable();
    insertSampleData();
    DisplayData();
    sqlite3_close(db);
}