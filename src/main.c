// Path: src/main.c
#include <gtk/gtk.h>
#include <stdio.h>
#include "/home/ghaster/Proiecte/Project_2/include/gui.h"
#include "/home/ghaster/Proiecte/Project_2/include/database.h"
#include <sqlite3.h>

int main(int argc, char *argv[])
{
    sqlInit();

    sqlite3 *db;
    int rc = sqlite3_open("banking_app_database.db", &db);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    gui_login_init();
    
    return 0;
}
