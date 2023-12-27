// Path: src/functions.c
#include "functions.h"
#include "gui.h"  
#include <stdio.h>
#include <gtk/gtk.h>

void on_login_exit_clicked(GtkButton *button, gpointer user_data)
{
    gtk_widget_destroy(GTK_WIDGET(login_window));
    gtk_main_quit();
}
