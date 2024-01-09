// Path: src/functions.c
#include "/home/ghaster/Proiecte/Project_2/include/functions.h"
#include "/home/ghaster/Proiecte/Project_2/include/gui.h"  
#include "/home/ghaster/Proiecte/Project_2/include/database.h"
#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
#include <sqlite3.h>
#include <time.h>

void on_login_exit_clicked(GtkButton *button, gpointer user_data)
{
    /*
    Close the login window and the application
    */
    gtk_widget_hide(GTK_WIDGET(login_window));
    gtk_main_quit();
}

void validate_login(GtkEntry *entry, gpointer user_data)
{
    /*
    Check if the username and password entries are valid
    */
    gboolean v1 = check_username(login_username_entry, NULL);
    gboolean v2 = check_password(login_password_entry, NULL);
    if (v1 == TRUE && v2 == TRUE)
    {
        gtk_widget_set_sensitive(GTK_WIDGET(login_button), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(register_button), TRUE);
    }
    else
    {
        gtk_widget_set_sensitive(GTK_WIDGET(login_button), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(register_button), FALSE);
    }
}

gboolean check_username(GtkEntry *entry, gpointer user_data)
{
    /*
    Check if the username entry is valid
    */
    const char *username = gtk_entry_get_text(entry);

    if (strlen(username) < 5)
    {
        gtk_label_set_text(login_error_username, "Username must be at least 5 characters long");
        return FALSE;
    }
    else if (strlen(username) > 20)
    {
        gtk_label_set_text(login_error_username, "Username must be at most 20 characters long");
        return FALSE;
    }
    else if (strchr(username, ' ') != NULL)
    {
        gtk_label_set_text(login_error_username, "Username must not contain spaces");
        return FALSE;
    }
    else
    {
        gtk_label_set_markup(GTK_LABEL(login_error_username), "<span font_desc='30'><b>✔</b></span>");
        return TRUE;
    }
    //TODO check if username is already taken, after implementing the database
}

gboolean check_password(GtkEntry *entry, gpointer user_data)
{
    /*
    Check if the password is correct, containing at least 5 characters, at most 20 characters, no spaces, 
    only printable characters, at least one special character and at least one digit
    */
    const char *password = gtk_entry_get_text(entry);
    const char *sp_chr = "@!#$^&*()-_=+[{]}|;:,<.>/?";
    const char *nr = "0123456789";
    if (strlen(password) < 5)
    {
        gtk_label_set_text(login_error_password, "Password must be at least 5 characters long");
        return FALSE;
    }
    else if (strlen(password) > 20)
    {
        gtk_label_set_text(login_error_password, "Password must be at most 20 characters long");
        return FALSE;
    }
    else if (strchr(password, ' ') != NULL)
    {
        gtk_label_set_text(login_error_password, "Password must not contain spaces");
        return FALSE;
    }
    int ok = 0;
    int n = 0;
    for (int i = 0; i < strlen(password); i++)
    {
        if (password[i] < 33 || password[i] > 126)
        {
            gtk_label_set_text(login_error_password, "Password must contain only printable characters");
            return FALSE;
        }
        for (int j = 0; j < strlen(sp_chr); j++)
        {
            if (password[i] == sp_chr[j])
            {
                ok = 1;
                break;
            }
        }
        for (int j = 0; j < strlen(nr); j++)
        {
            if (password[i] == nr[j])
            {
                n = 1;
                break;
            }
        }
    }
    if (ok == 0)
    {
        gtk_label_set_text(login_error_password, "Password must contain at least one special character");
        return FALSE;
    }
    else if (n == 0)
    {
        gtk_label_set_text(login_error_password, "Password must contain at least one digit");
        return FALSE;
    }
    else
    {
        gtk_label_set_markup(GTK_LABEL(login_error_password), "<span font_desc='30'><b>✔</b></span>");
        return TRUE;
    }
}

