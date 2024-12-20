#include "pages.h"

GtkWidget *main_menu_window;  // Declare the main menu window as a global variable
extern GtkWidget *fixed, *background, *title;  // Declare the fixed container, background image, and title label from startingPage.c
extern GtkWidget *starting_window;  // Declare the main window from startingPage.c

// Callback function for the Single Player button in the main menu
void on_single_player_clicked(GtkWidget *widget, gpointer data) {
    show_difficulty_page(main_menu_window);
    play_sound("audio/selectAudio.mp3"); // Play a sound effect
}

// Callback function for the Double Player button
void on_double_player_clicked(GtkWidget *widget, gpointer data) {
    show_double_player_page(main_menu_window);
    play_sound("audio/selectAudio.mp3"); // Play a sound effect
}

// Callback function for the Quit button
void on_quit_clicked(GtkWidget *widget, gpointer data) {
    play_sound("audio/selectAudio.mp3"); // Play a sound effect
    gtk_main_quit();
}

// Function to show the main menu page
void show_main_menu_page(GtkWidget *starting_window){
    GtkWidget *double_button;
    GtkWidget *single_button;

    // Hide the starting window
    gtk_widget_hide(starting_window);

    play_song("audio/gameAudio.mp3"); // Play a sound effect

    // Create the main menu window
    main_menu_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_menu_window), "TIC - TAC - TOE");
    gtk_window_set_default_size(GTK_WINDOW(main_menu_window), 480, 800);
    gtk_container_set_border_width(GTK_CONTAINER(main_menu_window), 40);
    gtk_window_set_resizable(GTK_WINDOW(main_menu_window), FALSE); // Enable window fixed size

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
    gtk_fixed_put(GTK_FIXED(fixed), background, 0, 40);

    // Create the Single Player button
    single_button = gtk_button_new_with_label("SINGLE PLAYER");
    GtkWidget *arrow = gtk_image_new_from_file("images/arrow.png");
    gtk_widget_set_name(single_button, "single_button");  // Set a name for CSS targeting
    gtk_widget_set_name(arrow, "arrow");  // Set a name for CSS targeting
    gtk_button_set_image(GTK_BUTTON(single_button),arrow);
    g_signal_connect(single_button, "clicked", G_CALLBACK(on_single_player_clicked), NULL);
    gtk_fixed_put(GTK_FIXED(fixed), single_button, 92, 240);    

    // Create the Double Player button 
    double_button = gtk_button_new_with_label("DOUBLE PLAYER");
    GtkWidget *arrow1 = gtk_image_new_from_file("images/arrow.png");
    gtk_widget_set_name(double_button, "double_button");  // Set a name for CSS targeting
    gtk_widget_set_name(arrow1, "arrow1");  // Set a name for CSS targeting
    gtk_button_set_image(GTK_BUTTON(double_button),arrow1);
    gtk_button_set_label(GTK_BUTTON(double_button), "DOUBLE PLAYER");
    g_signal_connect(double_button, "clicked", G_CALLBACK(on_double_player_clicked), NULL);
    gtk_fixed_put(GTK_FIXED(fixed), double_button, 92, 315);   

    GtkWidget *quit_button = gtk_button_new_with_label("QUIT GAME");
    gtk_widget_set_name(quit_button, "quit_button");  // Set a name for CSS targeting
    g_signal_connect(quit_button, "clicked", G_CALLBACK(on_quit_clicked), NULL);
    gtk_fixed_put(GTK_FIXED(fixed), quit_button, 90, 600);

    // Show all widgets in the main menu window
    gtk_widget_show_all(main_menu_window);
}
