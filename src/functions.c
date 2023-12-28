// Path: src/functions.c
#include "/home/ghaster/Proiecte/Project_2/include/functions.h"
#include "/home/ghaster/Proiecte/Project_2/include/gui.h"  
#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
#include <sqlite3.h>


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
    g_print("Sign out\n");
    gtk_widget_hide(GTK_WIDGET(main_window));
    gtk_widget_show_all(GTK_WIDGET(login_window));
}

void quit_main_menu(GtkMenuItem *menuitem, gpointer user_data)
{
    g_print("Quit\n");
    gtk_widget_destroy(GTK_WIDGET(main_window));
    gtk_main_quit();
}
