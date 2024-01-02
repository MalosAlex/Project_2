// Path: src/functions.c
#include "/home/ghaster/Proiecte/Project_2/include/functions.h"
#include "/home/ghaster/Proiecte/Project_2/include/gui.h"  
#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
#include <sqlite3.h>
#include <time.h>

void on_login_exit_clicked(GtkButton *button, gpointer user_data)
{
    gtk_widget_hide(GTK_WIDGET(login_window));
    gtk_main_quit();
}

void validate_login(GtkEntry *entry, gpointer user_data)
{
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
    user_id = -1;
    gtk_widget_hide(GTK_WIDGET(main_window));
    gtk_widget_show_all(GTK_WIDGET(login_window));
}

void quit_main_menu(GtkMenuItem *menuitem, gpointer user_data)
{
    gtk_widget_destroy(GTK_WIDGET(main_window));
    gtk_main_quit();
}

void view_accounts(GtkMenuItem *menuitem, gpointer user_data)
{
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
    const char *sql = "SELECT * FROM Accounts WHERE UserID = ?";
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
            const char *sql2 = "SELECT * FROM FinancialTransactions WHERE AccountID = ?";
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
    //Clear the text view
    gtk_text_buffer_set_text(gtk_text_view_get_buffer(view_text), "", -1);
    gtk_widget_hide(GTK_WIDGET(view_dialog));
}

void manage_accounts(GtkButton *button, gpointer user_data)
{
    g_print("Manage accounts\n");
    // TODO Implement this function
}

void delete_account(GtkButton *button, gpointer user_data)
{
    g_print("Delete account\n");
    // TODO Implement this function
}

void record_transactions(GtkButton *button, gpointer user_data)
{
    gtk_widget_hide(GTK_WIDGET(main_window));
    gtk_widget_show_all(GTK_WIDGET(record_window));
}

void manage_customer_data(GtkButton *button, gpointer user_data)
{
    g_print("Manage customer data\n");
    // TODO Implement this function
}

void hide_record(GtkButton *button, gpointer user_data)
{
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
    const char *customer = gtk_entry_get_text(entry);
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
    // in the submit function we check that the account has enough money
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
    const char *date = gtk_entry_get_text(entry);
    int year = 0;
    int month = 0;
    int day = 0;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    if (strlen(date) == 0)
    {
        gtk_label_set_text(record_error4, "The curent date will be used");
        // Remember to implement this in the submit function
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
    else
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

    sqlite3_close(db);
    // Insert the transaction into the database
    insert_transaction_db(customer_id, account_id, type, amount_int, date, notes);

    // Insert the operation into the activity log
    insert_activity_db(account_id, type, amount_int);

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
    

}

void insert_transaction_db(int customer_id, int account_id, const char *type, float amount, const char *date, const char *notes)
{
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



// TODO implement when submiting transaction, instead of just customerID, to also be accountID, different than the first one and to check when submitting if transfer if 
// customer exists and when transfering if account exists maybe
