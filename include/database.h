#ifndef DATABASE_H
#define DATABASE_H

void createUsersTable();
void createCustomersTable();
void createFinancialTransactionsTable();
void DisplayData();
void insertSampleData();
void insert_user(const char *username, const char *password);
void sqlInit();

#endif // DATABASE_H