gboolean check_login(GtkButton *button, gpointer user_data)
{
    /* 
    We check if the username and password are correct and if they are we hide the login window and show the main menu
    */
    sqlite3 *db;
    int rc = sqlite3_open("banking_app_database.db", &db);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_stmt *stmt;
    const char *username = gtk_entry_get_text(login_username_entry);
    const char *password = gtk_entry_get_text(login_password_entry);
    const char *sql = "SELECT * FROM Users WHERE Username = ? AND Password = ?";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement\n");
        return FALSE;
    }

    // Bind parameters
    if (sqlite3_bind_text(stmt, 1, username, -1, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 1\n");
        return FALSE;
    }
    if (sqlite3_bind_text(stmt, 2, password, -1, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 2\n");
        return FALSE;
    }

    // Execute the statement
    int result = sqlite3_step(stmt);

    if (result == SQLITE_ROW)
    {
        // Get the user ID
        user_id = sqlite3_column_int(stmt, 0);
        // There is a row, so finalize the statement
        sqlite3_finalize(stmt);
        // Close the database
        sqlite3_close(db);
        // Clear the username and password entries and the errors
        gtk_entry_set_text(login_username_entry, "");
        gtk_entry_set_text(login_password_entry, "");
        gtk_label_set_text(login_error_username, "");
        gtk_label_set_text(login_error_password, "");
        // Hide the login window
        gtk_widget_hide(GTK_WIDGET(login_window));
        // Call the main menu
        gtk_widget_show_all(GTK_WIDGET(main_window));
        return TRUE;
    }
    else
    {
        // There is no row, so finalize the statement
        sqlite3_finalize(stmt);
        // Close the database
        sqlite3_close(db);
        // Display an error message
        gtk_label_set_text(login_error_username, "Username or password incorrect");
        gtk_label_set_text(login_error_password, "Username or password incorrect");
        return FALSE;
    }

}

gboolean check_register(GtkButton *button, gpointer user_data)
{
    /*
    We check if the username is correct and its new and if it is we insert the new user into the database
    */
    sqlite3 *db;
    int rc = sqlite3_open("banking_app_database.db", &db);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_stmt *stmt;
    const char *username = gtk_entry_get_text(login_username_entry);
    const char *password = gtk_entry_get_text(login_password_entry);
    const char *sql = "SELECT * FROM Users WHERE Username = ?";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement\n");
        return FALSE;
    }

    // Bind parameters
    if (sqlite3_bind_text(stmt, 1, username, -1, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 1\n");
        return FALSE;
    }

    // Execute the statement
    int result = sqlite3_step(stmt);

    if (result == SQLITE_ROW)
    {
        // There is a row, so finalize the statement
        sqlite3_finalize(stmt);
        // Close the database
        sqlite3_close(db);
        // Display an error message
        gtk_label_set_text(login_error_username, "Username already taken");
        return FALSE;
    }
    else
    {
        // There is no row, so finalize the statement
        sqlite3_finalize(stmt);
        // Close the database
        sqlite3_close(db);
        // Insert the new user into the database
        insert_user(username, password);
        // Display a success message
        gtk_label_set_text(login_error_username, "Account created successfully");
        gtk_label_set_text(login_error_password, "Account created successfully");
        // Empty the username and password entries and the errors
        gtk_entry_set_text(login_username_entry, "");
        gtk_entry_set_text(login_password_entry, "");
        gtk_label_set_text(login_error_username, "");
        gtk_label_set_text(login_error_password, "");
        return TRUE;
    }
}

void sign_out(GtkMenuItem *menuitem, gpointer user_data)
{
    /*
    Exit the main window and go back to the login widget
    */
    user_id = -1;
    gtk_widget_hide(GTK_WIDGET(main_window));
    gtk_widget_show_all(GTK_WIDGET(login_window));
}

void quit_main_menu(GtkMenuItem *menuitem, gpointer user_data)
{
    /*
    Quit the main menu and the application
    */
    gtk_widget_destroy(GTK_WIDGET(main_window));
    gtk_main_quit();
}

void view_accounts(GtkMenuItem *menuitem, gpointer user_data)
{
    /*
    Shows a dialog with all the user's accounts
    */
    if (view_text == NULL || view_dialog == NULL) {
        fprintf(stderr, "Invalid GTK widgets\n");
        return;
    }

    sqlite3 *db;
    int rc = sqlite3_open("banking_app_database.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(view_text);

    sqlite3_stmt *stmt;
    const char *sql = "SELECT AccountID, AccountType, Balance FROM Accounts WHERE UserID = ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement\n");
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    if (sqlite3_bind_int(stmt, 1, user_id) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 1\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // Execute the statement
    int result;
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int col_count = sqlite3_column_count(stmt);
        for (int i = 0; i < col_count; i++)
        {
            const char *column_name = (const char *)sqlite3_column_name(stmt, i);
            const char *column_value = (const char *)sqlite3_column_text(stmt, i);

            char column_info[256];
            snprintf(column_info, sizeof(column_info), "%s: %s | ", column_name, column_value);
            gtk_text_buffer_insert_at_cursor(buffer, column_info, -1);
        }
        gtk_text_buffer_insert_at_cursor(buffer, "\n", -1);
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
    // Close the database
    sqlite3_close(db);

    // Put the text buffer in the text view
    gtk_text_view_set_buffer(view_text, buffer);
    // Show the dialog
    gtk_dialog_run(view_dialog);
}

void view_balance(GtkMenuItem *menuitem, gpointer user_data)
{
    /*
    Opens a dialog that shows every account's balance followed by the user's total balance
    */
    if (view_text == NULL || view_dialog == NULL)
    {
        fprintf(stderr, "Invalid GTK widgets\n");
        return;
    }

    sqlite3 *db;
    int rc = sqlite3_open("banking_app_database.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(view_text);

    // TODO Remember when inserting data to update the balance as well
    sqlite3_stmt *stmt;
    const char *sql = "SELECT AccountID, Balance FROM Accounts WHERE UserID = ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement\n");
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    if (sqlite3_bind_int(stmt, 1, user_id) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 1\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // Execute the statement
    int result;
    int gen_balance = 0;
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int col_count = sqlite3_column_count(stmt);
        for (int i = 0; i < col_count; i++)
        {
            const char *column_name = (const char *)sqlite3_column_name(stmt, i);
            const char *column_value = (const char *)sqlite3_column_text(stmt, i);

            if (strcmp(column_name, "Balance") == 0)
            {
                gen_balance += atoi(column_value);
            }
            char column_info[256];
            snprintf(column_info, sizeof(column_info), "%s: %s | ", column_name, column_value);
            gtk_text_buffer_insert_at_cursor(buffer, column_info, -1);
        }
        gtk_text_buffer_insert_at_cursor(buffer, "\n", -1);
    }
    char column_info[256];
    snprintf(column_info, sizeof(column_info), "General balance: %d", gen_balance);
    gtk_text_buffer_insert_at_cursor(buffer, column_info, -1);
    gtk_text_buffer_insert_at_cursor(buffer, "\n", -1);

    // Finalize the statement
    sqlite3_finalize(stmt);
    // Close the database
    sqlite3_close(db);

    // Put the text buffer in the text view
    gtk_text_view_set_buffer(view_text, buffer);
    // Show the dialog
    gtk_dialog_run(view_dialog);
}

void view_transactions(GtkMenuItem *menuitem, gpointer user_data)
{
    /*
    Opnes a dialog that shows every transaction made by the user
    */
    if (view_text == NULL || view_dialog == NULL) {
        fprintf(stderr, "Invalid GTK widgets\n");
        return;
    }

    sqlite3 *db;
    int rc = sqlite3_open("banking_app_database.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(view_text);

    sqlite3_stmt *stmt;
    const char *sql = "SELECT AccountID FROM Accounts WHERE UserID = ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement\n");
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    if (sqlite3_bind_int(stmt, 1, user_id) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 1\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // Execute the statement
    int result;
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int col_count = sqlite3_column_count(stmt);
        for (int i = 0; i < col_count; i++)
        {
            const char *column_value = (const char *)sqlite3_column_text(stmt, i);

            char column_info[256];
            snprintf(column_info, sizeof(column_info), "\n Transactions for account %s: \n ", column_value);
            gtk_text_buffer_insert_at_cursor(buffer, column_info, -1);

            // We got all accounts, now we search for the account ID

            sqlite3_stmt *stmt2;
            const char *sql2 = "SELECT TransactionId, CustomerId, TransactionType, Amount, Date, Notes FROM FinancialTransactions WHERE AccountID = ?";
            if (sqlite3_prepare_v2(db, sql2, -1, &stmt2, NULL) != SQLITE_OK)
            {
                fprintf(stderr, "Error preparing statement\n");
                sqlite3_close(db);
                return;
            }

            // Bind parameters
            if (sqlite3_bind_int(stmt2, 1, atoi(column_value)) != SQLITE_OK)
            {
                fprintf(stderr, "Error binding parameter 1\n");
                sqlite3_finalize(stmt2);
                sqlite3_close(db);
                return;
            }

            // Execute the statement
            int result2;
            while ((result2 = sqlite3_step(stmt2)) == SQLITE_ROW)
            {
                int col_count2 = sqlite3_column_count(stmt2);
                for (int j = 0; j < col_count2; j++)
                {
                    const char *column_name2 = (const char *)sqlite3_column_name(stmt2, j);
                    const char *column_value2 = (const char *)sqlite3_column_text(stmt2, j);

                    char column_info2[256];
                    snprintf(column_info2, sizeof(column_info2), "%s: %s | ", column_name2, column_value2);
                    gtk_text_buffer_insert_at_cursor(buffer, column_info2, -1);
                }
                gtk_text_buffer_insert_at_cursor(buffer, "\n", -1);
            }

        }
        gtk_text_buffer_insert_at_cursor(buffer, "\n", -1);
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
    // Close the database
    sqlite3_close(db);

    // Put the text buffer in the text view
    gtk_text_view_set_buffer(view_text, buffer);
    // Show the dialog
    gtk_dialog_run(view_dialog);

}

void view_activity(GtkMenuItem *menuitem, gpointer user_data)   
{
    /*
    Opens a dialog that shows all the activity made by the user's accounts
    */
    if (view_text == NULL || view_dialog == NULL) {
        fprintf(stderr, "Invalid GTK widgets\n");
        return;
    }

    sqlite3 *db;
    int rc = sqlite3_open("banking_app_database.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(view_text);

    sqlite3_stmt *stmt;
    const char *sql = "SELECT AccountID FROM Accounts WHERE UserID = ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement\n");
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    if (sqlite3_bind_int(stmt, 1, user_id) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 1\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // Execute the statement
    int result;
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int col_count = sqlite3_column_count(stmt);
        for (int i = 0; i < col_count; i++)
        {
            const char *column_value = (const char *)sqlite3_column_text(stmt, i);

            char column_info[256];
            snprintf(column_info, sizeof(column_info), "\n Activity for account %s: \n ", column_value);
            gtk_text_buffer_insert_at_cursor(buffer, column_info, -1);

            // We got all accounts, now we search for the account ID

            sqlite3_stmt *stmt2;
            const char *sql2 = "SELECT Date, ActivityType, Amount FROM ActivityLog WHERE AccountID = ?";
            if (sqlite3_prepare_v2(db, sql2, -1, &stmt2, NULL) != SQLITE_OK)
            {
                fprintf(stderr, "Error preparing statement\n");
                sqlite3_close(db);
                return;
            }

            // Bind parameters
            if (sqlite3_bind_int(stmt2, 1, atoi(column_value)) != SQLITE_OK)
            {
                fprintf(stderr, "Error binding parameter 1\n");
                sqlite3_finalize(stmt2);
                sqlite3_close(db);
                return;
            }

            // Execute the statement
            int result2;
            while ((result2 = sqlite3_step(stmt2)) == SQLITE_ROW)
            {
                int col_count2 = sqlite3_column_count(stmt2);
                for (int j = 0; j < col_count2; j++)
                {
                    const char *column_name2 = (const char *)sqlite3_column_name(stmt2, j);
                    const char *column_value2 = (const char *)sqlite3_column_text(stmt2, j);

                    char column_info2[256];
                    snprintf(column_info2, sizeof(column_info2), "%s: %s | ", column_name2, column_value2);
                    gtk_text_buffer_insert_at_cursor(buffer, column_info2, -1);
                }
                gtk_text_buffer_insert_at_cursor(buffer, "\n", -1);
            }

        }
        gtk_text_buffer_insert_at_cursor(buffer, "\n", -1);
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
    // Close the database
    sqlite3_close(db);

    // Put the text buffer in the text view
    gtk_text_view_set_buffer(view_text, buffer);
    // Show the dialog
    gtk_dialog_run(view_dialog);

    // TODO Remember to update the activity log when inserting data
    // TODO Remember to make the activity log to search after account ID
}

void hide_view(GtkButton *button, gpointer user_data)
{
    /*
    hides the dialog used by all the view functions
    */
    //Clear the text view
    gtk_text_buffer_set_text(gtk_text_view_get_buffer(view_text), "", -1);
    gtk_widget_hide(GTK_WIDGET(view_dialog));
}

void manage_accounts(GtkButton *button, gpointer user_data)
{
    /*
    Opens the window used to create accounts and hides the main window
    */
    gtk_widget_hide(GTK_WIDGET(main_window));
    gtk_widget_show_all(GTK_WIDGET(create_window));
}

void delete_account(GtkButton *button, gpointer user_data)
{
    /*
    Opens the window used to delete and edit accounts and hides the main window
    */
    gtk_widget_hide(GTK_WIDGET(main_window));
    gtk_widget_show_all(GTK_WIDGET(edit_window));
}

void record_transactions(GtkButton *button, gpointer user_data)
{
    /*
    Opens the window used to record transactions and hides the main window
    */
    gtk_widget_hide(GTK_WIDGET(main_window));
    gtk_widget_show_all(GTK_WIDGET(record_window));
}

void manage_customer_data(GtkButton *button, gpointer user_data)
{
    /*
    Opens the window used to manage customer data(create and edit) and hides the main window
    */
    gtk_widget_hide(GTK_WIDGET(main_window));
    gtk_widget_show_all(GTK_WIDGET(customer_window));
}

void hide_record(GtkButton *button, gpointer user_data)
{
    /*
    Hides the record window for transactions and shows the main window
    */
    //Clear the text view and the errors
    gtk_entry_set_text(record_customer_entry, "");
    gtk_entry_set_text(record_type_entry, "");
    gtk_entry_set_text(record_amount_entry, "");
    gtk_entry_set_text(record_date_entry, "");
    gtk_entry_set_text(record_notes_entry, "");
    gtk_label_set_text(record_error1, "");
    gtk_label_set_text(record_error2, "");
    gtk_label_set_text(record_error3, "");
    gtk_label_set_text(record_error4, "");
    gtk_label_set_text(record_error5, "");
    gtk_widget_hide(GTK_WIDGET(record_window));
    gtk_widget_show_all(GTK_WIDGET(main_window));
}

void validate_record(GtkEntry *entry, gpointer user_data)
{
    /*
    Checks if all the entries are valid and if they are enables the submit button
    */
    gboolean v1 = check_customer(record_customer_entry, NULL);
    gboolean v2 = check_type(record_type_entry, NULL);
    gboolean v3 = check_amount(record_amount_entry, NULL);
    gboolean v4 = check_date(record_date_entry, NULL);
    gboolean v5 = check_notes(record_notes_entry, NULL);
    gboolean v6 = check_account(record_account_entry, NULL);
    if (v1 == TRUE && v2 == TRUE && v3 == TRUE && v4 == TRUE && v5 == TRUE && v6 == TRUE)
    {
        gtk_widget_set_sensitive(GTK_WIDGET(record_submit), TRUE);
    }
    else
    {
        gtk_widget_set_sensitive(GTK_WIDGET(record_submit), FALSE);
    }
}

gboolean check_customer(GtkEntry *entry, gpointer user_data)
{
    /*
    Check if the type of transaction is Transfer or Payment and if it is we check if the customer ID is valid, and if it's not that type we check that the ID entry is empty
    */
    const char *customer = gtk_entry_get_text(entry);
    const char *type = gtk_entry_get_text(record_type_entry);
    if((strcmp(type, "Transfer") == 0 || strcmp(type, "Payment") == 0) && strlen(customer) == 0)
    {
        gtk_label_set_text(record_error1, "This type of transaction requires either a customer ID or an account ID");
        return FALSE;
    }
    if((strcmp(type, "Deposit") == 0 || strcmp(type, "Withdrawal") == 0) && strlen(customer) != 0)
    {
        gtk_label_set_text(record_error1, "This type of transaction does not require a customer ID");
        return FALSE;
    }
    if(strlen(customer) > 5)
    {
        gtk_label_set_text(record_error1, "Customer ID must be at most 5 characters long");
        return FALSE;
    }
    else if(strlen(customer) != 0)
    {
        for(int i = 0; i < strlen(customer); i++)
        {
            if (customer[i] > '9' || customer[i] < '0')
            {
                gtk_label_set_text(record_error1, "Customer ID must be a number");
                return FALSE;
            }
        }
    }
    gtk_label_set_markup(record_error1, "<span font_desc='30'><b>✔</b></span>");
    return TRUE;
}

gboolean check_type(GtkEntry *entry, gpointer user_data)
{
    /*
    Checks if the type of transaction is among the 4 predefined types
    */
    const char *type = gtk_entry_get_text(entry);
    if(strlen(type) == 0 || strlen(type) > 20)
    {
        gtk_label_set_text(record_error2, "Please enter a valid type: Deposit, Withdrawal, Transfer or Payment");
        return FALSE;
    }
    else
    {
        if (strcmp(type, "Deposit") != 0 && strcmp(type, "Withdrawal") != 0 && strcmp(type, "Transfer") != 0 && strcmp(type, "Payment") != 0)
        {
            gtk_label_set_text(record_error2, "Type must be Deposit, Withdrawal, Transfer or Payment");
            return FALSE;
        }
        gtk_label_set_markup(record_error2, "<span font_desc='30'><b>✔</b></span>");
        return TRUE;
    }
    
}

gboolean check_amount(GtkEntry *entry, gpointer user_data)
{
    /*
    We check if the inserted amount is a valid integer or float number
    */
    const char *amount = gtk_entry_get_text(entry);
    if(strlen(amount) == 0 || strlen(amount) > 9)
    {
        gtk_label_set_text(record_error3, "Please enter a valid amount");
        return FALSE;
    }
    else
    {
        for(int i = 0; i < strlen(amount); i++)
        {
            if ((amount[i] > '9' || amount[i] < '0') && amount[i] != '.')
            {
                gtk_label_set_text(record_error3, "Amount must be a number");
                return FALSE;
            }
        }
        gtk_label_set_markup(record_error3, "<span font_desc='30'><b>✔</b></span>");
        return TRUE;
    }
}

gboolean check_date(GtkEntry *entry, gpointer user_data)
{
    /* 
    Checks if the date is valid, taking into consideration leap years and the number of days in each month. If the date is empty we will later use the current date
    */
    const char *date = gtk_entry_get_text(entry);
    int year = 0;
    int month = 0;
    int day = 0;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    if (strlen(date) == 0)
    {
        gtk_label_set_text(record_error4, "The curent date will be used");
        return TRUE;
    }
    else
    if(strlen(date) > 10)
    {
        gtk_label_set_text(record_error4, "Please enter a valid date");
        return FALSE;
    }
    else
    {
        if (date[2] != '-' || date[5] != '-' )
        {
            gtk_label_set_text(record_error4, "Date must be in the format DD-MM-YYYY");
            return FALSE;
        }
        for(int i = 0; i < strlen(date); i++)
        {
            if (i != 2 && i != 5)
            {
                if (date[i] > '9' || date[i] < '0')
                {
                    gtk_label_set_text(record_error4, "Date must be made only of numbers in the format DD-MM-YYYY");
                    return FALSE;
                }
            }
        }
        for(int i = 0; i < 2; i++)
        {
            day = day * 10 + (date[i] - '0');
        }
        for(int i = 3; i < 5; i++)
        {
            month = month * 10 + (date[i] - '0');
        }
        for(int i = 6; i < 10; i++)
        {
            year = year * 10 + (date[i] - '0');
        }

        if (year < 2000 || year > tm.tm_year + 1900)
        {
            gtk_label_set_text(record_error4, "Year must be after 2000 until the curent year");
            return FALSE;
        }
        if (month < 1 || month > 12)
        {
            gtk_label_set_text(record_error4, "Month must be between 1 and 12");
            return FALSE;
        }
        if (year == tm.tm_year + 1900 && month > tm.tm_mon + 1)
        {
            gtk_label_set_text(record_error4, "Month must be before the curent month");
            return FALSE;
        }
        if (day < 1)
        {
            gtk_label_set_text(record_error4, "Day must be at least 1");
            return FALSE;
        }
        if (year == tm.tm_year + 1900 && month == tm.tm_mon + 1 && day > tm.tm_mday)
        {
            gtk_label_set_text(record_error4, "Day must be before the curent day");
            return FALSE;
        }
        if (year % 4 == 0 && month == 2 && day > 29)
        {
            gtk_label_set_text(record_error4, "February has 29 days in leap years");
            return FALSE;
        }
        if (year % 4 != 0 && month == 2 && day > 28)
        {
            gtk_label_set_text(record_error4, "February has 28 days in non-leap years");
            return FALSE;
        }
        if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day > 31)
        {
            gtk_label_set_text(record_error4, "This month has 31 days");
            return FALSE;
        }
        if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        {
            gtk_label_set_text(record_error4, "This month has 30 days");
            return FALSE;
        }
        gtk_label_set_markup(record_error4, "<span font_desc='30'><b>✔</b></span>");
        return TRUE;
    }
}

gboolean check_notes(GtkEntry *entry, gpointer user_data)
{
    /*
    We check that the optional notes don't exceed the 100 characters limit
    */
    const char *notes = gtk_entry_get_text(entry);
    if(strlen(notes) > 100)
    {
        gtk_label_set_text(record_error5, "Notes must be at most 100 characters long");
        return FALSE;
    }
    gtk_label_set_markup(record_error5, "<span font_desc='30'><b>✔</b></span>");
    return TRUE;
}

gboolean check_account(GtkEntry *entry, gpointer user_data)
{
    /*
    We check that the account ID is a valid integer
    */
    const char *account = gtk_entry_get_text(entry);
    if(strlen(account) > 5)
    {
        gtk_label_set_text(record_error6, "Account ID must be at most 5 characters long");
        return FALSE;
    }
    else if(strlen(account) != 0)
    {
        for(int i = 0; i < strlen(account); i++)
        {
            if (account[i] > '9' || account[i] < '0')
            {
                gtk_label_set_text(record_error6, "Account ID must be a number");
                return FALSE;
            }
        }
    }
    gtk_label_set_markup(record_error6, "<span font_desc='30'><b>✔</b></span>");
    return TRUE;
}

void insert_transaction(GtkButton *button, gpointer user_data)
{
    /*
    We check if the account exists, if the customer exists and if the account has enough money.
    If everything is ok we insert the transaction into the database with another insert_transaction_db
    After we insert the transaction we update the accounts' balance and the activity log
    */

    // First checking wether the account exists and has enough money
    sqlite3 *db;
    int rc = sqlite3_open("banking_app_database.db", &db);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_stmt *stmt;
    const char *account = gtk_entry_get_text(record_account_entry);
    // Make into int
    int account_id = atoi(account);

    const char *sql = "SELECT AccountID FROM Accounts WHERE UserID = ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement\n");
        return;
    }

    // Bind parameters
    if (sqlite3_bind_int(stmt, 1, user_id) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter !!1\n");
        return;
    }

    // Execute the statement
    int result;
    int ok = 0;
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int col_count = sqlite3_column_count(stmt);
        for (int i = 0; i < col_count; i++)
        {
            const char *column_value = (const char *)sqlite3_column_text(stmt, i);
            if (atoi(column_value) == account_id)
            {
                ok = 1;
                break;
            }
        }
    }

    if (ok == 0)
    {
        // There is no row, so finalize the statement
        sqlite3_finalize(stmt);
        // Close the database
        sqlite3_close(db);
        // Display an error message
        gtk_label_set_text(record_error6, "Account does not exist");
        gtk_widget_set_sensitive(GTK_WIDGET(record_submit), FALSE);
        return;
    }
    else
    {
        // There is a row, so finalize the statement
        sqlite3_finalize(stmt);
        // Close the database
        sqlite3_close(db);
    }

    // Checking if there is a customer and if it exists
    const char *customer = gtk_entry_get_text(record_customer_entry);
    // Make into int
    int customer_id = atoi(customer);

    if (strlen(customer) != 0)
    {
        sqlite3 *db;
        int rc = sqlite3_open("banking_app_database.db", &db);
        if (rc != SQLITE_OK){
            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            exit(1);
        }
        sqlite3_stmt *stmt;
        const char *sql = "SELECT CustomerID FROM Customers";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error preparing statement\n");
            return;
        }


        // Execute the statement
        int result;
        int ok = 0;
        while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
        {
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++)
            {
                const char *column_value = (const char *)sqlite3_column_text(stmt, i);
                if (atoi(column_value) == customer_id)
                {
                    ok = 1;
                    break;
                }
            }
        }

        if (ok == 0)
        {
            // There is no row, so finalize the statement
            sqlite3_finalize(stmt);
            // Close the database
            sqlite3_close(db);
            // Display an error message
            gtk_label_set_text(record_error1, "Customer does not exist");
            gtk_widget_set_sensitive(GTK_WIDGET(record_submit), FALSE);
            return;
        }
        else
        {
            // There is a row, so finalize the statement
            sqlite3_finalize(stmt);
            // Close the database
            sqlite3_close(db);
        }
    }
    else
    {
        customer_id = -1;
    }    

    // Checking if the account has enough money

    const char *type = gtk_entry_get_text(record_type_entry);
    const char *amount = gtk_entry_get_text(record_amount_entry);
    // Make into int
    int amount_int = atoi(amount);
    int balance = 0;

    if(strcmp(type, "Withdrawal") == 0 || strcmp(type, "Transfer") == 0 || strcmp(type, "Payment") == 0)
    {
        sqlite3 *db;
        int rc = sqlite3_open("banking_app_database.db", &db);
        if (rc != SQLITE_OK){
            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            exit(1);
        }
        sqlite3_stmt *stmt;
        const char *sql = "SELECT Balance FROM Accounts WHERE AccountID = ?";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error preparing statement\n");
            return;
        }

        // Bind parameters
        if (sqlite3_bind_int(stmt, 1, account_id) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 1\n");
            return;
        }

        // Execute the statement
        int result;
        while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
        {
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++)
            {
                const char *column_value = (const char *)sqlite3_column_text(stmt, i);
                balance = atof(column_value);
            }
        }

        if (balance < amount_int)
        {
            // There is not enough money, so finalize the statement
            sqlite3_finalize(stmt);
            // Close the database
            sqlite3_close(db);
            // Display an error message
            gtk_label_set_text(record_error3, "Account does not have enough money");
            gtk_widget_set_sensitive(GTK_WIDGET(record_submit), FALSE);
            return;
        }
        else
        {
            // There is enough money, so update the account balance
            balance -= amount_int;
        }
    }
    // We check if type is transfer if the accountID exists, if the type is payment if the customerID exists and nothing more if type is withdrawal

    if(strcmp(type, "Transfer") == 0)
    {
        // We use customer_id that was declared earlier

        sqlite3 *db;
        int rc = sqlite3_open("banking_app_database.db", &db);
        if (rc != SQLITE_OK){
            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            exit(1);
        }
        sqlite3_stmt *stmt;
        const char *sql = "SELECT AccountID FROM Accounts";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error preparing statement\n");
            return;
        }

        // Execute the statement
        int result;
        int ok = 0;
        while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
        {
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++)
            {
                const char *column_value = (const char *)sqlite3_column_text(stmt, i);
                if (atoi(column_value) == account_id)
                {
                    ok = 1;
                    break;
                }
            }
        }

        if (ok == 0)
        {
            // There is no row, so finalize the statement
            sqlite3_finalize(stmt);
            // Close the database
            sqlite3_close(db);
            // Display an error message
            gtk_label_set_text(record_error1, "Account does not exist");
            gtk_widget_set_sensitive(GTK_WIDGET(record_submit), FALSE);
            return;
        }
        else
        {
            // There is a row, so finalize the statement
            sqlite3_finalize(stmt);
            // Close the database
            sqlite3_close(db);
        }
        
        // We now transfer the money to the other account
        sqlite3 *db2;
        int rc2 = sqlite3_open("banking_app_database.db", &db2);
        if (rc2 != SQLITE_OK){
            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db2));
            sqlite3_close(db2);
            exit(1);
        }

        sqlite3_stmt *stmt2;
        const char *sql2 = "SELECT Balance FROM Accounts WHERE AccountID = ?";
        if (sqlite3_prepare_v2(db2, sql2, -1, &stmt2, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error preparing statement\n");
            return;
        }

        // Bind parameters
        if (sqlite3_bind_int(stmt2, 1, customer_id) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 12\n");
            return;
        }

        int cus_balance = 0;
        // Execute the statement
        int result2;
        while ((result2 = sqlite3_step(stmt2)) == SQLITE_ROW)
        {
            int col_count = sqlite3_column_count(stmt2);
            for (int i = 0; i < col_count; i++)
            {
                const char *column_value = (const char *)sqlite3_column_text(stmt2, i);
                cus_balance = atof(column_value);
            }
        }

        cus_balance += amount_int;

        // Update the balance in the database
        sqlite3 *db3;
        int rc3 = sqlite3_open("banking_app_database.db", &db3);
        if (rc3 != SQLITE_OK){
            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db3));
            sqlite3_close(db3);
            exit(1);
        }
        sqlite3_stmt *stmt3;
        const char *sql3 = "UPDATE Accounts SET Balance = ? WHERE AccountID = ?";
        if (sqlite3_prepare_v2(db3, sql3, -1, &stmt3, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error preparing statement\n");
            return;
        }

        // Bind parameters
        if (sqlite3_bind_int(stmt3, 1, cus_balance) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 13\n");
            return;
        }

        if (sqlite3_bind_int(stmt3, 2, customer_id) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 2\n");
            return;
        }

        // Execute the statement
        sqlite3_step(stmt3);

        // Insert the operation into the activity log
        insert_activity_db(account_id, type, amount_int);

        // Record the operation into the other accounts log
        insert_activity_db(customer_id, "Recieved Transfer", amount_int);

        const char *date1 = gtk_entry_get_text(record_date_entry);
        const char *notes1 = gtk_entry_get_text(record_notes_entry);

        if(strlen(date1) == 0)
        {
            g_print("The current date will be used\n");
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            char date2[11];
            snprintf(date2, sizeof(date2), "%d-%d-%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
            date1 = date2;
        }

        // Insert the transaction into the database
        insert_transaction_db(customer_id, account_id, type, amount_int, date1, notes1);

    }
    if(strcmp(type, "Deposit") == 0)
    {
        sqlite3 *db;
        int rc = sqlite3_open("banking_app_database.db", &db);
        if (rc != SQLITE_OK){
            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            exit(1);
        }
        sqlite3_stmt *stmt;
        const char *sql = "SELECT Balance FROM Accounts WHERE AccountID = ?";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error preparing statement\n");
            return;
        }

        // Bind parameters
        if (sqlite3_bind_int(stmt, 1, account_id) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 1\n");
            return;
        }

        // Execute the statement
        int result;
        while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
        {
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++)
            {
                const char *column_value = (const char *)sqlite3_column_text(stmt, i);
                balance = atof(column_value);
            }
        }

        balance += amount_int;
    }

    const char *date = gtk_entry_get_text(record_date_entry);
    const char *notes = gtk_entry_get_text(record_notes_entry);

    if(strlen(date) == 0)
    {
        g_print("The current date will be used\n");
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        char date3[11];
        snprintf(date3, sizeof(date3), "%d-%d-%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
        date = date3;
    }

    sqlite3_close(db);
    if(strcmp(type, "Transfer")!= 0)
    {
        // Insert the transaction into the database
        insert_transaction_db(customer_id, account_id, type, amount_int, date, notes);

        // Insert the operation into the activity log
        insert_activity_db(account_id, type, amount_int);
    }

    // Clear the entries and the errors
    gtk_entry_set_text(record_account_entry, "");
    gtk_entry_set_text(record_customer_entry, "");
    gtk_entry_set_text(record_type_entry, "");
    gtk_entry_set_text(record_amount_entry, "");
    gtk_entry_set_text(record_date_entry, "");
    gtk_entry_set_text(record_notes_entry, "");
    gtk_label_set_text(record_error1, "");
    gtk_label_set_text(record_error2, "");
    gtk_label_set_text(record_error3, "");
    gtk_label_set_text(record_error4, "");
    gtk_label_set_text(record_error5, "");
    gtk_label_set_text(record_error6, "");

    
    // Update the balance in the database
    sqlite3 *db2;
    int rc2 = sqlite3_open("banking_app_database.db", &db2);
    if (rc2 != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db2));
        sqlite3_close(db2);
        exit(1);
    }
    sqlite3_stmt *stmt2;
    const char *sql2 = "UPDATE Accounts SET Balance = ? WHERE AccountID = ?";
    if (sqlite3_prepare_v2(db2, sql2, -1, &stmt2, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement\n");
        return;
    }

    // Bind parameters
    if (sqlite3_bind_int(stmt2, 1, balance) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 1\n");
        return;
    }

    if (sqlite3_bind_int(stmt2, 2, account_id) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 2\n");
        return;
    }

    // Execute the statement
    sqlite3_step(stmt2);
    
    // Finalize the statement
    sqlite3_finalize(stmt2);

    // Close the database
    sqlite3_close(db2);

}

