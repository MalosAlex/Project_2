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

GtkWindow *main_window;
GtkGrid *main_grid;
GtkMenuBar *menu_bar;
GtkMenuItem *exit_item;
GtkMenuItem *data_item;
GtkMenuItem *view_item;
GtkMenu *exit_menu;
GtkMenuItem *main_sign_out;
GtkMenuItem *main_quit;
GtkMenu *data_menu;
GtkMenuItem *csv_import;
GtkMenuItem *csv_export;
GtkMenuItem *excel_import;
GtkMenuItem *excel_export;
GtkMenuItem *pdf_import;
GtkMenuItem *pdf_export;
GtkMenu *view_menu;
GtkMenuItem *accounts_view;
GtkMenuItem *balance_view;
GtkMenuItem *transactions_view;
GtkMenuItem *activity_view;
GtkMenuItem *financial_item;
GtkMenu *financial_menu;
GtkMenuItem *account_statement;
GtkMenuItem *transactions_register;
GtkMenuItem *expense_report;
GtkButton *main_acc_manage;
GtkButton *main_acc_delete;
GtkButton *main_rec_transactions;
GtkButton *main_cus_data_manage;


GtkDialog *view_dialog;
GtkBox *view_box;
GtkButtonBox *view_button_box;
GtkButton *view_close;
GtkTextView *view_text;


