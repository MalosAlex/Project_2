#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>

// Window declarations
extern GtkWindow *login_window;

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


// Function declarations related to the GUI
void gui_init(void);

#endif // GUI_H
