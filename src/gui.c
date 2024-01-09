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
GtkMenuItem *customers_view;
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
GtkEntry *record_account_entry;
GtkButton *record_exit;
GtkButton *record_submit;
GtkLabel *record_error1;
GtkLabel *record_error2;
GtkLabel *record_error3;
GtkLabel *record_error4;
GtkLabel *record_error5;
GtkLabel *record_error6;


GtkWindow *create_window;
GtkGrid *create_grid;
GtkEntry *create_type_entry;
GtkEntry *create_balance_entry;
GtkLabel *create_error1;
GtkLabel *create_error2;
GtkButton *create_cancel;
GtkButton *create_submit;


GtkWindow *edit_window;
GtkGrid *edit_grid; 
GtkEntry *edit_account_entry;
GtkEntry *edit_type_entry;  
GtkEntry *edit_balance_entry; 
GtkButton *edit_exit;
GtkButton *edit_submit; 
GtkButton *edit_delete;  
GtkLabel *edit_error1;
GtkLabel *edit_error2;  
GtkLabel *edit_error3;


GtkWindow *customer_window;
GtkGrid *customer_grid;
GtkButton *customer_cancel;
GtkButton *customer_change;
GtkButton *customer_create;
GtkEntry *customer_id_entry;
GtkEntry *customer_first_entry;
GtkEntry *customer_last_entry;
GtkEntry *customer_address_entry;
GtkEntry *customer_email_entry;
GtkEntry *customer_phone_entry;
GtkLabel *customer_error1;
GtkLabel *customer_error2;
GtkLabel *customer_error3;
GtkLabel *customer_error4;
GtkLabel *customer_error5;
GtkLabel *customer_error6;


GtkDialog *financial_dialog;
GtkBox *financial_box;
GtkButtonBox *financial_button_box;
GtkButton *financial_close;
GtkTreeView *financial_tree_view;
GtkTreeSelection *financial_selection;
GtkListStore *financial_list_store;
GtkTreeViewColumn *date_column;
GtkTreeViewColumn *client_column;
GtkTreeViewColumn *account_column;
GtkTreeViewColumn *type_column;
GtkTreeViewColumn *description_column;
GtkTreeViewColumn *amount_column;

