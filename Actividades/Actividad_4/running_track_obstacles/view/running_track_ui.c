#include <gtk/gtk.h>
#include <stdlib.h>
#include "../model/running_track.h"
#include "../model/main.h"

GtkWidget *window;
GtkWidget *grid;

GtkWidget *labels_start_line[NUM_LANES];
GtkWidget *labels_finish_line[NUM_LANES];

GtkWidget *button_start;

static void my_callback(GObject *source_object, GAsyncResult *res,
                        gpointer user_data)
{
   /* Do nothing */
}

static void start_async(GTask *task, gpointer source_object,
                        gpointer task_data, GCancellable *cancellable)
{
    start_race();
}

static void start_clicked()
{
    GCancellable *cancellable = g_cancellable_new();
    GTask *task = g_task_new(g_object_new(G_TYPE_OBJECT, NULL), cancellable, my_callback, NULL);
    g_task_run_in_thread(task, start_async);
    g_object_unref(task);
}

static gboolean draw_running_track(GtkWidget *widget,
                                GdkEventExpose *event,
                                gpointer data)
{
    if (running_track) {

        for (int i = 0; i < NUM_LANES; ++i) {
        
            if (running_track->lanes_start_line[i]) {
                gtk_label_set_text(GTK_LABEL(labels_start_line[i]), "READY!");
            } else {
                gtk_label_set_text(GTK_LABEL(labels_start_line[i]), "-");
            }

            if (running_track->lanes_finish_line[i]) {
                char position[10];
                sprintf(position, "Place: %d", running_track->lanes_finish_line[i]);
                gtk_label_set_text(GTK_LABEL(labels_finish_line[i]), position);
            } else {
                gtk_label_set_text(GTK_LABEL(labels_finish_line[i]), "-");
            }

        }
    }

    return TRUE;
}

static void
activate(GtkApplication *app,
         gpointer user_data)
{

    /* create a new window, and set its title */
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Running Track");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    /* construct the container and sets the rows / columns to be of the same size (homogeneeous) */
    grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    /* Put the grid in the window */
    gtk_container_add(GTK_CONTAINER(window), grid);

    /* create the button and set the event 'clicked' */
    button_start = gtk_button_new_with_label("START");
    g_signal_connect(button_start, "clicked", G_CALLBACK(start_clicked), NULL);

    /* place the button in the grid cell (2, 9), width=2 heigth=1 */
    gtk_grid_attach(GTK_GRID(grid), button_start, 2, 9, 2, 1);

    /* create and attatch labels to grid */
    for (int i = 0; i < NUM_LANES; ++i) {

        labels_start_line[i] = gtk_label_new("-");
        gtk_grid_attach(GTK_GRID(grid), labels_start_line[i], 1, i+1, 1, 1);

        labels_finish_line[i] = gtk_label_new("-");
        gtk_grid_attach(GTK_GRID(grid), labels_finish_line[i], 4, i+1, 1, 1);
    }

    g_timeout_add(33, (GSourceFunc)draw_running_track, (gpointer)window);

    /* Show all the widgets in the window */
    gtk_widget_show_all(window);
}

int main(int argc, char* argv[]) {

    GtkApplication *app;
    int status;

    app = gtk_application_new("example.gtk.running_track", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;

}