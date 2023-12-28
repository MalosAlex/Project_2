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
#endif