void insert_transaction_db(int customer_id, int account_id, const char *type, float amount, const char *date, const char *notes)
{
    /*
    We insert the transaction after it's checked into the database
    */
    fprintf(stderr, "Inserting transaction into database\n");
    sqlite3 *db;
    int rc = sqlite3_open("banking_app_database.db", &db);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO FinancialTransactions (AccountID, CustomerID, TransactionType, Amount, Date, Notes) VALUES (?, ?, ?, ?, ?, ?)";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement\n");
        return;
    }

    // Bind parameters
    if (sqlite3_bind_int(stmt, 1, account_id) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 1\n");
        return;
    }

    if (sqlite3_bind_int(stmt, 2, customer_id) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 2\n");
        return;
    }

    if (sqlite3_bind_text(stmt, 3, type, -1, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 3\n");
        return;
    }

    if (sqlite3_bind_int(stmt, 4, amount) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 4\n");
        return;
    }

    if (sqlite3_bind_text(stmt, 5, date, -1, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 5\n");
        return;
    }

    if (sqlite3_bind_text(stmt, 6, notes, -1, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 6\n");
        return;
    }

    // Execute the statement
    sqlite3_step(stmt);

    // Finalize the statement
    sqlite3_finalize(stmt);
    // Close the database
    sqlite3_close(db);
}

void insert_activity_db(int account_id, const char *type, int amount)
{
    /*
    We insert the activity after it's checked into the database
    */
    fprintf(stderr, "Inserting activity into database\n");
    sqlite3 *db;
    int rc = sqlite3_open("banking_app_database.db", &db);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO ActivityLog (AccountID, Date, ActivityType, Amount) VALUES (?, ?, ?, ?)";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement\n");
        return;
    }

    // Bind parameters
    if (sqlite3_bind_int(stmt, 1, account_id) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 1\n");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char date[11];
    snprintf(date, sizeof(date), "%d-%d-%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    if (sqlite3_bind_text(stmt, 2, date, -1, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 2\n");
        return;
    }

    if (sqlite3_bind_text(stmt, 3, type, -1, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 3\n");
        return;
    }

    if (sqlite3_bind_int(stmt, 4, amount) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 4\n");
        return;
    }

    // Execute the statement
    sqlite3_step(stmt);

    // Finalize the statement
    sqlite3_finalize(stmt);
    // Close the database
    sqlite3_close(db);
}

void hide_create(GtkButton *button, gpointer user_data)
{
    /*
    Hide the create account window and show the main window
    */
    //Clear the text view and the errors
    gtk_entry_set_text(create_type_entry, "");
    gtk_entry_set_text(create_balance_entry, "");
    gtk_label_set_text(create_error1, "");
    gtk_label_set_text(create_error2, "");

    gtk_widget_hide(GTK_WIDGET(create_window));
    gtk_widget_show_all(GTK_WIDGET(main_window));
}

void validate_create(GtkEntry *entry, gpointer user_data)
{
    /*
    Calls the functions that check the type and the balance and if they are both valid it enables the submit button
    */
    gboolean v1 = check_acc_type(create_type_entry, NULL);
    gboolean v2 = check_balance(create_balance_entry, NULL);
    if (v1 == TRUE && v2 == TRUE)
    {
        gtk_widget_set_sensitive(GTK_WIDGET(create_submit), TRUE);
    }
    else
    {
        gtk_widget_set_sensitive(GTK_WIDGET(create_submit), FALSE);
    }
}

gboolean check_acc_type(GtkEntry *entry, gpointer user_data)
{
    /*
    We check that the account type is valid and that it doesn't contain spaces or numbers, and at least one letter
    */
    const char *type = gtk_entry_get_text(entry);
    if(strlen(type) == 0 || strlen(type) > 20)
    {
        gtk_label_set_text(create_error1, "Please enter a valid type");
        return FALSE;
    }
    for(int i = 0; i < strlen(type); i++)
    {
        if (type[i] == ' ')
        {
            gtk_label_set_text(create_error1, "Type must not contain spaces");
            return FALSE;
        }
        if (type[i] > 'z' || type[i] < 'a')
        {
            if (type[i] > 'Z' || type[i] < 'A')
            {
                gtk_label_set_text(create_error1, "Type must contain only letters");
                return FALSE;
            }
        }
    }
    gtk_label_set_markup(create_error1, "<span font_desc='30'><b>✔</b></span>");
    return TRUE;
}

gboolean check_balance(GtkEntry *entry, gpointer user_data)
{
    /*
    We check if the balance is a valid integer or float number. If the entry is empty the balance is set to 0
    */
    const char *balance = gtk_entry_get_text(entry);
    if(strlen(balance) == 0)
    {
        gtk_label_set_text(create_error2, "Balance automatically set to 0");
        return TRUE;
    }
    if(strlen(balance) > 9)
    {
        gtk_label_set_text(create_error2, "Please enter a valid balance");
        return FALSE;
    }
    else
    {
        for(int i = 0; i < strlen(balance); i++)
        {
            if ((balance[i] > '9' || balance[i] < '0') && balance[i] != '.')
            {
                gtk_label_set_text(create_error2, "Balance must be a number");
                return FALSE;
            }
        }
        gtk_label_set_markup(create_error2, "<span font_desc='30'><b>✔</b></span>");
        return TRUE;
    }
}

void insert_account(GtkButton *button, gpointer user_data)
{
    /*
    Calls the function that inserts the account into the database and the function that inserts the operation into the activity log
    */
    const char *type = gtk_entry_get_text(create_type_entry);
    const char *balance = gtk_entry_get_text(create_balance_entry);
    // Make into float
    if (strlen(balance) == 0)
    {
        balance = "0";
    }
    int balance_int = atof(balance);

    // Insert the account into the database
    int acc_id = insert_account_db(type, balance_int);

    // Insert the operation into the activity log
    if (acc_id != -1)
        insert_activity_db(acc_id, "Account creation", balance_int);


    // Clear the entries and the errors
    gtk_entry_set_text(create_type_entry, "");
    gtk_entry_set_text(create_balance_entry, "");
    gtk_label_set_text(create_error1, "");
    gtk_label_set_text(create_error2, "");
}

int insert_account_db(const char *type, float balance)
{
    /*
    Creates the account in the database and returns the accountID
    */
    sqlite3 *db;
    int rc = sqlite3_open("banking_app_database.db", &db);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Accounts (UserID, AccountType, Balance) VALUES (?, ?, ?)";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement\n");
        return -1;
    }

    // Bind parameters
    if (sqlite3_bind_int(stmt, 1, user_id) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 1\n");
        return -1;
    }

    if (sqlite3_bind_text(stmt, 2, type, -1, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 2\n");
        return -1;
    }

    if (sqlite3_bind_double(stmt, 3, balance) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 3\n");
        return -1;
    }

    // Execute the statement
    sqlite3_step(stmt);

    // Get the last inserted rowid (AccountID)
    sqlite3_int64 accountID = sqlite3_last_insert_rowid(db);

    // Finalize the statement
    sqlite3_finalize(stmt);

    // Close the database
    sqlite3_close(db);

    return accountID;

}