GtkDialog *account_dialog;
GtkBox *account_box;
GtkButtonBox *account_button_box;
GtkButton *account_show_button;
GtkButton *account_close_button;
GtkBox *account_entrybox;
GtkLabel *account_entry_label;
GtkEntry *account_entry_entry;
GtkLabel *account_entry_error;


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
    customers_view = GTK_MENU_ITEM(gtk_builder_get_object(builder,"customers_view"));
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
    record_account_entry = GTK_ENTRY(gtk_builder_get_object(builder,"record_account_entry"));
    record_exit = GTK_BUTTON(gtk_builder_get_object(builder,"record_exit"));
    record_submit = GTK_BUTTON(gtk_builder_get_object(builder,"record_submit"));
    record_error1 = GTK_LABEL(gtk_builder_get_object(builder,"record_error1"));
    record_error2 = GTK_LABEL(gtk_builder_get_object(builder,"record_error2"));
    record_error3 = GTK_LABEL(gtk_builder_get_object(builder,"record_error3"));
    record_error4 = GTK_LABEL(gtk_builder_get_object(builder,"record_error4"));
    record_error5 = GTK_LABEL(gtk_builder_get_object(builder,"record_error5"));
    record_error6 = GTK_LABEL(gtk_builder_get_object(builder,"record_error6"));


    create_window = GTK_WINDOW(gtk_builder_get_object(builder,"create_window"));
    create_grid = GTK_GRID(gtk_builder_get_object(builder,"create_grid"));
    create_type_entry = GTK_ENTRY(gtk_builder_get_object(builder,"create_type_entry"));
    create_balance_entry = GTK_ENTRY(gtk_builder_get_object(builder,"create_balance_entry"));
    create_error1 = GTK_LABEL(gtk_builder_get_object(builder,"create_error1"));
    create_error2 = GTK_LABEL(gtk_builder_get_object(builder,"create_error2"));
    create_cancel = GTK_BUTTON(gtk_builder_get_object(builder,"create_cancel"));
    create_submit = GTK_BUTTON(gtk_builder_get_object(builder,"create_submit"));


    edit_window = GTK_WINDOW(gtk_builder_get_object(builder,"edit_window"));
    edit_grid = GTK_GRID(gtk_builder_get_object(builder,"edit_grid"));
    edit_account_entry = GTK_ENTRY(gtk_builder_get_object(builder,"edit_account_entry"));
    edit_type_entry = GTK_ENTRY(gtk_builder_get_object(builder,"edit_type_entry"));
    edit_balance_entry = GTK_ENTRY(gtk_builder_get_object(builder,"edit_balance_entry"));
    edit_exit = GTK_BUTTON(gtk_builder_get_object(builder,"edit_exit"));
    edit_submit = GTK_BUTTON(gtk_builder_get_object(builder,"edit_submit"));
    edit_delete = GTK_BUTTON(gtk_builder_get_object(builder,"edit_delete"));
    edit_error1 = GTK_LABEL(gtk_builder_get_object(builder,"edit_error1"));
    edit_error2 = GTK_LABEL(gtk_builder_get_object(builder,"edit_error2"));
    edit_error3 = GTK_LABEL(gtk_builder_get_object(builder,"edit_error3"));


    customer_window = GTK_WINDOW(gtk_builder_get_object(builder,"customer_window"));
    customer_grid = GTK_GRID(gtk_builder_get_object(builder,"customer_grid"));
    customer_cancel = GTK_BUTTON(gtk_builder_get_object(builder,"customer_cancel"));
    customer_change = GTK_BUTTON(gtk_builder_get_object(builder,"customer_change"));
    customer_create = GTK_BUTTON(gtk_builder_get_object(builder,"customer_create"));
    customer_id_entry = GTK_ENTRY(gtk_builder_get_object(builder,"customer_id_entry"));
    customer_first_entry = GTK_ENTRY(gtk_builder_get_object(builder,"customer_first_entry"));
    customer_last_entry = GTK_ENTRY(gtk_builder_get_object(builder,"customer_last_entry"));
    customer_address_entry = GTK_ENTRY(gtk_builder_get_object(builder,"customer_address_entry"));
    customer_email_entry = GTK_ENTRY(gtk_builder_get_object(builder,"customer_email_entry"));
    customer_phone_entry = GTK_ENTRY(gtk_builder_get_object(builder,"customer_phone_entry"));
    customer_error1 = GTK_LABEL(gtk_builder_get_object(builder,"customer_error1"));
    customer_error2 = GTK_LABEL(gtk_builder_get_object(builder,"customer_error2"));
    customer_error3 = GTK_LABEL(gtk_builder_get_object(builder,"customer_error3"));
    customer_error4 = GTK_LABEL(gtk_builder_get_object(builder,"customer_error4"));
    customer_error5 = GTK_LABEL(gtk_builder_get_object(builder,"customer_error5"));
    customer_error6 = GTK_LABEL(gtk_builder_get_object(builder,"customer_error6"));


    financial_dialog = GTK_DIALOG(gtk_builder_get_object(builder,"financial_dialog"));
    financial_box = GTK_BOX(gtk_builder_get_object(builder,"financial_box"));
    financial_button_box = GTK_BUTTON_BOX(gtk_builder_get_object(builder,"financial_button_box"));
    financial_close = GTK_BUTTON(gtk_builder_get_object(builder,"financial_close"));
    financial_tree_view = GTK_TREE_VIEW(gtk_builder_get_object(builder,"financial_tree_view"));
    financial_selection = GTK_TREE_SELECTION(gtk_builder_get_object(builder,"financial_selection"));
    financial_list_store = GTK_LIST_STORE(gtk_builder_get_object(builder,"financial_list_store"));
    date_column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder,"date_column"));
    client_column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder,"client_column"));
    account_column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder,"account_column"));
    type_column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder,"type_column"));
    description_column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder,"description_column"));
    amount_column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder,"amount_column"));


    account_dialog = GTK_DIALOG(gtk_builder_get_object(builder,"account_dialog"));
    account_box = GTK_BOX(gtk_builder_get_object(builder,"account_box"));
    account_button_box = GTK_BUTTON_BOX(gtk_builder_get_object(builder,"account_button_box"));
    account_show_button = GTK_BUTTON(gtk_builder_get_object(builder,"account_show_button"));
    account_close_button = GTK_BUTTON(gtk_builder_get_object(builder,"account_close_button"));
    account_entrybox = GTK_BOX(gtk_builder_get_object(builder,"account_entrybox"));
    account_entry_label = GTK_LABEL(gtk_builder_get_object(builder,"account_entry_label"));
    account_entry_entry = GTK_ENTRY(gtk_builder_get_object(builder,"account_entry_entry"));
    account_entry_error = GTK_LABEL(gtk_builder_get_object(builder,"account_entry_error"));


    GtkCellRenderer *renderer;

    // Date column
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(date_column, renderer, TRUE);
    gtk_tree_view_column_add_attribute(date_column, renderer, "text", 0);

    // Client column
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(client_column, renderer, TRUE);
    gtk_tree_view_column_add_attribute(client_column, renderer, "text", 1);

    // Account column
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(account_column, renderer, TRUE);
    gtk_tree_view_column_add_attribute(account_column, renderer, "text", 2);

    // Type column
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(type_column, renderer, TRUE);
    gtk_tree_view_column_add_attribute(type_column, renderer, "text", 3);

    // Description column
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(description_column, renderer, TRUE);
    gtk_tree_view_column_add_attribute(description_column, renderer, "text", 4);

    // Amount column
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(amount_column, renderer, TRUE);
    gtk_tree_view_column_add_attribute(amount_column, renderer, "text", 5);
    
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
    g_signal_connect(customers_view, "activate", G_CALLBACK(view_customers), NULL);
    g_signal_connect(view_close, "clicked", G_CALLBACK(hide_view), NULL);
    g_signal_connect(main_acc_manage, "clicked", G_CALLBACK(manage_accounts), NULL);
    g_signal_connect(main_acc_delete, "clicked", G_CALLBACK(delete_account), NULL);
    g_signal_connect(main_rec_transactions, "clicked", G_CALLBACK(record_transactions), NULL);
    g_signal_connect(main_cus_data_manage, "clicked", G_CALLBACK(manage_customer_data), NULL);
    g_signal_connect(expense_report, "activate", G_CALLBACK(view_expense_report), NULL);
    g_signal_connect(account_statement, "activate", G_CALLBACK(show_account), NULL);

    // Record window
    g_signal_connect(record_exit, "clicked", G_CALLBACK(hide_record), NULL);
    g_signal_connect(G_OBJECT(record_customer_entry), "changed", G_CALLBACK(validate_record), NULL);
    g_signal_connect(G_OBJECT(record_type_entry), "changed", G_CALLBACK(validate_record), NULL);
    g_signal_connect(G_OBJECT(record_amount_entry), "changed", G_CALLBACK(validate_record), NULL);
    g_signal_connect(G_OBJECT(record_date_entry), "changed", G_CALLBACK(validate_record), NULL);
    g_signal_connect(G_OBJECT(record_notes_entry), "changed", G_CALLBACK(validate_record), NULL);
    g_signal_connect(G_OBJECT(record_account_entry), "changed", G_CALLBACK(validate_record), NULL);
    gtk_widget_set_sensitive(GTK_WIDGET(record_submit), FALSE);
    g_signal_connect(record_submit, "clicked", G_CALLBACK(insert_transaction), NULL);

    // Create window
    g_signal_connect(create_cancel, "clicked", G_CALLBACK(hide_create), NULL);
    g_signal_connect(G_OBJECT(create_type_entry), "changed", G_CALLBACK(validate_create), NULL);
    g_signal_connect(G_OBJECT(create_balance_entry), "changed", G_CALLBACK(validate_create), NULL);
    gtk_widget_set_sensitive(GTK_WIDGET(create_submit), FALSE);
    g_signal_connect(create_submit, "clicked", G_CALLBACK(insert_account), NULL);

    // Edit window
    g_signal_connect(edit_exit, "clicked", G_CALLBACK(hide_edit), NULL);
    g_signal_connect(G_OBJECT(edit_account_entry), "changed", G_CALLBACK(validate_edit), NULL);
    g_signal_connect(G_OBJECT(edit_type_entry), "changed", G_CALLBACK(validate_edit), NULL);
    g_signal_connect(G_OBJECT(edit_balance_entry), "changed", G_CALLBACK(validate_edit), NULL);
    gtk_widget_set_sensitive(GTK_WIDGET(edit_submit), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(edit_delete), FALSE);
    g_signal_connect(edit_submit, "clicked", G_CALLBACK(update_account), NULL);
    g_signal_connect(edit_delete, "clicked", G_CALLBACK(delete_account_edit), NULL);

    // Customer window
    g_signal_connect(customer_cancel, "clicked", G_CALLBACK(hide_customer), NULL);
    g_signal_connect(customer_change, "clicked", G_CALLBACK(change_customer), NULL);
    g_signal_connect(customer_create, "clicked", G_CALLBACK(create_customer), NULL);
    g_signal_connect(G_OBJECT(customer_id_entry), "changed", G_CALLBACK(validate_customer), NULL);
    g_signal_connect(G_OBJECT(customer_first_entry), "changed", G_CALLBACK(validate_customer), NULL);
    g_signal_connect(G_OBJECT(customer_last_entry), "changed", G_CALLBACK(validate_customer), NULL);
    g_signal_connect(G_OBJECT(customer_address_entry), "changed", G_CALLBACK(validate_customer), NULL);
    g_signal_connect(G_OBJECT(customer_email_entry), "changed", G_CALLBACK(validate_customer), NULL);
    g_signal_connect(G_OBJECT(customer_phone_entry), "changed", G_CALLBACK(validate_customer), NULL);
    gtk_widget_set_sensitive(GTK_WIDGET(customer_change), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(customer_create), FALSE);

    // Financial dialog
    g_signal_connect(financial_close, "clicked", G_CALLBACK(hide_financial), NULL);

    // Account dialog
    g_signal_connect(account_show_button, "clicked", G_CALLBACK(show_statement), NULL);
    g_signal_connect(account_close_button, "clicked", G_CALLBACK(hide_account), NULL);

}


