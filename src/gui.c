#include "/home/ghaster/Proiecte/Project_2/include/gui.h"
#include <gtk/gtk.h>
#include <stdlib.h>
#include "/home/ghaster/Proiecte/Project_2/include/functions.h"

// Path: src/gui.c

// Widgets declarations
GtkWindow *login_window;
GtkGrid *login_grid;
GtkLabel *login_welcome;
GtkLabel *login_please;
GtkLabel *login_username;
GtkLabel *login_password;
GtkEntry *login_username_entry;
GtkEntry *login_password_entry;
GtkButton *login_button;
GtkButton *register_button;
GtkLabel *login_error_username;
GtkLabel *login_error_password;
GtkButton *login_exit_button;

void gui_init(void)
{
    // Login window
    gtk_init(NULL, NULL);
    GtkBuilder *builder = gtk_builder_new();
    const char *home_directory = getenv("HOME");
    const char *glade_file_path = "/Proiecte/Project_2/resources/project_gui.glade";

    // Combine the home directory and the relative path to get the full path
    char full_path[256];  // Adjust the buffer size as needed
    snprintf(full_path, sizeof(full_path), "%s%s", home_directory, glade_file_path);

    gtk_builder_add_from_file(builder, full_path, NULL);
    
    login_window = GTK_WINDOW(gtk_builder_get_object(builder,"login_window"));
    login_grid = GTK_GRID(gtk_builder_get_object(builder,"login_grid"));
    login_welcome = GTK_LABEL(gtk_builder_get_object(builder,"login_welcome"));
    login_please = GTK_LABEL(gtk_builder_get_object(builder,"login_please"));
    login_username = GTK_LABEL(gtk_builder_get_object(builder,"login_username"));
    login_password = GTK_LABEL(gtk_builder_get_object(builder,"login_password"));
    login_username_entry = GTK_ENTRY(gtk_builder_get_object(builder,"login_username_entry"));
    login_password_entry = GTK_ENTRY(gtk_builder_get_object(builder,"login_password_entry"));
    login_button = GTK_BUTTON(gtk_builder_get_object(builder,"login_button"));
    register_button = GTK_BUTTON(gtk_builder_get_object(builder,"register_button"));
    login_error_username = GTK_LABEL(gtk_builder_get_object(builder,"login_error_username"));
    login_error_password = GTK_LABEL(gtk_builder_get_object(builder,"login_error_password"));
    login_exit_button = GTK_BUTTON(gtk_builder_get_object(builder,"login_exit_button"));


    g_signal_connect(login_exit_button, "clicked", G_CALLBACK(on_login_exit_clicked), NULL);
    g_signal_connect(G_OBJECT(login_username_entry), "changed", G_CALLBACK(check_username), NULL);
    g_signal_connect(G_OBJECT(login_password_entry), "changed", G_CALLBACK(check_password), NULL);
    
    gtk_widget_show_all(GTK_WIDGET(login_window));

    gtk_main();

}
