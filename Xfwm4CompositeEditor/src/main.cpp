/*
 *
 * K.D.Hedger 2012 <kdhedger68713@gmail.com>
 *
 */

#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <sys/stat.h>
#include "config.h"

#ifdef GOT_LIBXFCEUI
#include <libxfce4ui/libxfce4ui.h>
#endif

#define WINDOWNAME "Xfwm4-Composite-Editor"

GtkWidget *window = NULL;

void shutdown(GtkWidget* widget,gpointer data)
{
	gtk_main_quit();
}

gboolean lastev(GtkWidget *widget,GdkEvent  *event,gpointer   user_data)
{
printf("XXX\n");
return(false);

}

gboolean shadowOpacity(GtkWidget *widget,gpointer user_data)
{
	double	value;
	int val;

	if(strcmp(G_OBJECT_TYPE_NAME(widget),"GtkSpinButton")==0)
		{
			val=gtk_spin_button_get_value_as_int((GtkSpinButton*)widget);
			gtk_range_set_value((GtkRange*)user_data,val);
		}
	else
		{
			val=(int)gtk_range_get_value((GtkRange*)widget);
			gtk_spin_button_set_value((GtkSpinButton*)user_data,val);
		}
	//value=gtk_range_get_value((GtkRange*)widget);
	//gtk_range_set_value((GtkRange*)user_data,value);
	//printf("XXX%f\n",value);
//	printf("ZZ%s\n",G_OBJECT_TYPE_NAME(widget));
	//printf("ZZ%i\n",G_OBJECT_TYPE(widget));
	printf("%i\n",val);
	return(false);
}

int main(int argc,char **argv)
{
	gtk_init(&argc,&argv);

	GtkWidget*	vbox;
	GtkWidget*	hbox;
	GtkWidget*	range;
	GtkWidget*	spin;

#ifdef GOT_LIBXFCEUI
			window=xfce_titled_dialog_new();
			gtk_window_set_title((GtkWindow*)window,WINDOWNAME);
			xfce_titled_dialog_set_subtitle((XfceTitledDialog*)window,WINDOWNAME);
			vbox=gtk_dialog_get_content_area((GtkDialog *)window);
#else
			window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
			vbox=gtk_vbox_new(false,0);
			gtk_container_add(GTK_CONTAINER(window),(GtkWidget*)vbox);
#endif

	gtk_window_set_default_size((GtkWindow*)window,400,320);
	g_signal_connect(G_OBJECT(window),"delete-event",G_CALLBACK(shutdown),NULL);
	gtk_window_set_icon_name((GtkWindow*)window,"preferences-desktop-theme");

//shadaow opac
	hbox=gtk_hbox_new(false,0);
	gtk_box_pack_start(GTK_BOX(hbox),gtk_label_new("Shadow Opacity:\t"),false,false,4);
	spin=gtk_spin_button_new_with_range(0,200,1); 
	gtk_box_pack_start(GTK_BOX(hbox),spin,false,false,4);
	range=gtk_hscale_new_with_range(0,200,1);
	gtk_scale_set_draw_value ((GtkScale*)range,false);
	gtk_scale_set_digits((GtkScale*)range,0);

	g_signal_connect(G_OBJECT(range),"value-changed",G_CALLBACK(shadowOpacity),(gpointer)spin);
	g_signal_connect(G_OBJECT(spin),"value-changed",G_CALLBACK(shadowOpacity),(gpointer)range);

	g_signal_connect(G_OBJECT(range),"button-release-event",G_CALLBACK(lastev),(gpointer)spin);
	g_signal_connect(G_OBJECT(spin),"button-release-event",G_CALLBACK(lastev),(gpointer)range);

	gtk_box_pack_start(GTK_BOX(hbox),range,true,true,4);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);

	gtk_widget_show_all(window);
	gtk_main();
}






