#include <gtk/gtk.h>
#include "pages.h"

GtkWidget *starting_window;  // Declare the main menu window as a global variable
GtkWidget *fixed, *background, *title;

// Function to load CSS styling
void load_css(void){
    const gchar *css_style_file = "style.css";
    GFile *css_fp = g_file_new_for_path(css_style_file);
    GError *error = 0;

    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);
    
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_css_provider_load_from_file(provider, css_fp, &error);
    
    g_object_unref(provider);

}

// Callback for the "Start" button
void on_start_clicked(GtkWidget *widget, gpointer data) {
    g_print("Start button clicked!\n");
    // Show the main menu page
    show_main_menu_page(starting_window);
}

int main(int argc, char *argv[]) {
    GtkImage *start;
    GtkWidget *start_button;

    gtk_init(&argc, &argv);
    load_css();

    // Create the main window
    starting_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(starting_window), "TIC - TAC - TOE");
    gtk_window_set_default_size(GTK_WINDOW(starting_window), 600, 1000);
    // gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(starting_window), 40);


    // Fixed container to allow positioning of widgets
    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(starting_window), fixed);

    // Create the label
    title = gtk_label_new("TIC - TAC - TOE");
    gtk_widget_set_name(title, "title_label");  // Set a name for CSS targeting
    gtk_fixed_put(GTK_FIXED(fixed), title, 0, 0);

    // Add the background image
    background = gtk_image_new_from_file("images/bg.png"); // Replace with the actual image path
    gtk_widget_set_name(background, "bg");  // Set a name for CSS targeting
    gtk_fixed_put(GTK_FIXED(fixed), background, 0, 60);

    // Add the "Start" button 
    start_button = gtk_button_new();
    start = (GtkImage *)gtk_image_new_from_file("images/start.png");
    gtk_widget_set_name(start_button, "start_button");  // Set a name for CSS targeting
    gtk_button_set_image(GTK_BUTTON(start_button), (GtkWidget *)start);
    g_signal_connect(start_button, "clicked", G_CALLBACK(on_start_clicked), NULL);
    gtk_fixed_put(GTK_FIXED(fixed), start_button, 140, 350);

    // Signal to quit the application when the window is closed
    g_signal_connect(starting_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Show all widgets
    gtk_widget_show_all(starting_window);

    GtkSettings *default_settings = gtk_settings_get_default();
    g_object_set(default_settings, "gtk-button-images", TRUE, NULL);

    gtk_main();

    return 0;
}
