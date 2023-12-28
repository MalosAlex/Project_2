#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>

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


// Function declarations related to the GUI
void gui_init(void);

#endif // GUI_H
