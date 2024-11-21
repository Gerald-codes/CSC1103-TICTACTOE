#include <gtk/gtk.h>
#include "pages.h"

int main(int argc, char *argv[]) {


    gtk_init(&argc, &argv); // Initialize GTK
    load_css(); // Call the load_css function
    initialize_gui();  // Call the main_window function
    gtk_main(); // Start the GTK main loop

    return 0;
}