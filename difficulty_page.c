// difficulty_page.c

#include "pages.h"

extern GtkWidget *main_menu_window;  // Reference to the main menu window
extern GtkWidget *fixed, *background, *title;  // Declare the fixed container, background image, and title label from startingPage.c
GtkWidget *difficulty_window;

// Callback functions for difficulty buttons
void on_easy_clicked(GtkWidget *widget, gpointer data) {
    g_print("Easy difficulty selected\n");
    show_single_player_page(difficulty_window);  // Show the single player page
}

void on_medium_clicked(GtkWidget *widget, gpointer data) {
    g_print("Medium difficulty selected\n");
    // Code to start the game in Medium mode goes here
    // Algorithm for medium difficulty, run the ML page (see how the CPU can click the button)
    // Navigate to the single page (difficulty mode as a parameter)
}

void on_hard_clicked(GtkWidget *widget, gpointer data) {
    g_print("Hard difficulty selected\n");
    // Code to start the game in Hard mode goes here
}

// Callback function for the "Back" button
void on_back_clicked(GtkWidget *widget, gpointer data) {
    gtk_widget_hide(GTK_WIDGET(data));  // Hide the difficulty window
    gtk_widget_show_all(main_menu_window);  // Show the main menu window
}

// Function to show the difficulty selection page
void show_difficulty_page(GtkWidget *main_menu_window) {
    GtkWidget *fixed, *background, *title;

    // Hide the main menu window
    gtk_widget_hide(main_menu_window);

    // Create the difficulty window
    difficulty_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(difficulty_window), "Difficulty Levels");
    gtk_window_set_default_size(GTK_WINDOW(difficulty_window), 600, 1000);
    gtk_container_set_border_width(GTK_CONTAINER(difficulty_window), 40);

    // Fixed container to allow positioning of widgets
    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(difficulty_window), fixed);

     // Create the label
    title = gtk_label_new("TIC - TAC - TOE");
    gtk_widget_set_name(title, "title_label");  // Set a name for CSS targeting
    gtk_fixed_put(GTK_FIXED(fixed), title, 0, 0);

    // Add the background image
    background = gtk_image_new_from_file("images/bg.png"); // Replace with the actual image path
    gtk_widget_set_name(background, "bg");  // Set a name for CSS targeting
    gtk_fixed_put(GTK_FIXED(fixed), background, 0, 60);

    // // Easy Button
    GtkWidget *easy_button = gtk_button_new_with_label("EASY");
    gtk_widget_set_name(easy_button, "easy_button");  // Set a name for CSS targeting
    g_signal_connect(easy_button, "clicked", G_CALLBACK(on_easy_clicked), NULL);
    gtk_fixed_put(GTK_FIXED(fixed), easy_button, 130, 290);   

    // // Medium Button
    GtkWidget *medium_button = gtk_button_new_with_label("MEDIUM");
    gtk_widget_set_name(medium_button, "medium_button");  // Set a name for CSS targeting
    g_signal_connect(medium_button, "clicked", G_CALLBACK(on_medium_clicked), NULL);
    gtk_fixed_put(GTK_FIXED(fixed), medium_button, 130, 370);   

    // // Hard Button
    GtkWidget *hard_button = gtk_button_new_with_label("HARD");
    gtk_widget_set_name(hard_button, "hard_button");  // Set a name for CSS targeting
    g_signal_connect(hard_button, "clicked", G_CALLBACK(on_hard_clicked), NULL);
    gtk_fixed_put(GTK_FIXED(fixed), hard_button, 130, 450);   

    // Create the "Back" button 
    GtkWidget *back_button = gtk_button_new_with_label("Back");
    GtkWidget *arrow = gtk_image_new_from_file("images/backArrow.png");
    gtk_widget_set_name(back_button, "back_button");  // Set a name for CSS targeting
    gtk_widget_set_name(arrow, "arrow");  // Set a name for CSS targeting
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_clicked), difficulty_window);
    gtk_button_set_image(GTK_BUTTON(back_button),arrow);
    gtk_fixed_put(GTK_FIXED(fixed), back_button, 10, 800);

    // Show all widgets in the difficulty window
    gtk_widget_show_all(difficulty_window);

    // Connect the delete-event signal to quit the application when the difficulty window is closed
    g_signal_connect(difficulty_window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
}
