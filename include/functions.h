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

void hide_edit(GtkButton *button, gpointer user_data);
void validate_edit(GtkEntry *entry, gpointer user_data);
gboolean check_account_id(GtkEntry *entry, gpointer user_data);
gboolean check_edit_acc_type(GtkEntry *entry, gpointer user_data);
gboolean check_edit_balance(GtkEntry *entry, gpointer user_data);
void update_account(GtkButton *button, gpointer user_data);
int update_account_db(int account_id, const char *type, float balance);
int delete_account_db(int account_id);
void delete_account_edit(GtkButton *button, gpointer user_data);
void view_customers(GtkMenuItem *menuitem, gpointer user_data);
void hide_customer(GtkButton *button, gpointer user_data);
void change_customer(GtkButton *button, gpointer user_data);
void create_customer(GtkButton *button, gpointer user_data);
int create_customer_db(const char *first_name, const char *last_name, const char *address, const char *phone_number, const char *email);
void validate_customer(GtkEntry *entry, gpointer user_data);
gboolean check_customer_id(GtkEntry *entry, gpointer user_data);
gboolean check_customer_last(GtkEntry *entry, gpointer user_data);
gboolean check_customer_first(GtkEntry *entry, gpointer user_data);
gboolean check_customer_address(GtkEntry *entry, gpointer user_data);
gboolean check_customer_phone(GtkEntry *entry, gpointer user_data);
gboolean check_customer_email(GtkEntry *entry, gpointer user_data);
void change_customer_db(int customer_id);
void view_expense_report();
void hide_financial(GtkButton *button, gpointer user_data);
#endif