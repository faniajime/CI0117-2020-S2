#include <gtk/gtk.h>
#include <stdlib.h>
#include "../controller/referee.c"

GtkWidget *window;
GtkWidget *grid;

static void my_callback(GObject *source_object, GAsyncResult *res,
                        gpointer user_data)
{
   /* Do nothing */
}

static void start_async(GTask *task, gpointer source_object,
                        gpointer task_data, GCancellable *cancellable)
{
    main();
}

static void start_clicked()
{
    GCancellable *cancellable = g_cancellable_new();
    GTask *task = g_task_new(g_object_new(G_TYPE_OBJECT, NULL), cancellable, my_callback, NULL);
    g_task_run_in_thread(task, start_async);
    g_object_unref(task);
}


int main(int argc, char* argv[]) {

    GtkApplication *app;
    int status;

    app = gtk_application_new("Pokemon_battle", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;

}