void hide_edit(GtkButton *button, gpointer user_data)
{
    /*
    We hide the edit account window and show the main window
    */
    //Clear the text view and the errors
    gtk_entry_set_text(edit_account_entry, "");
    gtk_entry_set_text(edit_type_entry, "");
    gtk_entry_set_text(edit_balance_entry, "");
    gtk_label_set_text(edit_error1, "");
    gtk_label_set_text(edit_error2, "");
    gtk_label_set_text(edit_error3, "");

    gtk_widget_hide(GTK_WIDGET(edit_window));
    gtk_widget_show_all(GTK_WIDGET(main_window));
}

void validate_edit(GtkEntry *entry, gpointer user_data)
{
    /*
    Calls the functions that checks whether the account ID, the type and the balance are valid and if they are it enables the submit button
    Also if the account ID is valid it enables the delete button
    */
    gboolean v1 = check_edit_acc_type(edit_type_entry, NULL);
    gboolean v2 = check_edit_balance(edit_balance_entry, NULL);
    gboolean v3 = check_account_id(edit_account_entry, NULL);
    if (v1 == TRUE && v2 == TRUE && v3 == TRUE)
    {
        gtk_widget_set_sensitive(GTK_WIDGET(edit_submit), TRUE);
    }
    else
    {
        gtk_widget_set_sensitive(GTK_WIDGET(edit_submit), FALSE);
    }
    if (v3 == TRUE)
    {
        gtk_widget_set_sensitive(GTK_WIDGET(edit_delete), TRUE);
    }
    else
    {
        gtk_widget_set_sensitive(GTK_WIDGET(edit_delete), FALSE);
    }
}

