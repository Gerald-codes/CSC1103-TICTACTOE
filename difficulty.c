//gcc -arch arm64 difficulty.c -o difficulty $(pkg-config --cflags --libs gtk+-3.0)
//gcc -arch arm64 difficulty.c -o difficulty $(pkg-config --cflags --libs gtk+-3.0)

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void easy_mode(GtkWidget *widget, gpointer data) {
    g_print("Easy mode selected\n");
    execl("./TTT_single", "./TTT_single", (char *)NULL); // Launch the easy_mode_app(change to ur file name that u want to execute)
    perror("execl failed"); // Print an error message if execl fails
    exit(1); // Exit if execl fails
}

void medium_mode(GtkWidget *widget, gpointer data) {
    g_print("Medium mode selected\n");
    execl("./medium_mode_app", "./medium_mode_app", (char *)NULL); //  Launch the medium_mode_app (change to ur file name that u want to execute)
    perror("execl failed"); // Print an error message if execl fails
    exit(1); // Exit if execl fails
}

void hard_mode(GtkWidget *widget, gpointer data) {
    g_print("Hard mode selected\n");
    execl("./hard_mode_app", "./hard_mode_app", (char *)NULL); // Launch the hard_mode_app (change to ur file name that u want to execute)
    perror("execl failed"); // Print an error message if execl fails
    exit(1); // Exit if execl fails
}

void back_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Back selected\n");
    GtkWidget *window = gtk_widget_get_toplevel(widget); // Get the top-level window
    gtk_widget_destroy(window); // Close the current window
    execl("./Main_GUI", "./Main_GUI", (char *)NULL); // Launch the Main_GUI application
    perror("execl failed");
    exit(1); // Exit if execl fails
}

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *button_easy;
    GtkWidget *button_medium;
    GtkWidget *button_hard;
    GtkWidget *button_back;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Difficulty");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 1000); // Set the default size of the window to 600x1000

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20); // Create a vertical box with spacing
    gtk_widget_set_halign(vbox, GTK_ALIGN_CENTER); // Center the box horizontally
    gtk_widget_set_valign(vbox, GTK_ALIGN_CENTER); // Center the box vertically
    gtk_container_add(GTK_CONTAINER(window), vbox); // Add the box to the window

    gtk_widget_set_name(window, "main-window");

    button_easy = gtk_button_new_with_label("Easy"); // Create a button with the label "Easy"
    g_signal_connect(button_easy, "clicked", G_CALLBACK(easy_mode), NULL); // Connect the button to the easy_mode function
    gtk_box_pack_start(GTK_BOX(vbox), button_easy, FALSE, FALSE, 0); // Add the button to the box

    button_medium = gtk_button_new_with_label("Medium"); // Create a button with the label "Medium"
    g_signal_connect(button_medium, "clicked", G_CALLBACK(medium_mode), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button_medium, FALSE, FALSE, 0);

    button_hard = gtk_button_new_with_label("Hard");
    g_signal_connect(button_hard, "clicked", G_CALLBACK(hard_mode), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button_hard, FALSE, FALSE, 0);

    button_back = gtk_button_new_with_label("Back");
    g_signal_connect(button_back, "clicked", G_CALLBACK(back_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button_back, FALSE, FALSE, 0);

    // Load CSS
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);

    gtk_widget_show_all(window);
}

int main(int argc, char *argv[]) {
    GtkApplication *app; // Declare a pointer to a GTK application
    int status;

    app = gtk_application_new("com.example.Difficulty", G_APPLICATION_DEFAULT_FLAGS); // Create a new GTK application
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL); // Connect the "activate" signal to the activate function
    status = g_application_run(G_APPLICATION(app), argc, argv); // Run the application
    g_object_unref(app); // Free the application object

    return status; // Return the status
}