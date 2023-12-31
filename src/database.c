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
                      "FirstName TEXT NOT NULL,"
                      "LastName TEXT NOT NULL,"
                      "Address TEXT NOT NULL,"
                      "Email TEXT NOT NULL,"
                      "Phone TEXT NOT NULL)";         
    int result = sqlite3_exec(db, sql, 0, 0, 0);
    if (result != SQLITE_OK) {
        fprintf(stderr, "Error creating Customers table: %s\n", sqlite3_errmsg(db));
    }
}

void createFinancialTransactionsTable(sqlite3 *db){
    const char *sql = "CREATE TABLE IF NOT EXISTS FinancialTransactions ("
                      "TransactionID INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "UserID INTEGER NOT NULL,"
                      "CustomerID INTEGER ,"  // NULLABLE since we can have transactions without a customer
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

void createAccountTable(sqlite3 *db)
{
    const char *sql = "CREATE TABLE IF NOT EXISTS Accounts ("
                      "AccountID INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "UserID INTEGER NOT NULL,"
                      "AccountType TEXT NOT NULL,"
                      "Balance REAL NOT NULL,"
                      "FOREIGN KEY (UserID) REFERENCES Users(UserID))";
    int result = sqlite3_exec(db, sql, 0, 0, 0);
    if (result != SQLITE_OK)
    {
        fprintf(stderr, "Error creating Accounts table: %s\n", sqlite3_errmsg(db));
    }

}

void createActivityLogTable(sqlite3 *db)
{
    const char *sql = "CREATE TABLE IF NOT EXISTS ActivityLog ("
                      "LogID INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "AccountID INTEGER NOT NULL,"
                      "ActivityType TEXT NOT NULL,"
                      "Amount REAL,"
                      "Date TEXT NOT NULL,"
                      "FOREIGN KEY (AccountID) REFERENCES Accounts(AccountID))";
    int result = sqlite3_exec(db, sql, 0, 0, 0);
    if (result != SQLITE_OK)
    {
        fprintf(stderr, "Error creating ActivityLog table: %s\n", sqlite3_errmsg(db));
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
    // const char *sql = "INSERT INTO Users (Username, Password) VALUES ('admin', '@dm1n')";
    // sqlite3_exec(db, sql, 0, 0, 0);
    // const char *sql = "INSERT INTO Customers (FirstName, LastName, Address, Email, Phone) VALUES ('adminul', 'adminescu', 'Piata Marasti, nr 13', 'admin@gmail.com', '0743217968')";
    // sqlite3_exec(db, sql, 0, 0, 0);
    // const char *sql = "INSERT INTO FinancialTransactions (UserID, CustomerID, TransactionType, Amount, Date) VALUES (5, 1, 'Deposit', 1000, '2021-05-05')";
    // sqlite3_exec(db, sql, 0, 0, 0);
    // const char *sql = "INSERT INTO Accounts (UserID, AccountType, Balance) VALUES (5, 'Savings', 1000)";
    // sqlite3_exec(db, sql, 0, 0, 0);
    // sql = "INSERT INTO Accounts (UserID, AccountType, Balance) VALUES (5, 'Checking', 2000)";
    // sqlite3_exec(db, sql, 0, 0, 0);
    const char *sql = "INSERT INTO ActivityLog (AccountID, ActivityType, Amount, Date) VALUES (14, 'Deposit', 500, '2021-05-05')";
    sqlite3_exec(db, sql, 0, 0, 0);
}

void insert_user(const char *username, const char *password){
    sqlite3 *db;
    int rc = sqlite3_open("banking_app_database.db", &db);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    const char *sql = "INSERT INTO Users (Username, Password) VALUES (?, ?)";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement\n");
        return;
    }
    // Bind parameters
    if (sqlite3_bind_text(stmt, 1, username, -1, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 1\n");
        return;
    }
    if (sqlite3_bind_text(stmt, 2, password, -1, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 2\n");
        return;
    }
    // Execute the statement
    int result = sqlite3_step(stmt);
    if (result != SQLITE_DONE)
    {
        fprintf(stderr, "Error inserting new user into Users table\n");
        return;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
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
    createAccountTable(db);
    createActivityLogTable(db);

    //insertSampleData(db);

    // Commit the transaction
    sqlite3_exec(db, "COMMIT", 0, 0, 0);

    DisplayData(db);

    sqlite3_close(db);
}