// Path: src/main.c
#include <gtk/gtk.h>
#include <stdio.h>
#include "gui.h"

int main(int argc, char *argv[])
{
    gui_init();

    // Showing the login window
    gtk_widget_show_all(GTK_WIDGET(login_window));

    // Running the main loop
    gtk_main();

    return 0;
}
