// Path: src/main.c
#include <gtk/gtk.h>
#include <stdio.h>
#include "/home/ghaster/Proiecte/Project_2/include/gui.h"
#include "/home/ghaster/Proiecte/Project_2/include/database.h"

int main(int argc, char *argv[])
{
    gui_init();

    sqlInit();
    
    return 0;
}
