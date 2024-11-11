//gcc -o TTT_GUI TTT_GUI.c -I/opt/homebrew/include/SDL2 -L/opt/homebrew/lib -lSDL2
//gcc -o TTT_GUI2 TTT_GUI2.c -I/opt/homebrew/Cellar/gtk+3/3.24.43/include/gtk-3.0 -L/opt/homebrew/Cellar/glib/2.82.2/include/glib-2.0
//gcc -arch arm64 TTT_GUI2.c -o TTT_GUI2 $(pkg-config --cflags --libs gtk+-3.0)
#include <gtk/gtk.h> 
#include <stdio.h>

static char player = 'X';
static char board[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
static GtkWidget *buttons[9];
static GtkWidget *start_button;
static GtkWidget *end_button;
static GtkWidget *status_label;
static GtkWidget *score_label_x;
static GtkWidget *score_label_o;
static GtkWidget *score_label_ties;
static int score_x = 0;
static int score_o = 0;
static int score_ties = 0;

void update_scoreboard() {
    char score_text[50];
    snprintf(score_text, sizeof(score_text), "Player X: %d", score_x);
    gtk_label_set_text(GTK_LABEL(score_label_x), score_text);
    snprintf(score_text, sizeof(score_text), "Player O: %d", score_o);
    gtk_label_set_text(GTK_LABEL(score_label_o), score_text);
    snprintf(score_text, sizeof(score_text), "Ties: %d", score_ties);
    gtk_label_set_text(GTK_LABEL(score_label_ties), score_text);
}

void reset_board() {
    for (int i = 0; i < 9; i++) {
        board[i] = ' ';
        gtk_button_set_label(GTK_BUTTON(buttons[i]), " ");
    }
    player = 'X';
    gtk_label_set_text(GTK_LABEL(status_label), "Player X's turn");
}

char check_winner() {
    // Check rows
    for (int i = 0; i < 3; i++) {
        if (board[i * 3] == board[i * 3 + 1] && board[i * 3 + 1] == board[i * 3 + 2] && board[i * 3] != ' ') {
            return board[i * 3];
        }
    }
    // Check columns
    for (int i = 0; i < 3; i++) {
        if (board[i] == board[i + 3] && board[i + 3] == board[i + 6] && board[i] != ' ') {
            return board[i];
        }
    }
    // Check diagonals
    if (board[0] == board[4] && board[4] == board[8] && board[0] != ' ') {
        return board[0];
    }
    if (board[2] == board[4] && board[4] == board[6] && board[2] != ' ') {
        return board[2];
    }
    // Check for draw
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ') {
            return ' '; // Game is still ongoing
        }
    }
    return 'D'; // Draw
}

void button_clicked(GtkWidget *widget, gpointer data) {
    int index = GPOINTER_TO_INT(data);
    if (board[index] == ' ') {
        board[index] = player;
        gtk_button_set_label(GTK_BUTTON(widget), &player);
        char winner = check_winner();
        if (winner != ' ') {
            char message[50];
            if (winner == 'D') {
                snprintf(message, sizeof(message), "It's a draw!");
                score_ties++;
            } else {
                snprintf(message, sizeof(message), "Player %c wins!", winner);
                if (winner == 'X') {
                    score_x++;
                } else {
                    score_o++;
                }
            }
            gtk_label_set_text(GTK_LABEL(status_label), message);
            update_scoreboard();
            gtk_widget_set_sensitive(start_button, TRUE);
            gtk_widget_set_sensitive(end_button, FALSE);
            for (int i = 0; i < 9; i++) {
                gtk_widget_set_sensitive(buttons[i], FALSE);
            }
        } else {
            player = (player == 'X') ? 'O' : 'X';
            char message[50];
            snprintf(message, sizeof(message), "Player %c's turn", player);
            gtk_label_set_text(GTK_LABEL(status_label), message);
        }
    }
}

void start_game(GtkWidget *widget, gpointer data) {
    reset_board();
    gtk_widget_set_sensitive(start_button, FALSE);
    gtk_widget_set_sensitive(end_button, TRUE);
    for (int i = 0; i < 9; i++) {
        gtk_widget_set_sensitive(buttons[i], TRUE);
    }
}

void end_game(GtkWidget *widget, gpointer data) {
    gtk_label_set_text(GTK_LABEL(status_label), "Game ended");
    gtk_widget_set_sensitive(start_button, TRUE);
    gtk_widget_set_sensitive(end_button, FALSE);
    for (int i = 0; i < 9; i++) {
        gtk_widget_set_sensitive(buttons[i], FALSE);
    }
}

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *vbox;
    GtkWidget *hbox;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Tic-Tac-Toe");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);

    // Load CSS
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "style.css", NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    status_label = gtk_label_new("Click 'Start Game' to begin");
    gtk_box_pack_start(GTK_BOX(vbox), status_label, FALSE, FALSE, 0);

    grid = gtk_grid_new();
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);

    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    for (int i = 0; i < 9; i++) {
        buttons[i] = gtk_button_new_with_label(" ");
        gtk_widget_set_hexpand(buttons[i], TRUE);
        gtk_widget_set_vexpand(buttons[i], TRUE);
        gtk_grid_attach(GTK_GRID(grid), buttons[i], i % 3, i / 3, 1, 1);
        g_signal_connect(buttons[i], "clicked", G_CALLBACK(button_clicked), GINT_TO_POINTER(i));
        gtk_widget_set_sensitive(buttons[i], FALSE);
    }

    start_button = gtk_button_new_with_label("Start Game");
    gtk_box_pack_start(GTK_BOX(vbox), start_button, FALSE, FALSE, 0);
    g_signal_connect(start_button, "clicked", G_CALLBACK(start_game), NULL);

    end_button = gtk_button_new_with_label("End Game");
    gtk_box_pack_start(GTK_BOX(vbox), end_button, FALSE, FALSE, 0);
    g_signal_connect(end_button, "clicked", G_CALLBACK(end_game), NULL);
    gtk_widget_set_sensitive(end_button, FALSE);

    // Scoreboard
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    score_label_x = gtk_label_new("Player X: 0");
    gtk_box_pack_start(GTK_BOX(hbox), score_label_x, TRUE, TRUE, 0);

    score_label_o = gtk_label_new("Player O: 0");
    gtk_box_pack_start(GTK_BOX(hbox), score_label_o, TRUE, TRUE, 0);

    score_label_ties = gtk_label_new("Ties: 0");
    gtk_box_pack_start(GTK_BOX(hbox), score_label_ties, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.TTT_GUI2", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}