GtkWindow *record_window;
GtkGrid *record_grid;
GtkEntry *record_customer_entry;
GtkEntry *record_type_entry;
GtkEntry *record_amount_entry;
GtkEntry *record_date_entry;
GtkEntry *record_notes_entry;
GtkButton *record_exit;
GtkButton *record_submit;
GtkLabel *record_error1;
GtkLabel *record_error2;
GtkLabel *record_error3;
GtkLabel *record_error4;
GtkLabel *record_error5;



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

    main_window = GTK_WINDOW(gtk_builder_get_object(builder,"main_window"));
    main_grid = GTK_GRID(gtk_builder_get_object(builder,"main_grid"));
    menu_bar = GTK_MENU_BAR(gtk_builder_get_object(builder,"menu_bar"));
    exit_item = GTK_MENU_ITEM(gtk_builder_get_object(builder,"exit_item"));
    data_item = GTK_MENU_ITEM(gtk_builder_get_object(builder,"data_item"));
    view_item = GTK_MENU_ITEM(gtk_builder_get_object(builder,"view_item"));
    exit_menu = GTK_MENU(gtk_builder_get_object(builder,"exit_menu"));
    main_sign_out = GTK_MENU_ITEM(gtk_builder_get_object(builder,"main_sign_out"));
    main_quit = GTK_MENU_ITEM(gtk_builder_get_object(builder,"main_quit"));
    data_menu = GTK_MENU(gtk_builder_get_object(builder,"data_menu"));
    csv_import = GTK_MENU_ITEM(gtk_builder_get_object(builder,"csv_import"));
    csv_export = GTK_MENU_ITEM(gtk_builder_get_object(builder,"csv_export"));
    excel_import = GTK_MENU_ITEM(gtk_builder_get_object(builder,"excel_import"));
    excel_export = GTK_MENU_ITEM(gtk_builder_get_object(builder,"excel_export"));
    pdf_import = GTK_MENU_ITEM(gtk_builder_get_object(builder,"pdf_import"));
    pdf_export = GTK_MENU_ITEM(gtk_builder_get_object(builder,"pdf_export"));
    view_menu = GTK_MENU(gtk_builder_get_object(builder,"view_menu"));
    accounts_view = GTK_MENU_ITEM(gtk_builder_get_object(builder,"accounts_view"));
    balance_view = GTK_MENU_ITEM(gtk_builder_get_object(builder,"balance_view"));
    transactions_view = GTK_MENU_ITEM(gtk_builder_get_object(builder,"transactions_view"));
    activity_view = GTK_MENU_ITEM(gtk_builder_get_object(builder,"activity_view"));
    financial_item = GTK_MENU_ITEM(gtk_builder_get_object(builder,"financial_item"));
    financial_menu = GTK_MENU(gtk_builder_get_object(builder,"financial_menu"));
    account_statement = GTK_MENU_ITEM(gtk_builder_get_object(builder,"account_statement"));
    transactions_register = GTK_MENU_ITEM(gtk_builder_get_object(builder,"transactions_register"));
    expense_report = GTK_MENU_ITEM(gtk_builder_get_object(builder,"expense_report"));
    main_acc_manage = GTK_BUTTON(gtk_builder_get_object(builder,"main_acc_manage"));
    main_acc_delete = GTK_BUTTON(gtk_builder_get_object(builder,"main_acc_delete"));
    main_rec_transactions = GTK_BUTTON(gtk_builder_get_object(builder,"main_rec_transactions"));
    main_cus_data_manage = GTK_BUTTON(gtk_builder_get_object(builder,"main_cus_data_manage"));


    view_dialog = GTK_DIALOG(gtk_builder_get_object(builder,"view_dialog"));
    view_box = GTK_BOX(gtk_builder_get_object(builder,"view_box"));
    view_button_box = GTK_BUTTON_BOX(gtk_builder_get_object(builder,"view_button_box"));
    view_close = GTK_BUTTON(gtk_builder_get_object(builder,"view_close"));
    view_text = GTK_TEXT_VIEW(gtk_builder_get_object(builder,"view_text"));


    record_window = GTK_WINDOW(gtk_builder_get_object(builder,"record_window"));
    record_grid = GTK_GRID(gtk_builder_get_object(builder,"record_grid"));
    record_customer_entry = GTK_ENTRY(gtk_builder_get_object(builder,"record_customer_entry"));
    record_type_entry = GTK_ENTRY(gtk_builder_get_object(builder,"record_type_entry"));
    record_amount_entry = GTK_ENTRY(gtk_builder_get_object(builder,"record_amount_entry"));
    record_date_entry = GTK_ENTRY(gtk_builder_get_object(builder,"record_date_entry"));
    record_notes_entry = GTK_ENTRY(gtk_builder_get_object(builder,"record_notes_entry"));
    record_exit = GTK_BUTTON(gtk_builder_get_object(builder,"record_exit"));
    record_submit = GTK_BUTTON(gtk_builder_get_object(builder,"record_submit"));
    record_error1 = GTK_LABEL(gtk_builder_get_object(builder,"record_error1"));
    record_error2 = GTK_LABEL(gtk_builder_get_object(builder,"record_error2"));
    record_error3 = GTK_LABEL(gtk_builder_get_object(builder,"record_error3"));
    record_error4 = GTK_LABEL(gtk_builder_get_object(builder,"record_error4"));
    record_error5 = GTK_LABEL(gtk_builder_get_object(builder,"record_error5"));


    // Connect signals
    g_signal_connect(login_exit_button, "clicked", G_CALLBACK(on_login_exit_clicked), NULL);
    g_signal_connect(login_button, "clicked", G_CALLBACK(check_login), NULL);
    g_signal_connect(register_button, "clicked", G_CALLBACK(check_register), NULL);
    g_signal_connect(G_OBJECT(login_username_entry), "changed", G_CALLBACK(validate_login), NULL);
    g_signal_connect(G_OBJECT(login_password_entry), "changed", G_CALLBACK(validate_login), NULL);
    gtk_widget_set_sensitive(GTK_WIDGET(login_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(register_button), FALSE);

    // Main window
    g_signal_connect(main_sign_out, "activate", G_CALLBACK(sign_out), NULL);
    g_signal_connect(main_quit, "activate", G_CALLBACK(quit_main_menu), NULL);
    g_signal_connect(accounts_view, "activate", G_CALLBACK(view_accounts), NULL);
    g_signal_connect(balance_view, "activate", G_CALLBACK(view_balance), NULL);
    g_signal_connect(transactions_view, "activate", G_CALLBACK(view_transactions), NULL);
    g_signal_connect(activity_view, "activate", G_CALLBACK(view_activity), NULL);
    g_signal_connect(view_close, "clicked", G_CALLBACK(hide_view), NULL);
    g_signal_connect(main_acc_manage, "clicked", G_CALLBACK(manage_accounts), NULL);
    g_signal_connect(main_acc_delete, "clicked", G_CALLBACK(delete_account), NULL);
    g_signal_connect(main_rec_transactions, "clicked", G_CALLBACK(record_transactions), NULL);
    g_signal_connect(main_cus_data_manage, "clicked", G_CALLBACK(manage_customer_data), NULL);

    // Record window
    g_signal_connect(record_exit, "clicked", G_CALLBACK(hide_record), NULL);
    g_signal_connect(G_OBJECT(record_customer_entry), "changed", G_CALLBACK(validate_record), NULL);
    g_signal_connect(G_OBJECT(record_type_entry), "changed", G_CALLBACK(validate_record), NULL);
    g_signal_connect(G_OBJECT(record_amount_entry), "changed", G_CALLBACK(validate_record), NULL);
    g_signal_connect(G_OBJECT(record_date_entry), "changed", G_CALLBACK(validate_record), NULL);
    g_signal_connect(G_OBJECT(record_notes_entry), "changed", G_CALLBACK(validate_record), NULL);
    gtk_widget_set_sensitive(GTK_WIDGET(record_submit), FALSE);


}


