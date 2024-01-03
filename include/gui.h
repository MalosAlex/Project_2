#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>

extern int user_id;

// Window declarations
extern GtkWindow *login_window;
extern GtkWindow *main_window;

// Widget declarations
extern GtkGrid *login_grid;
extern GtkLabel *login_welcome;
extern GtkLabel *login_please;
extern GtkLabel *login_username;
extern GtkLabel *login_password;
extern GtkEntry *login_username_entry;
extern GtkEntry *login_password_entry;
extern GtkButton *login_button;
extern GtkButton *register_button;
extern GtkLabel *login_error_username;
extern GtkLabel *login_error_password;
extern GtkButton *login_exit_button;

extern GtkGrid *main_grid;
extern GtkMenuBar *menu_bar;
extern GtkMenuItem *exit_item;
extern GtkMenuItem *data_item;
extern GtkMenuItem *view_item;
extern GtkMenu *exit_menu;
extern GtkMenuItem *main_sign_out;
extern GtkMenuItem *main_quit;
extern GtkMenu *data_menu;
extern GtkMenuItem *csv_import;
extern GtkMenuItem *csv_export;
extern GtkMenuItem *excel_import;
extern GtkMenuItem *excel_export;
extern GtkMenuItem *pdf_import;
extern GtkMenuItem *pdf_export;
extern GtkMenu *view_menu;
extern GtkMenuItem *accounts_view;
extern GtkMenuItem *balance_view;
extern GtkMenuItem *transactions_view;
extern GtkMenuItem *activity_view;
extern GtkMenuItem *customers_view;
extern GtkMenuItem *financial_item;
extern GtkMenu *financial_menu;
extern GtkMenuItem *account_statement;
extern GtkMenuItem *transactions_register;
extern GtkMenuItem *expense_report;
extern GtkButton *main_acc_manage;
extern GtkButton *main_acc_delete;
extern GtkButton *main_rec_transactions;
extern GtkButton *main_cus_data_manage;


extern GtkDialog *view_dialog;
extern GtkBox *view_box;
extern GtkButtonBox *view_button_box;
extern GtkButton *view_close;
extern GtkTextView *view_text;


extern GtkWindow *record_window;
extern GtkGrid *record_grid;
extern GtkEntry *record_please_entry;
extern GtkEntry *record_customer_entry;
extern GtkEntry *record_type_entry;
extern GtkEntry *record_amount_entry;
extern GtkEntry *record_date_entry;
extern GtkEntry *record_notes_entry;
extern GtkEntry *record_account_entry;
extern GtkButton *record_exit;
extern GtkButton *record_submit;
extern GtkLabel *record_error1;
extern GtkLabel *record_error2;
extern GtkLabel *record_error3;
extern GtkLabel *record_error4;
extern GtkLabel *record_error5;
extern GtkLabel *record_error6;


extern GtkWindow *create_window;
extern GtkGrid *create_grid;
extern GtkEntry *create_type_entry;
extern GtkEntry *create_balance_entry;
extern GtkButton *create_cancel;
extern GtkButton *create_submit;
extern GtkLabel *create_error1;
extern GtkLabel *create_error2;


extern GtkWindow *edit_window;
extern GtkGrid *edit_grid;
extern GtkEntry *edit_account_entry;
extern GtkEntry *edit_type_entry;
extern GtkEntry *edit_balance_entry;
extern GtkButton *edit_exit;
extern GtkButton *edit_submit;
extern GtkButton *edit_delete;
extern GtkLabel *edit_error1;
extern GtkLabel *edit_error2;
extern GtkLabel *edit_error3;


extern GtkWindow *customer_window;
extern GtkGrid *customer_grid;
extern GtkButton *customer_cancel;
extern GtkButton *customer_change;
extern GtkButton *customer_create;
extern GtkEntry *customer_id_entry;
extern GtkEntry *customer_first_entry;
extern GtkEntry *customer_last_entry;
extern GtkEntry *customer_address_entry;
extern GtkEntry *customer_email_entry;
extern GtkEntry *customer_phone_entry;
extern GtkLabel *customer_error1;
extern GtkLabel *customer_error2;
extern GtkLabel *customer_error3;
extern GtkLabel *customer_error4;
extern GtkLabel *customer_error5;
extern GtkLabel *customer_error6;


// Function declarations related to the GUI
void gui_init(void);

#endif // GUI_H
