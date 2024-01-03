#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <gtk/gtk.h>

// Function declarations
void on_login_exit_clicked(GtkButton *button, gpointer user_data);
void validate_login(GtkEntry *entry, gpointer user_data);
gboolean check_username(GtkEntry *entry, gpointer user_data);
gboolean check_password(GtkEntry *entry, gpointer user_data);
gboolean check_login(GtkButton *button, gpointer user_data);
gboolean check_register(GtkButton *button, gpointer user_data);

void sign_out(GtkMenuItem *menuitem, gpointer user_data);
void quit_main_menu(GtkMenuItem *menuitem, gpointer user_data);
void view_accounts(GtkMenuItem *menuitem, gpointer user_data);
void view_balance(GtkMenuItem *menuitem, gpointer user_data);
void view_transactions(GtkMenuItem *menuitem, gpointer user_data);
void view_activity(GtkMenuItem *menuitem, gpointer user_data);
void hide_view(GtkButton *button, gpointer user_data);
void manage_accounts(GtkButton *button, gpointer user_data);
void delete_account(GtkButton *button, gpointer user_data);
void record_transactions(GtkButton *button, gpointer user_data);
void manage_customer_data(GtkButton *button, gpointer user_data);
void hide_record(GtkButton *button, gpointer user_data);

void validate_record(GtkEntry *entry, gpointer user_data);
gboolean check_customer(GtkEntry *entry, gpointer user_data);
gboolean check_type(GtkEntry *entry, gpointer user_data);
gboolean check_amount(GtkEntry *entry, gpointer user_data);
gboolean check_date(GtkEntry *entry, gpointer user_data);
gboolean check_notes(GtkEntry *entry, gpointer user_data);
gboolean check_account(GtkEntry *entry, gpointer user_data);
void insert_transaction(GtkButton *button, gpointer user_data);
void insert_transaction_db(int customer_id, int account_id, const char *type, float amount, const char *date, const char *notes);
void insert_activity_db(int account_id, const char *type, int amount);

void hide_create(GtkButton *button, gpointer user_data);
void validate_create(GtkEntry *entry, gpointer user_data);
gboolean check_acc_type(GtkEntry *entry, gpointer user_data);
gboolean check_balance(GtkEntry *entry, gpointer user_data);
void insert_account(GtkButton *button, gpointer user_data);
int insert_account_db(const char *type, float balance);
#endif