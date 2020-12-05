#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <math.h>
#include <ctype.h>

#include "../Model/battle_field.h"
#include "../Model/pokemon.h"
#include "../Model/player.h"
#include "../Model/referee.h"

// Make them global

GtkWidget	*window;
GtkWidget	*fixed;
GtkWidget	*frame1;
GtkWidget	*team1;
GtkWidget	*pokemon1;
GtkWidget	*pokemon2;
GtkWidget	*pokemon3;
GtkWidget	*team2;
GtkWidget	*pokemon4;
GtkWidget	*pokemon5;
GtkWidget	*pokemon6;
GtkWidget	*frame2;
GtkWidget	*tittle;
GtkWidget	*hp1;
GtkWidget	*hp2;
GtkWidget	*active1;
GtkWidget	*energy1;
GtkWidget	*active2;
GtkWidget	*energy2;
GtkWidget	*startButton;
GtkWidget	*infoAtacks;
GtkWidget	*labelHp1;
GtkWidget	*labelHp2;
GtkWidget	*labelenergy1;
GtkWidget	*labelenergy2;
GtkWidget	*atacksLabel;
GtkBuilder	*builder;

int main(int argc, char *argv[]) 
{
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file ("Pokemon_gui.glade");
 
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_builder_connect_signals(builder, NULL);

	fixed = GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));
	frame1 = GTK_WIDGET(gtk_builder_get_object(builder, "frame1"));
    frame2 = GTK_WIDGET(gtk_builder_get_object(builder, "frame2"));
	tittle = GTK_WIDGET(gtk_builder_get_object(builder, "tittle"));
    hp1= GTK_WIDGET(gtk_builder_get_object(builder, "hp1"));
    hp2 = GTK_WIDGET(gtk_builder_get_object(builder, "hp2"));
    active1 = GTK_WIDGET(gtk_builder_get_object(builder, "active1"));
    energy1 = GTK_WIDGET(gtk_builder_get_object(builder, "energy1"));
    active2 = GTK_WIDGET(gtk_builder_get_object(builder, "active2"));
    energy2 = GTK_WIDGET(gtk_builder_get_object(builder, "energy2"));
    startButton = GTK_WIDGET(gtk_builder_get_object(builder, "startButton"));
    infoAtacks = GTK_WIDGET(gtk_builder_get_object(builder, "infoAtacks"));
    labelHp1 = GTK_WIDGET(gtk_builder_get_object(builder, "labelHp1"));
    labelHp2 = GTK_WIDGET(gtk_builder_get_object(builder, "labelHp2"));
    labelenergy1 = GTK_WIDGET(gtk_builder_get_object(builder, "labelenergy1"));
    labelenergy2 = GTK_WIDGET(gtk_builder_get_object(builder, "labelenergy2"));
    atacksLabel = GTK_WIDGET(gtk_builder_get_object(builder, "atacksLabel"));

	gtk_widget_show(window);

    gtk_main();

	return EXIT_SUCCESS;
}


void on_entry1_changed(GtkEntry *e) 
{
	char tmp[128];
	sprintf(tmp, "entry=%s", gtk_entry_get_text(e));

}