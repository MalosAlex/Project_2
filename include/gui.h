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
extern GtkButton *record_exit;
extern GtkButton *record_submit;
extern GtkLabel *record_error1;
extern GtkLabel *record_error2;
extern GtkLabel *record_error3;
extern GtkLabel *record_error4;
extern GtkLabel *record_error5;



// Function declarations related to the GUI
void gui_init(void);

#endif // GUI_H
