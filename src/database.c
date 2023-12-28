#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "/home/ghaster/Proiecte/Project_2/include/database.h"


void createUsersTable(sqlite3 *db){
    const char *sql = "CREATE TABLE IF NOT EXISTS Users ("
                      "UserID INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "Username TEXT NOT NULL,"
                      "Password TEXT NOT NULL)";
    int result = sqlite3_exec(db, sql, 0, 0, 0);
    if (result != SQLITE_OK) {
        fprintf(stderr, "Error creating Users table: %s\n", sqlite3_errmsg(db));
    }
}

void createCustomersTable(sqlite3 *db){
    const char *sql = "CREATE TABLE IF NOT EXISTS Customers ("
                      "CustomerID INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "UserID INTEGER NOT NULL,"
                      "FirstName TEXT NOT NULL,"
                      "LastName TEXT NOT NULL,"
                      "Address TEXT NOT NULL,"
                      "Email TEXT NOT NULL,"
                      "Phone TEXT NOT NULL,"
                      "FOREIGN KEY (UserID) REFERENCES Users(UserID))";
    int result = sqlite3_exec(db, sql, 0, 0, 0);
    if (result != SQLITE_OK) {
        fprintf(stderr, "Error creating Customers table: %s\n", sqlite3_errmsg(db));
    }
}

void createFinancialTransactionsTable(sqlite3 *db){
    const char *sql = "CREATE TABLE IF NOT EXISTS FinancialTransactions ("
                      "TransactionID INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "UserID INTEGER NOT NULL,"
                      "CustomerID INTEGER NOT NULL,"
                      "TransactionType TEXT NOT NULL,"
                      "Amount REAL NOT NULL,"
                      "Date TEXT NOT NULL,"
                      "FOREIGN KEY (UserID) REFERENCES Users(UserID),"
                      "FOREIGN KEY (CustomerID) REFERENCES Customers(CustomerID))";
    int result = sqlite3_exec(db, sql, 0, 0, 0);
    if (result != SQLITE_OK) {
        fprintf(stderr, "Error creating FinancialTransactions table: %s\n", sqlite3_errmsg(db));
    }

}


void DisplayData(sqlite3 *db)
{
    sqlite3_stmt *stmt;
    g_print("entering display data\n");
    int rc = sqlite3_prepare_v2(db, "SELECT * FROM Users", -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        g_print("%d %s %s\n", sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1), sqlite3_column_text(stmt, 2));
    }

    sqlite3_finalize(stmt);
    
}


void insertSampleData(sqlite3 *db){
    const char *sql = "INSERT INTO Users (Username, Password) VALUES ('admin', '@dm1n')";
    sqlite3_exec(db, sql, 0, 0, 0);
    sql = "INSERT INTO Customers (UserID, FirstName, LastName, Address, Email, Phone) VALUES (1, 'adminul', 'adminescu', 'Piata Marasti, nr 13', 'admin@gmail.com', '0743217968')";
    sqlite3_exec(db, sql, 0, 0, 0);
    sql = "INSERT INTO FinancialTransactions (UserID, CustomerID, TransactionType, Amount, Date) VALUES (1, 1, 'Deposit', 1000, '2021-05-05')";
    sqlite3_exec(db, sql, 0, 0, 0);
}

void sqlInit(){
    sqlite3 *db;
    int rc = sqlite3_open("banking_app_database.db", &db);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    createUsersTable(db);
    createCustomersTable(db);
    createFinancialTransactionsTable(db);

    // insertSampleData(db);

    // Commit the transaction
    sqlite3_exec(db, "COMMIT", 0, 0, 0);

    DisplayData(db);

    sqlite3_close(db);
}