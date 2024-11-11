// gcc -o tic_tac_toe_menu tic_tac_toe_menu.c difficulty_page.c `pkg-config --cflags --libs gtk+-3.0`
// tic_tac_toe_menu.c

#include <gtk/gtk.h>
#include "difficulty_page.h"  // Include the header for difficulty page functions
#include "pages.h"

GtkWidget *main_menu_window;  // Declare the main menu window as a global variable
extern GtkWidget *fixed, *background, *title;  // Declare the fixed container, background image, and title label from startingPage.c
extern GtkWidget *starting_window;  // Declare the main window from startingPage.c

// Callback function for the Single Player button in the main menu
void on_single_player_clicked(GtkWidget *widget, gpointer data) {
    show_difficulty_page(main_menu_window);
}

// Callback function for the Double Player button
void on_double_player_clicked(GtkWidget *widget, gpointer data) {
    g_print("Double Player mode selected\n");
    // Code to initialize Double Player mode goes here
}

// Callback function for the Quit button
void on_quit_clicked(GtkWidget *widget, gpointer data) {
    g_print("Quitting the game\n");
    gtk_main_quit();
}

// Function to show the main menu page
void show_main_menu_page(GtkWidget *starting_window){
    GtkWidget *double_button;
    GtkWidget *single_button;

    // Hide the starting window
    gtk_widget_hide(starting_window);

    // Create the main menu window
    main_menu_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_menu_window), "TIC - TAC - TOE");
    gtk_window_set_default_size(GTK_WINDOW(main_menu_window), 300, 500);
    gtk_container_set_border_width(GTK_CONTAINER(main_menu_window), 20);
         
    // Connect the delete-event signal to quit the GTK main loop when the window is closed
    g_signal_connect(main_menu_window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

    // Fixed container to allow positioning of widgets
    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(main_menu_window), fixed);

    // Create the label
    title = gtk_label_new("TIC - TAC - TOE");
    gtk_widget_set_name(title, "title_label");  // Set a name for CSS targeting
    gtk_fixed_put(GTK_FIXED(fixed), title, 0, 0);

    // Add the background image
    background = gtk_image_new_from_file("images/bg.png"); // Replace with the actual image path
    gtk_widget_set_name(background, "bg");  // Set a name for CSS targeting
    gtk_fixed_put(GTK_FIXED(fixed), background, 0, 10);

    // Create the Single Player button
    single_button = gtk_button_new_with_label("SINGLE PLAYER");
    GtkWidget *arrow = gtk_image_new_from_file("images/arrow.png");
    gtk_widget_set_name(single_button, "single_button");  // Set a name for CSS targeting
    gtk_widget_set_name(arrow, "arrow");  // Set a name for CSS targeting
    gtk_button_set_image(GTK_BUTTON(single_button),arrow);
    g_signal_connect(single_button, "clicked", G_CALLBACK(on_single_player_clicked), NULL);
    gtk_fixed_put(GTK_FIXED(fixed), single_button, 55, 185);    

    // Create the Double Player button 
    double_button = gtk_button_new_with_label("DOUBLE PLAYER");
    GtkWidget *arrow1 = gtk_image_new_from_file("images/arrow.png");
    gtk_widget_set_name(double_button, "double_button");  // Set a name for CSS targeting
    gtk_widget_set_name(arrow1, "arrow1");  // Set a name for CSS targeting
    gtk_button_set_image(GTK_BUTTON(double_button),arrow1);
    gtk_button_set_label(GTK_BUTTON(double_button), "DOUBLE PLAYER");
    g_signal_connect(double_button, "clicked", G_CALLBACK(on_double_player_clicked), NULL);
    gtk_fixed_put(GTK_FIXED(fixed), double_button, 55, 240);   

    GtkWidget *quit_button = gtk_button_new_with_label("QUIT GAME");
    gtk_widget_set_name(quit_button, "quit_button");  // Set a name for CSS targeting
    g_signal_connect(quit_button, "clicked", G_CALLBACK(on_quit_clicked), NULL);
    gtk_fixed_put(GTK_FIXED(fixed), quit_button, 55, 370);

    // Show all widgets in the main menu window
    gtk_widget_show_all(main_menu_window);

}