gboolean check_account_id(GtkEntry *entry, gpointer user_data)
{
    /*
    Checks if the account ID is a valid integer
    */
    const char *account = gtk_entry_get_text(entry);
    if(strlen(account) == 0)
    {
        gtk_label_set_text(edit_error1, "Please enter an account ID");
        return FALSE;
    }
    if(strlen(account) > 5)
    {
        gtk_label_set_text(edit_error1, "Account ID must be at most 5 characters long");
        return FALSE;
    }
    else if(strlen(account) != 0)
    {
        for(int i = 0; i < strlen(account); i++)
        {
            if (account[i] > '9' || account[i] < '0')
            {
                gtk_label_set_text(edit_error1, "Account ID must be a number");
                return FALSE;
            }
        }
    }
    gtk_label_set_markup(edit_error1, "<span font_desc='30'><b>✔</b></span>");
    return TRUE;
}

gboolean check_edit_acc_type(GtkEntry *entry, gpointer user_data)
{
    /*
    We check that the account type is valid and that it doesn't contain spaces or numbers, and at least one letter
    */
    const char *type = gtk_entry_get_text(entry);
    if(strlen(type) > 20)
    {
        gtk_label_set_text(edit_error2, "Please enter a valid type");
        return FALSE;
    }
    for(int i = 0; i < strlen(type); i++)
    {
        if (type[i] == ' ')
        {
            gtk_label_set_text(edit_error2, "Type must not contain spaces");
            return FALSE;
        }
        if (type[i] > 'z' || type[i] < 'a')
        {
            if (type[i] > 'Z' || type[i] < 'A')
            {
                gtk_label_set_text(edit_error2, "Type must contain only letters");
                return FALSE;
            }
        }
    }      
    gtk_label_set_markup(edit_error2, "<span font_desc='30'><b>✔</b></span>");
    return TRUE;
}

gboolean check_edit_balance(GtkEntry *entry, gpointer user_data)
{
    /*
    We check that the balance is a valid integer or float number. If the entry is empty the balance is set to 0
    */
    const char *balance = gtk_entry_get_text(entry);
    if(strlen(balance) > 9)
    {
        gtk_label_set_text(edit_error3, "Please enter a valid balance");
        return FALSE;
    }
    else
    {
        for(int i = 0; i < strlen(balance); i++)
        {
            if ((balance[i] > '9' || balance[i] < '0') && balance[i] != '.')
            {
                gtk_label_set_text(edit_error3, "Balance must be a number");
                return FALSE;
            }
        }
        gtk_label_set_markup(edit_error3, "<span font_desc='30'><b>✔</b></span>");
        return TRUE;
    }
}

void update_account(GtkButton *button, gpointer user_data)
{
    /*
    Checks if the account ID is valid and if the type or balance are valid
    Calls the function that updates the account in the database and the function that inserts the operation into the activity log
    */
    const char *account = gtk_entry_get_text(edit_account_entry);
    // Make into int
    int account_id = atoi(account);

    // First checking wether the account exists
    sqlite3 *db1;
    int rc1 = sqlite3_open("banking_app_database.db", &db1);
    if (rc1 != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db1));
        sqlite3_close(db1);
        exit(1);
    }

    sqlite3_stmt *stmt1;
    const char *sql1 = "SELECT AccountID FROM Accounts WHERE UserID = ?";
    if (sqlite3_prepare_v2(db1, sql1, -1, &stmt1, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement\n");
        return;
    }

    // Bind parameters
    if (sqlite3_bind_int(stmt1, 1, user_id) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 1\n");
        return;
    }

    // Execute the statement
    int result1;
    int ok = 0;
    while ((result1 = sqlite3_step(stmt1)) == SQLITE_ROW)
    {
        int col_count = sqlite3_column_count(stmt1);
        for (int i = 0; i < col_count; i++)
        {
            const char *column_value = (const char *)sqlite3_column_text(stmt1, i);
            if (atoi(column_value) == account_id)
            {
                ok = 1;
                break;
            }
        }
    }
    if(ok == 0)
    {
        // There is no row, so finalize the statement
        sqlite3_finalize(stmt1);
        // Close the database
        sqlite3_close(db1);
        // Display an error message
        gtk_label_set_text(edit_error1, "Account does not exist");
        gtk_widget_set_sensitive(GTK_WIDGET(edit_submit), FALSE);
        return;
    }
    else
    {
        // There is a row, so finalize the statement
        sqlite3_finalize(stmt1);
        // Close the database
        sqlite3_close(db1);
    }
    // Checking if the type and balance aren't both empty
    const char *type = gtk_entry_get_text(edit_type_entry);
    const char *balance = gtk_entry_get_text(edit_balance_entry);
    if (strlen(balance) == 0 && strlen(type) == 0)
    {
        gtk_label_set_text(edit_error3, "Please enter at least a valid type or balance");
        gtk_label_set_text(edit_error2, "Please enter at least a valid type or balance");
        gtk_widget_set_sensitive(GTK_WIDGET(edit_submit), FALSE);
        return;
    }
    if (strlen(balance) == 0)
    {
        balance = "-1";
    }
    // Make into float
    int balance_int = atof(balance);

    // Insert the account into the database
    int acc_id = update_account_db(account_id, type, balance_int);

    // Getting the account balance for the activity log
    sqlite3 *db;
    int rc = sqlite3_open("banking_app_database.db", &db);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    sqlite3_stmt *stmt;
    const char *sql = "SELECT Balance FROM Accounts WHERE AccountID = ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement\n");
        return;
    }

    // Bind parameters
    if (sqlite3_bind_int(stmt, 1, account_id) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 1\n");
        return;
    }

    // Execute the statement
    int result;
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int col_count = sqlite3_column_count(stmt);
        for (int i = 0; i < col_count; i++)
        {
            const char *column_value = (const char *)sqlite3_column_text(stmt, i);
            balance_int = atof(column_value);
        }
    }

    // Insert the operation into the activity log
    if (acc_id != -1)
        insert_activity_db(acc_id, "Account update", balance_int);

    // Clear the entries and the errors
    gtk_entry_set_text(edit_account_entry, "");
    gtk_entry_set_text(edit_type_entry, "");
    gtk_entry_set_text(edit_balance_entry, "");
    gtk_label_set_text(edit_error1, "");
    gtk_label_set_text(edit_error2, "");
    gtk_label_set_text(edit_error3, "");

}

int update_account_db(int account_id, const char *type, float balance)
{
    /*
    Updates the account in the database and returns the accountID
    */
    fprintf(stderr, "Updating account in database\n");
    sqlite3 *db;
    int rc = sqlite3_open("banking_app_database.db", &db);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    if(balance == -1)
    {
        sqlite3_stmt *stmt;
        const char *sql = "UPDATE Accounts SET AccountType = ? WHERE AccountID = ?";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error preparing statement\n");
            return -1;
        }

        // Bind parameters
        if (sqlite3_bind_text(stmt, 1, type, -1, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 1\n");
            return -1;
        }

        if (sqlite3_bind_int(stmt, 2, account_id) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 2\n");
            return -1;
        }

        // Execute the statement
        sqlite3_step(stmt);

        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    else if(strlen(type) == 0)
    {
        sqlite3_stmt *stmt;
        const char *sql = "UPDATE Accounts SET Balance = ? WHERE AccountID = ?";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error preparing statement\n");
            return -1;
        }

        // Bind parameters
        if (sqlite3_bind_double(stmt, 1, balance) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 1\n");
            return -1;
        }

        if (sqlite3_bind_int(stmt, 2, account_id) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 2\n");
            return -1;
        }

        // Execute the statement
        sqlite3_step(stmt);

        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    else
    {
        sqlite3_stmt *stmt;
        const char *sql = "UPDATE Accounts SET AccountType = ?, Balance = ? WHERE AccountID = ?";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error preparing statement\n");
            return -1;
        }

        // Bind parameters
        if (sqlite3_bind_text(stmt, 1, type, -1, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 1\n");
            return -1;
        }

        if (sqlite3_bind_double(stmt, 2, balance) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 2\n");
            return -1;
        }

        if (sqlite3_bind_int(stmt, 3, account_id) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 3\n");
            return -1;
        }

        // Execute the statement
        sqlite3_step(stmt);

        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    // Close the database
    sqlite3_close(db);

    return account_id;
}

void delete_account_edit(GtkButton *button, gpointer user_data)
{
    /*
    Check if the account ID is valid and if it is it calls the function that deletes the account from the database 
    */
    const char *account = gtk_entry_get_text(edit_account_entry);
    // Make into int
    int account_id = atoi(account);

    // Check if the account exists
    sqlite3 *db;
    int rc = sqlite3_open("banking_app_database.db", &db);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    sqlite3_stmt *stmt;
    const char *sql = "SELECT AccountID FROM Accounts WHERE UserID = ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement\n");
        return;
    }

    // Bind parameters
    if (sqlite3_bind_int(stmt, 1, user_id) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter !!1\n");
        return;
    }

    // Execute the statement
    int result;
    int ok = 0;
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int col_count = sqlite3_column_count(stmt);
        for (int i = 0; i < col_count; i++)
        {
            const char *column_value = (const char *)sqlite3_column_text(stmt, i);
            if (atoi(column_value) == account_id)
            {
                ok = 1;
                break;
            }
        }
    }
    if(ok == 0)
    {
        // There is no row, so finalize the statement
        sqlite3_finalize(stmt);
        // Close the database
        sqlite3_close(db);
        // Display an error message
        gtk_label_set_text(edit_error1, "Account does not exist");
        gtk_widget_set_sensitive(GTK_WIDGET(edit_delete), FALSE);
        return;
    }
    else
    {
        // There is a row, so finalize the statement
        sqlite3_finalize(stmt);
        // Close the database
        sqlite3_close(db);
    }

    // Insert the account into the database
    int acc_id = delete_account_db(account_id);

    // Insert the operation into the activity log
    if (acc_id != -1)
        insert_activity_db(acc_id, "Account deletion", 0);

    // Clear the entries and the errors
    gtk_entry_set_text(edit_account_entry, "");
    gtk_entry_set_text(edit_type_entry, "");
    gtk_entry_set_text(edit_balance_entry, "");
    gtk_label_set_text(edit_error1, "");
    gtk_label_set_text(edit_error2, "");
    gtk_label_set_text(edit_error3, "");
}

