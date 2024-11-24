// difficultyPage.c

#include "pages.h"

extern GtkWidget *main_menu_window;  // Reference to the main menu window
extern GtkWidget *fixed, *background, *title;  // Declare the fixed container, background image, and title label from startingPage.c
GtkWidget *difficulty_window;  // Declare the difficulty window

// Callback function for the "Easy" difficulty button
void on_easy_clicked(GtkWidget *widget, gpointer data) {
    static char mode[10] = "EASY";  // Set the game mode to "EASY"
    play_sound("audio/selectAudio.mp3"); // Play a sound effect
    show_single_player_page(difficulty_window, mode);  // Navigate to the single-player page with "EASY" mode
    
}

// Callback function for the "Medium" difficulty button
void on_medium_clicked(GtkWidget *widget, gpointer data) {
    static char mode[10] = "MEDIUM";  // Set the game mode to "MEDIUM"
    play_sound("audio/selectAudio.mp3"); // Play a sound effect
    show_single_player_page(difficulty_window, mode);  // Navigate to the single-player page with "MEDIUM" mode
}

// Callback function for the "Hard" difficulty button
void on_hard_clicked(GtkWidget *widget, gpointer data) {
    static char mode[10] = "HARD";  // Set the game mode to "HARD"
    play_sound("audio/selectAudio.mp3"); // Play a sound effect
    show_single_player_page(difficulty_window, mode);  // Navigate to the single-player page with "HARD" mode
}

// Callback function for the "Back" button
void on_back_clicked(GtkWidget *widget, gpointer data) {
    play_sound("audio/selectAudio.mp3"); // Play a sound effect
    gtk_widget_hide(GTK_WIDGET(data));  // Hide the difficulty selection window
    gtk_widget_show_all(main_menu_window);  // Show the main menu window
}

// Function to display the difficulty selection page
void show_difficulty_page(GtkWidget *main_menu_window) {

    // Hide the main menu window
    gtk_widget_hide(main_menu_window);

    // Create the difficulty selection window
    difficulty_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(difficulty_window), "Difficulty Levels");  // Set window title
    gtk_window_set_default_size(GTK_WINDOW(difficulty_window), 480, 800);  // Set default size of the window
    gtk_container_set_border_width(GTK_CONTAINER(difficulty_window), 40);  // Set padding around the window content

    // Create a fixed container for precise widget placement
    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(difficulty_window), fixed);

    // Create and position the title label
    title = gtk_label_new("TIC - TAC - TOE");
    gtk_widget_set_name(title, "title_label");  // Set a name for CSS targeting
    gtk_fixed_put(GTK_FIXED(fixed), title, 0, 0);

    // Add a background image to the window
    background = gtk_image_new_from_file("images/bg.png");  // Replace with the actual path to the image
    gtk_widget_set_name(background, "bg");  // Set a name for CSS targeting
    gtk_fixed_put(GTK_FIXED(fixed), background, 0, 40);

    // Create the "Easy" button
    GtkWidget *easy_button = gtk_button_new_with_label("EASY");
    gtk_widget_set_name(easy_button, "easy_button");  // Set a name for CSS targeting
    g_signal_connect(easy_button, "clicked", G_CALLBACK(on_easy_clicked), NULL);  // Connect to the "Easy" callback
    gtk_fixed_put(GTK_FIXED(fixed), easy_button, 92, 215);  // Position the button

    // Create the "Medium" button
    GtkWidget *medium_button = gtk_button_new_with_label("MEDIUM");
    gtk_widget_set_name(medium_button, "medium_button");  // Set a name for CSS targeting
    g_signal_connect(medium_button, "clicked", G_CALLBACK(on_medium_clicked), NULL);  // Connect to the "Medium" callback
    gtk_fixed_put(GTK_FIXED(fixed), medium_button, 92, 285);  // Position the button

    // Create the "Hard" button
    GtkWidget *hard_button = gtk_button_new_with_label("HARD");
    gtk_widget_set_name(hard_button, "hard_button");  // Set a name for CSS targeting
    g_signal_connect(hard_button, "clicked", G_CALLBACK(on_hard_clicked), NULL);  // Connect to the "Hard" callback
    gtk_fixed_put(GTK_FIXED(fixed), hard_button, 92, 355);  // Position the button

    // Create the "Back" button with an arrow image
    GtkWidget *back_button = gtk_button_new_with_label("Back");
    GtkWidget *arrow = gtk_image_new_from_file("images/backArrow.png");  // Get the arrow image from file
    gtk_widget_set_name(back_button, "back_button");  // Set a name for CSS targeting
    gtk_widget_set_name(arrow, "arrow");  // Name the image for CSS styling
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_clicked), difficulty_window);  // Connect to the "Back" callback
    gtk_button_set_image(GTK_BUTTON(back_button), arrow);  // Set the arrow image on the button
    gtk_fixed_put(GTK_FIXED(fixed), back_button, 8, 640);  // Position the button

    // Display all widgets in the difficulty selection window
    gtk_widget_show_all(difficulty_window);

    // Connect the delete-event signal to quit the application when the window is closed
    g_signal_connect(difficulty_window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
}
