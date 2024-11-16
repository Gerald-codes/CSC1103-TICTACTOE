//gcc -arch arm64 Main_GUI.c -o Main_GUI $(pkg-config --cflags --libs gtk+-3.0)
//gcc -arch arm64 Main_GUI.c -o Main_GUI $(pkg-config --cflags --libs gtk+-3.0)

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void launch_difficulty_application(GtkWidget *window) {
    gtk_widget_destroy(window); // Close the Main_GUI window
    execl("./difficulty", "./difficulty", (char *)NULL); // Launch the difficulty application
    perror("execl failed"); // Print an error message if execl fails
    exit(1); // Exit if execl fails
}

void launch_TTT_double_application(GtkWidget *window) { // 2 player game
    gtk_widget_destroy(window); // Close the Main_GUI window
    execl("./TTT_double", "./TTT_double", (char *)NULL); // Launch the TTT_GUI2 application
    perror("execl failed"); // Print an error message if execl fails
    exit(1); // Exit if execl fails
}

void setup_difficulty_page(GtkWidget *window) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); // Create a vertical box
    GtkWidget *label = gtk_label_new("Difficulty Page"); // Create a label
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0); // Add the label to the box
    gtk_container_add(GTK_CONTAINER(window), box); // Add the box to the window
    gtk_widget_show_all(window); // Show all widgets in the window
}

void setup_double_player_page(GtkWidget *window) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); // similar to setup_difficulty_page
    GtkWidget *label = gtk_label_new("Double Player Page"); 
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_widget_show_all(window); 
}

void single_player(GtkWidget *widget, gpointer data) {
    g_print("Single Player selected\n"); // Print a message to the console
    GtkWidget *window = gtk_widget_get_toplevel(widget);  
    launch_difficulty_application(window); // Launch the difficulty page
}

void double_player(GtkWidget *widget, gpointer data) {
    g_print("Double Player selected\n"); // similar to single_player
    GtkWidget *window = gtk_widget_get_toplevel(widget); // Get the top-level window
    launch_TTT_double_application(window); // Launch the TTT_GUI2 application 
}

void quit_application(GtkWidget *widget, gpointer data) {
    g_print("Quit selected\n");  // Print a message to the console
    GApplication *app = G_APPLICATION(data); // Get the application
    g_application_quit(app); // close the application
}

void activate(GtkApplication *app, gpointer user_data) { //takes in 2 parameters: a pointer to the GTK_apps instance and generic pointer to any additional user data
    GtkWidget *window; // Declare a GtkWidget pointer called window
    GtkWidget *vbox; // Declare a GtkWidget pointer called box
    GtkWidget *button_single; // Declare a GtkWidget pointer called button_single
    GtkWidget *button_double; // Declare a GtkWidget pointer called button_double
    GtkWidget *button_quit; // Declare a GtkWidget pointer called button_quit

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Main GUI"); // Set the title of the window to Main GUI
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 1000); // Set the default size of the window to 600x1000 pixel

    gtk_widget_set_name(window, "main-window");

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20); // Create a vertical box with spacing
    gtk_widget_set_halign(vbox, GTK_ALIGN_CENTER); // Center the box horizontally
    gtk_widget_set_valign(vbox, GTK_ALIGN_CENTER); // Center the box vertically
    gtk_container_add(GTK_CONTAINER(window), vbox); // Add the box to the window

    button_single = gtk_button_new_with_label("Single Player"); // Create a button with the label "Single Player"
    gtk_widget_set_size_request(button_single, 199, 105); // Set the size of the button
    g_signal_connect(button_single, "clicked", G_CALLBACK(single_player), app); // Connect the "clicked" signal of the button to the single_player function
    gtk_box_pack_start(GTK_BOX(vbox), button_single, FALSE, FALSE, 0); // Add the button to the box

    button_double = gtk_button_new_with_label("Double Player"); // Create a button with the label "Double Player"
    gtk_widget_set_size_request(button_double, 199, 105); // Set the size of the button
    g_signal_connect(button_double, "clicked", G_CALLBACK(double_player), app); // Connect the "clicked" signal of the button to the double_player function
    gtk_box_pack_start(GTK_BOX(vbox), button_double, FALSE, FALSE, 0); // Add the button to the box

    button_quit = gtk_button_new_with_label("Quit"); // Create a button with the label "Quit"
    gtk_widget_set_size_request(button_quit, 199, 105); // Set the size of the button
    g_signal_connect(button_quit, "clicked", G_CALLBACK(quit_application), app); // Connect the "clicked" signal of the button to the quit_application function
    gtk_box_pack_start(GTK_BOX(vbox), button_quit, FALSE, FALSE, 0); // Add the button to the box

    // Load CSS
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);

    gtk_widget_show_all(window); // Show all widgets in the window
}

int main(int argc, char *argv[]) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.MainGUI", G_APPLICATION_DEFAULT_FLAGS); // Create a new GTK application with the ID "com.example.MainGUI"
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL); // Connect the "activate" signal of the application to the activate function
    status = g_application_run(G_APPLICATION(app), argc, argv); // Run the application
    g_object_unref(app); // Free the application object

    return status; // Return the status (0) for successful execution 
}