int delete_account_db(int account_id)
{
    /*
    Deletes the account from the database
    */
    fprintf(stderr, "Deleting account from database\n");
    sqlite3 *db;
    int rc = sqlite3_open("banking_app_database.db", &db);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM Accounts WHERE AccountID = ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement\n");
        return -1;
    }

    // Bind parameters
    if (sqlite3_bind_int(stmt, 1, account_id) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 1\n");
        return -1;
    }

    // Execute the statement
    sqlite3_step(stmt);

    // Finalize the statement
    sqlite3_finalize(stmt);

    // Close the database
    sqlite3_close(db);

    return account_id;
}

void view_customers(GtkMenuItem *menuitem, gpointer user_data)
{
    /*
    Opens the same view dialog and shows all the customers affiliated with the user's account
    */

    // We first get the accounts, and for every account we get the customers by checking the transactions then we show them
    // First we get the accounts
    int processed_customers[101];
    int l = 0;
    sqlite3 *db1;
    int rc1 = sqlite3_open("banking_app_database.db", &db1);
    if (rc1 != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db1));
        sqlite3_close(db1);
        exit(1);
    }

    sqlite3_stmt *stmt1;
    const char *sql1 = "SELECT AccountID FROM Accounts WHERE UserID = ?";

    if (sqlite3_prepare_v2(db1, sql1, -1, &stmt1, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement\n");
        return;
    }

    // Bind parameters
    if (sqlite3_bind_int(stmt1, 1, user_id) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 1\n");
        return;
    }

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(view_text);

    // Execute the statement
    int result1;
    while((result1 = sqlite3_step(stmt1)) == SQLITE_ROW)
    {
        int col_count = sqlite3_column_count(stmt1);
        for (int i = 0; i < col_count; i++)
        {
            const char *column_value = (const char *)sqlite3_column_text(stmt1, i);
            int account_id = atoi(column_value);

            // we specify the account
            char column_info[256];
            snprintf(column_info, sizeof(column_info), "\n Customers for account %s: \n ", column_value);
            gtk_text_buffer_insert_at_cursor(buffer, column_info, -1);

            // Now we get the customers
            sqlite3 *db2;
            int rc2 = sqlite3_open("banking_app_database.db", &db2);
            if (rc2 != SQLITE_OK){
                fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db2));
                sqlite3_close(db2);
                exit(1);
            }

            sqlite3_stmt *stmt2;
            const char *sql2 = "SELECT CustomerID FROM FinancialTransactions WHERE AccountID = ?";

            if (sqlite3_prepare_v2(db2, sql2, -1, &stmt2, NULL) != SQLITE_OK)
            {
                fprintf(stderr, "Error preparing statement\n");
                return;
            }

            // Bind parameters
            if (sqlite3_bind_int(stmt2, 1, account_id) != SQLITE_OK)
            {
                fprintf(stderr, "Error binding parameter 1\n");
                return;
            }

            // Execute the statement
            int result2;
            while((result2 = sqlite3_step(stmt2)) == SQLITE_ROW)
            {
                int col_count2 = sqlite3_column_count(stmt2);
                for (int j = 0; j < col_count2; j++)
                {
                    const char *column_value2 = (const char *)sqlite3_column_text(stmt2, i);
                    int customer_id = atoi(column_value2);
                    // We check if we already processed this customer
                    int ok = 0;
                    for (int k = 0; k <= l; k++)
                    {
                        if (processed_customers[k] == customer_id)
                        {
                            ok = 1;
                        }
                    }
                    // Now we get the customer info
                    if(ok == 0)
                    {
                        sqlite3 *db3;
                        int rc3 = sqlite3_open("banking_app_database.db", &db3);
                        if (rc3 != SQLITE_OK){
                            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db3));
                            sqlite3_close(db3);
                            exit(1);
                        }

                        sqlite3_stmt *stmt3;
                        const char *sql3 = "SELECT * FROM Customers WHERE CustomerID = ?";

                        if (sqlite3_prepare_v2(db3, sql3, -1, &stmt3, NULL) != SQLITE_OK)
                        {
                            fprintf(stderr, "Error preparing statement\n");
                            return;
                        }

                        // Bind parameters
                        if (sqlite3_bind_int(stmt3, 1, customer_id) != SQLITE_OK)
                        {
                            fprintf(stderr, "Error binding parameter 1\n");
                            return;
                        }

                        // Execute the statement
                        int result3;
                        while((result3 = sqlite3_step(stmt3)) == SQLITE_ROW)
                        {
                            int col_count3 = sqlite3_column_count(stmt3);
                            processed_customers[l] = customer_id;
                            l++;
                            for(int k = 0; k < col_count3; k++)
                            {
                                const char *column_name3 = (const char *)sqlite3_column_name(stmt3, k);
                                const char *column_value3 = (const char *)sqlite3_column_text(stmt3, k);

                                char column_info2[256];
                                snprintf(column_info2, sizeof(column_info2), "%s: %s | ", column_name3, column_value3);
                                gtk_text_buffer_insert_at_cursor(buffer, column_info2, -1);
                            }
                            gtk_text_buffer_insert_at_cursor(buffer, "\n", -1);
                            // Put the text buffer in the text view
                        }
                    }
                }
            }
        }
    }
    gtk_text_view_set_buffer(view_text, buffer);
    // Show the dialog
    gtk_dialog_run(view_dialog);    
}

void hide_customer(GtkButton *button, gpointer user_data)
{
    /*
    Clears the customer manage window and hides it, opening the main widnow
    */
    //Clear the text view and the errors
    gtk_entry_set_text(customer_id_entry, "");
    gtk_entry_set_text(customer_last_entry, "");
    gtk_entry_set_text(customer_first_entry, "");
    gtk_entry_set_text(customer_address_entry, "");
    gtk_entry_set_text(customer_email_entry, "");
    gtk_entry_set_text(customer_phone_entry, "");
    gtk_label_set_text(customer_error1, "");
    gtk_label_set_text(customer_error3, "");
    gtk_label_set_text(customer_error3, "");
    gtk_label_set_text(customer_error4, "");
    gtk_label_set_text(customer_error5, "");
    gtk_label_set_text(customer_error6, "");

    gtk_widget_hide(GTK_WIDGET(customer_window));
    gtk_widget_show_all(GTK_WIDGET(main_window));
}

void change_customer(GtkButton *button, gpointer user_data)
{
    /*
    We check if the customer exists and if it does we call the function that changes the customer in the database
    */
    const char *cus_id = gtk_entry_get_text(customer_id_entry);
    // Make into int
    int customer_id = atoi(cus_id);

    // First checking wether the customer exists
    sqlite3 *db1;
    int rc1 = sqlite3_open("banking_app_database.db", &db1);
    if (rc1 != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db1));
        sqlite3_close(db1);
        exit(1);
    }

    sqlite3_stmt *stmt1;
    const char *sql1 = "SELECT CustomerID FROM Customers WHERE CustomerID = ?";
    if (sqlite3_prepare_v2(db1, sql1, -1, &stmt1, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement\n");
        return;
    }

    // Bind parameters
    if (sqlite3_bind_int(stmt1, 1, customer_id) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 1\n");
        return;
    }

    // Execute the statement
    int result1;
    int ok = 0;
    while ((result1 = sqlite3_step(stmt1)) == SQLITE_ROW)
    {
        int col_count = sqlite3_column_count(stmt1);
        for (int i = 0; i < col_count; i++)
        {
            const char *column_value = (const char *)sqlite3_column_text(stmt1, i);
            if (atoi(column_value) == customer_id)
            {
                ok = 1;
                break;
            }
        }
    }
    if(ok == 0)
    {
        // There is no row, so finalize the statement
        sqlite3_finalize(stmt1);
        // Close the database
        sqlite3_close(db1);
        // Display an error message
        gtk_label_set_text(customer_error1, "Customer does not exist");
        gtk_widget_set_sensitive(GTK_WIDGET(customer_change), FALSE);
        return;
    }
    else
    {
        // There is a row, so finalize the statement
        sqlite3_finalize(stmt1);
        // Close the database
        sqlite3_close(db1);
    }

    // Now updating the customer with the 5 possible informations
    // Change the customer into the database
    change_customer_db(customer_id);

    // Clear the entries and the errors
    gtk_entry_set_text(customer_id_entry, "");
    gtk_entry_set_text(customer_last_entry, "");
    gtk_entry_set_text(customer_first_entry, "");
    gtk_entry_set_text(customer_address_entry, "");
    gtk_entry_set_text(customer_email_entry, "");
    gtk_entry_set_text(customer_phone_entry, "");
    gtk_label_set_text(customer_error1, "");
    gtk_label_set_text(customer_error2, "");
    gtk_label_set_text(customer_error3, "");
    gtk_label_set_text(customer_error4, "");
    gtk_label_set_text(customer_error5, "");
    gtk_label_set_text(customer_error6, "");
}

void create_customer(GtkButton *button, gpointer user_data)
{
    /*
    We call the create_customer_db with the 5 inputs, to create the customer, returning the ID
    */
    const char *last = gtk_entry_get_text(customer_last_entry);
    const char *first = gtk_entry_get_text(customer_first_entry);
    const char *address = gtk_entry_get_text(customer_address_entry);
    const char *email = gtk_entry_get_text(customer_email_entry);
    const char *phone = gtk_entry_get_text(customer_phone_entry);

    // Insert the customer into the database
    int customer_id = create_customer_db(last, first, address, email, phone);

    // We cant add to the audit this action cause the operation is done by the user not an account
    // Create a string with the customer id
    char customer_id_string[256];
    snprintf(customer_id_string, sizeof(customer_id_string), "The customer has been created with ID: %d", customer_id);
    // Clear the entries and the errors
    gtk_entry_set_text(customer_id_entry, "");
    gtk_entry_set_text(customer_last_entry, "");
    gtk_entry_set_text(customer_first_entry, "");
    gtk_entry_set_text(customer_address_entry, "");
    gtk_entry_set_text(customer_email_entry, "");
    gtk_entry_set_text(customer_phone_entry, "");
    gtk_label_set_text(customer_error1, customer_id_string);
    gtk_label_set_text(customer_error2, "");
    gtk_label_set_text(customer_error3, "");
    gtk_label_set_text(customer_error4, "");
    gtk_label_set_text(customer_error5, "");
    gtk_label_set_text(customer_error6, "");
}

