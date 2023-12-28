#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <gtk/gtk.h>

// Function declarations
void on_login_exit_clicked(GtkButton *button, gpointer user_data);
void validate_login(GtkEntry *entry, gpointer user_data);
gboolean check_username(GtkEntry *entry, gpointer user_data);
gboolean check_password(GtkEntry *entry, gpointer user_data);


#endif