int create_customer_db(const char *last, const char *first, const char *address, const char *email, const char *phone)
{
    /*
    Creates the customer in the database and returns the customerID
    */
    fprintf(stderr, "Inserting customer into database\n");
    sqlite3 *db;
    int rc = sqlite3_open("banking_app_database.db", &db);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Customers (LastName, FirstName, Address, Email, Phone) VALUES (?, ?, ?, ?, ?)";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement\n");
        return -1;
    }

    // Bind parameters
    if (sqlite3_bind_text(stmt, 1, last, -1, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 1\n");
        return -1;
    }

    if (sqlite3_bind_text(stmt, 2, first, -1, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 2\n");
        return -1;
    }

    if (sqlite3_bind_text(stmt, 3, address, -1, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 3\n");
        return -1;
    }

    if (sqlite3_bind_text(stmt, 4, email, -1, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 4\n");
        return -1;
    }

    if (sqlite3_bind_text(stmt, 5, phone, -1, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 5\n");
        return -1;
    }

    // Execute the statement
    sqlite3_step(stmt);

    // Get the last inserted rowid (CustomerID)
    sqlite3_int64 customerID = sqlite3_last_insert_rowid(db);

    // Finalize the statement
    sqlite3_finalize(stmt);

    // Close the database
    sqlite3_close(db);

    return customerID;
}

void validate_customer(GtkEntry *entry, gpointer user_data)
{
    /*
    Calls the functions that check the data.
    If the customer ID and at least one other entry are valid it enables the change button
    If the 5 entries except the customer ID are valid it enables the create button
    */
    gboolean v1 = check_customer_id(customer_id_entry, NULL);
    gboolean v2 = check_customer_last(customer_last_entry, NULL);
    gboolean v3 = check_customer_first(customer_first_entry, NULL);
    gboolean v4 = check_customer_address(customer_address_entry, NULL);
    gboolean v5 = check_customer_email(customer_email_entry, NULL);
    gboolean v6 = check_customer_phone(customer_phone_entry, NULL);
    if (v2 == TRUE && v3 == TRUE && v4 == TRUE && v5 == TRUE && v6 == TRUE)
    {
        gtk_widget_set_sensitive(GTK_WIDGET(customer_create), TRUE);
    }
    else
    {
        gtk_widget_set_sensitive(GTK_WIDGET(customer_create), FALSE);
    }
    if (v1 == TRUE && (v2 == TRUE || v3 == TRUE || v4 == TRUE || v5 == TRUE || v6 == TRUE))
    {
        gtk_widget_set_sensitive(GTK_WIDGET(customer_change), TRUE);
    }
    else
    {
        gtk_widget_set_sensitive(GTK_WIDGET(customer_change), FALSE);
    }
}

gboolean check_customer_id(GtkEntry *entry, gpointer user_data)
{
    /*
    Checks if the customer ID is a valid integer
    */
    if(strlen(gtk_entry_get_text(entry)) == 0)
    {
        gtk_label_set_text(customer_error1, "");
        return FALSE;
    }
    const char *cus_id = gtk_entry_get_text(entry);
    if(strlen(cus_id) > 5)
    {
        gtk_label_set_text(customer_error1, "Customer ID must be at most 5 characters long");
        return FALSE;
    }
    else if(strlen(cus_id) != 0)
    {
        for(int i = 0; i < strlen(cus_id); i++)
        {
            if (cus_id[i] > '9' || cus_id[i] < '0')
            {
                gtk_label_set_text(customer_error1, "Customer ID must be a number");
                return FALSE;
            }
        }
    }
    gtk_label_set_markup(customer_error1, "<span font_desc='30'><b>✔</b></span>");
    return TRUE;
}

gboolean check_customer_last(GtkEntry *entry, gpointer user_data)
{
    /*
    Checks if the last name is valid and if it doesn't contain numbers, and at least one Capital Letter
    */
    const char *last = gtk_entry_get_text(entry);
    if(strlen(last) == 0)
    {
        gtk_label_set_text(customer_error3, "");
        return FALSE;
    }
    if(strlen(last) > 20)
    {
        gtk_label_set_text(customer_error3, "Please enter a valid last name");
        return FALSE;
    }     
    if(last[0]< 'A' || last[0] > 'Z')
    {
        gtk_label_set_text(customer_error3, "Last name must start with a capital letter");
        return FALSE;
    }
    for(int i = 1; i < strlen(last); i++)
    {
        if(last[i] < 'a' || last[i] > 'z')
        {
            if(last[i] < 'A' || last[i] > 'Z') 
            {
                if (last[i] != ' ')
                {
                    gtk_label_set_text(customer_error3, "Last name must contain only letters");
                    return FALSE;
                }
            }
        }
    }
    gtk_label_set_markup(customer_error3, "<span font_desc='30'><b>✔</b></span>");
    return TRUE;
}

gboolean check_customer_first(GtkEntry *entry, gpointer user_data)
{
    /*
    Checks if the First name is valid and if it doesn't contain numbers, and at least one Capital Letter
    */
    const char *first = gtk_entry_get_text(entry);
    if(strlen(first) == 0)
    {
        gtk_label_set_text(customer_error2, "");
        return FALSE;
    }
    if(strlen(first) > 20)
    {
        gtk_label_set_text(customer_error2, "Please enter a valid first name");
        return FALSE;
    }     
    if(first[0]< 'A' || first[0] > 'Z')
    {
        gtk_label_set_text(customer_error2, "First name must start with a capital letter");
        return FALSE;
    }
    for(int i = 1; i < strlen(first); i++)
    {
        if(first[i] < 'a' || first[i] > 'z')
        {
            if(first[i] < 'A' || first[i] > 'Z') 
            {
                if (first[i] != ' ' && first[i] != '-')
                {
                    gtk_label_set_text(customer_error2, "First name must contain only letters");
                    return FALSE;
                }
            }
        }
    }
    gtk_label_set_markup(customer_error2, "<span font_desc='30'><b>✔</b></span>");
    return TRUE;
}

gboolean check_customer_address(GtkEntry *entry, gpointer user_data)
{
    /*
    Checks if the address isn't too long
    */
    const char *address = gtk_entry_get_text(entry);
    if(strlen(address) == 0)
    {
        gtk_label_set_text(customer_error4, "");
        return FALSE;
    }
    if(strlen(address) > 60)
    {
        gtk_label_set_text(customer_error4, "Please enter a shorter address");
        return FALSE;
    }     
    gtk_label_set_markup(customer_error4, "<span font_desc='30'><b>✔</b></span>");
    return TRUE;
}

gboolean check_customer_email(GtkEntry *entry, gpointer user_data)
{
    /*
    Checks if the email is valid and if it contains an @
    */
    const char *email = gtk_entry_get_text(entry);
    if(strlen(email) == 0)
    {
        gtk_label_set_text(customer_error5, "");
        return FALSE;
    }
    if(strlen(email) > 30)
    {
        gtk_label_set_text(customer_error5, "Please enter a shorter email");
        return FALSE;
    }
    int ok = 0;
    for(int i = 0; i < strlen(email); i++)
    {
        if(email[i] == '@')
        {
            ok = 1;
            break;
        }
    }
    if(ok == 0)
    {
        gtk_label_set_text(customer_error5, "Please enter a valid email");
        return FALSE;
    }     
    gtk_label_set_markup(customer_error5, "<span font_desc='30'><b>✔</b></span>");
    return TRUE;
}

gboolean check_customer_phone(GtkEntry *entry, gpointer user_data)
{
    /*
    Checks if the phone number contains only numbers and is at max 10 characters long
    */
    const char *phone = gtk_entry_get_text(entry);
    if(strlen(phone) == 0)
    {
        gtk_label_set_text(customer_error6, "");
        return FALSE;
    }
    if(strlen(phone) > 10)
    {
        gtk_label_set_text(customer_error6, "Please enter a valid phone number");
        return FALSE;
    }
    for(int i = 0; i < strlen(phone); i++)
    {
        if (phone[i] > '9' || phone[i] < '0')
        {
            gtk_label_set_text(customer_error6, "Phone number must be a number");
            return FALSE;
        }
    }
    gtk_label_set_markup(customer_error6, "<span font_desc='30'><b>✔</b></span>");
    return TRUE;
}

void change_customer_db(int customer_id)
{
    /*
    Changes all 5 entries in the database, if they are not empty
    */
    const char *first = gtk_entry_get_text(customer_first_entry);
    const char *last = gtk_entry_get_text(customer_last_entry);
    const char *address = gtk_entry_get_text(customer_address_entry);
    const char *email = gtk_entry_get_text(customer_email_entry);
    const char *phone = gtk_entry_get_text(customer_phone_entry);

    if(strlen(first)!=0)
    {
        sqlite3 *db;
        int rc = sqlite3_open("banking_app_database.db", &db);
        if (rc != SQLITE_OK){
            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            exit(1);
        }
        sqlite3_stmt *stmt;
        const char *sql = "UPDATE Customers SET FirstName = ? WHERE CustomerID = ?";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error preparing statement\n");
            return;
        }

        // Bind parameters
        if (sqlite3_bind_text(stmt, 1, first, -1, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 1\n");
            return;
        }

        if (sqlite3_bind_int(stmt, 2, customer_id) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 2\n");
            return;
        }

        // Execute the statement
        sqlite3_step(stmt);

        // Finalize the statement
        sqlite3_finalize(stmt);

        // Close the database
        sqlite3_close(db);
    }

    if(strlen(last) != 0)
    {
        sqlite3 *db;
        int rc = sqlite3_open("banking_app_database.db", &db);
        if (rc != SQLITE_OK){
            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            exit(1);
        }
        sqlite3_stmt *stmt;
        const char *sql = "UPDATE Customers SET LastName = ? WHERE CustomerID = ?";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error preparing statement\n");
            return;
        }

        // Bind parameters
        if (sqlite3_bind_text(stmt, 1, last, -1, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 1\n");
            return;
        }

        if (sqlite3_bind_int(stmt, 2, customer_id) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 2\n");
            return;
        }

        // Execute the statement
        sqlite3_step(stmt);

        // Finalize the statement
        sqlite3_finalize(stmt);

        // Close the database
        sqlite3_close(db);
    }

    if(strlen(address) != 0)
    {
        sqlite3 *db;
        int rc = sqlite3_open("banking_app_database.db", &db);
        if (rc != SQLITE_OK){
            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            exit(1);
        }
        sqlite3_stmt *stmt;
        const char *sql = "UPDATE Customers SET Address = ? WHERE CustomerID = ?";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error preparing statement\n");
            return;
        }

        // Bind parameters
        if (sqlite3_bind_text(stmt, 1, address, -1, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 1\n");
            return;
        }

        if (sqlite3_bind_int(stmt, 2, customer_id) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 2\n");
            return;
        }

        // Execute the statement
        sqlite3_step(stmt);

        // Finalize the statement
        sqlite3_finalize(stmt);

        // Close the database
        sqlite3_close(db);
    }

    if(strlen(email) != 0)
    {
        sqlite3 *db;
        int rc = sqlite3_open("banking_app_database.db", &db);
        if (rc != SQLITE_OK){
            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            exit(1);
        }
        sqlite3_stmt *stmt;
        const char *sql = "UPDATE Customers SET Email = ? WHERE CustomerID = ?";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error preparing statement\n");
            return;
        }

        // Bind parameters
        if (sqlite3_bind_text(stmt, 1, email, -1, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 1\n");
            return;
        }

        if (sqlite3_bind_int(stmt, 2, customer_id) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 2\n");
            return;
        }

        // Execute the statement
        sqlite3_step(stmt);

        // Finalize the statement
        sqlite3_finalize(stmt);

        // Close the database
        sqlite3_close(db);
    }

    if(strlen(phone) != 0)
    {
        sqlite3 *db;
        int rc = sqlite3_open("banking_app_database.db", &db);
        if (rc != SQLITE_OK){
            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            exit(1);
        }
        sqlite3_stmt *stmt;
        const char *sql = "UPDATE Customers SET Phone = ? WHERE CustomerID = ?";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error preparing statement\n");
            return;
        }

        // Bind parameters
        if (sqlite3_bind_text(stmt, 1, phone, -1, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 1\n");
            return;
        }

        if (sqlite3_bind_int(stmt, 2, customer_id) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 2\n");
            return;
        }

        // Execute the statement
        sqlite3_step(stmt);

        // Finalize the statement
        sqlite3_finalize(stmt);

        // Close the database
        sqlite3_close(db);
    
    }
}

void view_expense_report()
{
    /*
    Shows the expense report for the user
    */
    sqlite3 *db;
    int rc = sqlite3_open("banking_app_database.db", &db);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    sqlite3_stmt *stmt;
    const char *sql = "SELECT FT.Date,"
                      "COALESCE(C.FirstName || ' ' || C.LastName, 'No Customer') AS CustomerName,"
                      "A.AccountType,"
                      "FT.TransactionType,"
                      "FT.Notes,"
                      "FT.Amount "
                      "FROM FinancialTransactions FT "
                      "JOIN Accounts A ON FT.AccountID = A.AccountID "
                      "LEFT JOIN Customers C ON FT.CustomerID = C.CustomerID "
                      "WHERE A.UserID = ? AND (FT.TransactionType = 'Payment' OR FT.TransactionType = 'Withdrawal')"  //Transfer isn't really an expense so we don't show it
                      "ORDER BY substr(FT.Date, 7, 4), substr(FT.Date, 4, 2), substr(FT.Date, 1, 2)";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    // Bind parameters
    if (sqlite3_bind_int(stmt, 1, user_id) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 1\n");
        return;
    }

    // This time we will use a tree view to display the data
    financial_list_store = gtk_list_store_new(6, G_TYPE_STRING , G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    GtkTreeIter iter;

    // Execute the statement
    int result;
    int balance = 0;
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        // Get the data from the row
        int col_count = sqlite3_column_count(stmt);
        char *column_value[col_count];
        for (int i = 0; i < col_count; i++)
        {
            column_value[i] = (char *)sqlite3_column_text(stmt, i);
        }
        balance = balance + atoi(column_value[5]);
        // Add the data to the tree view
        gtk_list_store_append(financial_list_store, &iter);
        gtk_list_store_set(financial_list_store, &iter, 0, column_value[0], 1, column_value[1], 2, column_value[2], 3, column_value[3], 4, column_value[4], 5, column_value[5], -1);

    }

    //  Add the total balance, first making the balance a string
    char balance_string[256];
    snprintf(balance_string, sizeof(balance_string), "%d", balance);

    gtk_list_store_append(financial_list_store, &iter);
    gtk_list_store_set(financial_list_store, &iter, 0, " ", 1, " ", 2, " ", 3, " ", 4, "Total Expense", 5, balance_string, -1);
    // Finalize the statement
    sqlite3_finalize(stmt);

    // Close the database
    sqlite3_close(db);

    // Create the tree view
    gtk_tree_view_set_model(financial_tree_view, GTK_TREE_MODEL(financial_list_store));
    g_object_unref(financial_list_store);

    // Show the dialog
    gtk_dialog_run(financial_dialog);
}

void hide_financial(GtkButton *button, gpointer user_data)
{
    /*
    Hides the dialog that contains the financial reports
    */
    // Clear the tree view
    gtk_tree_view_set_model(financial_tree_view, NULL);
    // Hide the dialog
    gtk_widget_hide(GTK_WIDGET(financial_dialog));
}

void show_account(GtkButton *button, gpointer user_data)
{
    /*
    Shows the dialog in which you insert the account
    */
    gtk_widget_show_all(GTK_WIDGET(account_dialog));
}

void hide_account(GtkButton *button, gpointer user_data)
{
    /*
    Hides the dialog in which you insert the account
    */
    // Clear the entry and the error
    gtk_entry_set_text(account_entry_entry, "");
    gtk_label_set_text(account_entry_error, "");
    gtk_widget_hide(GTK_WIDGET(account_dialog));
}

void show_statement(GtkButton *button, gpointer user_data)
{
    // We first check whether the accountID is valid
    const char *acc_id = gtk_entry_get_text(account_entry_entry);
    for (int i = 0; i < strlen(acc_id); i++)
    {
        if (acc_id[i] > '9' || acc_id[i] < '0')
        {
            gtk_label_set_text(account_entry_error, "Account ID must be a number");
            return;
        }
    }
    if (strlen(acc_id) > 5)
    {
        gtk_label_set_text(account_entry_error, "Account ID must be at most 5 characters long");
        return;
    }
    int account_id = atoi(acc_id);

    // First checking wether the account exists for this user
    sqlite3 *db1;
    int rc1 = sqlite3_open("banking_app_database.db", &db1);
    if (rc1 != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db1));
        sqlite3_close(db1);
        exit(1);
    }

    sqlite3_stmt *stmt1;
    const char *sql1 = "SELECT AccountID FROM Accounts WHERE UserID = ?";
    if (sqlite3_prepare_v2(db1, sql1, -1, &stmt1, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement hdhashd\n");
        return;
    }

    // Bind parameters
    if (sqlite3_bind_int(stmt1, 1, user_id) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameter 1\n");
        return;
    }

    // Execute the statement
    int result1;
    int ok = 0;
    while ((result1 = sqlite3_step(stmt1)) == SQLITE_ROW)
    {
        int col_count = sqlite3_column_count(stmt1);
        for (int i = 0; i < col_count; i++)
        {
            const char *column_value = (const char *)sqlite3_column_text(stmt1, i);
            if (atoi(column_value) == account_id)
            {
                ok = 1;
                break;
            }
        }
    }
    if(ok == 0)
    {
        // There is no row, so finalize the statement
        sqlite3_finalize(stmt1);
        // Close the database
        sqlite3_close(db1);
        // Display an error message
        gtk_label_set_text(account_entry_error, "Account does not exist");
        return;
    }
    else
    {
        // There is a row, so finalize the statement
        sqlite3_finalize(stmt1);
        // Close the database
        sqlite3_close(db1);

        // Now we put in the financial dialog the statement for the account
        // We first get the initial balance
        sqlite3 *db;
        int rc = sqlite3_open("banking_app_database.db", &db);
        if (rc != SQLITE_OK){
            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            exit(1);
        }

        sqlite3_stmt *stmt;
        const char *sql = "SELECT Amount FROM ActivityLog WHERE AccountID = ? AND ActivityType = 'Account creation'";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
            return;
        }

        // Bind parameters
        if (sqlite3_bind_int(stmt, 1, account_id) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 1\n");
            return;
        }

        // Execute the statement
        int result;
        int balance = 0;
        while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
        {
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++)
            {
                const char *column_value = (const char *)sqlite3_column_text(stmt, i);
                balance = atoi(column_value);
            }
            g_print("a");
        }
        // Finalize the statement
        sqlite3_finalize(stmt);

        // Close the database
        sqlite3_close(db);

        // Now we get the transactions
        sqlite3 *db2;
        int rc2 = sqlite3_open("banking_app_database.db", &db2);
        if (rc2 != SQLITE_OK){
            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db2));
            sqlite3_close(db2);
            exit(1);
        }

        sqlite3_stmt *stmt2;
        const char *sql2 = "SELECT Date, Notes, Amount, TransactionType FROM FinancialTransactions WHERE AccountID = ? ORDER BY substr(Date, 7, 4), substr(Date, 4, 2), substr(Date, 1, 2)";
        
        if (sqlite3_prepare_v2(db2, sql2, -1, &stmt2, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db2));
            return;
        }

        // Bind parameters
        if (sqlite3_bind_int(stmt2, 1, account_id) != SQLITE_OK)
        {
            fprintf(stderr, "Error binding parameter 1\n");
            return;
        }

        // Due to shortage of time I will destroy the old tree size view a bit and make it a bit ugly

        financial_list_store = gtk_list_store_new(6, G_TYPE_STRING , G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
        GtkTreeIter iter;
        gtk_list_store_append(financial_list_store, &iter);
        gtk_list_store_set(financial_list_store, &iter, 0, "DATE", 1, "DESCRIPTION", 2, "AMOUNT", 3, "TYPE", 4, "beta-view", 5, "ACCOUNT BALANCE", -1);
        char balance_string[256];
        snprintf(balance_string, sizeof(balance_string), "%d", balance);
        gtk_list_store_append(financial_list_store, &iter);
        gtk_list_store_set(financial_list_store, &iter, 0, "-", 1, "-", 2, "-", 3, "Account creation", 4, "Initial ammount", 5,balance_string , -1);
        // Execute the statement
        int result1;
        int acc_balance = balance;
        while ((result1 = sqlite3_step(stmt)) == SQLITE_ROW)
        {
            // Get the data from the row
            int col_count = sqlite3_column_count(stmt);
            char *column_value[col_count];
            for (int i = 0; i < col_count; i++)
            {
                column_value[i] = (char *)sqlite3_column_text(stmt, i);
                if (i == 3)
                {
                    if (strcmp(column_value[i], "Deposit") == 0)
                    {
                        acc_balance = acc_balance + atoi(column_value[2]);
                    }
                    else if (strcmp(column_value[i], "Withdrawal") == 0 || strcmp(column_value[i], "Payment") == 0 || strcmp(column_value[i], "Transfer") == 0)
                    {
                        acc_balance = acc_balance - atoi(column_value[2]);
                    }
                }

            }
            char balance_string[256];
            snprintf(balance_string, sizeof(balance_string), "%d", acc_balance);
            // Add the data to the tree view
            gtk_list_store_append(financial_list_store, &iter);
            gtk_list_store_set(financial_list_store, &iter, 0, column_value[0], 1, column_value[1], 2, column_value[2], 3, column_value[3], 4, "-", 5, balance_string, -1);

        }
        // Finalize the statement
        sqlite3_finalize(stmt);

        // Close the database
        sqlite3_close(db);

        // Create the tree view
        gtk_tree_view_set_model(financial_tree_view, GTK_TREE_MODEL(financial_list_store));
        g_object_unref(financial_list_store);

        // Show the dialog
        gtk_dialog_run(financial_dialog);

    }
}

// TODO implement when submiting transaction, instead of just customerID, to also be accountID, different than the first one and to check when submitting if transfer if 
// customer exists and when transfering if account exists maybe
// TODO Find out why balance isnt float and automatically eliminates everything after '.'

// To think about: Should I make also an audit for the user, not just for his accounts, so I may record the creation of the customers and the deletion of accounts? But how? 
// ANSWER TO ABOVE: No, I should not, the project file states that I need to make an audit for the accounts, not for the user, so I will only record the operations done on the accounts

// TODO Think of a way to make Last Name of First Name be the name in case of a company, maybe make one of the two be optional?