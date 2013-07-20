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

GtkWidget*	window=NULL;
int			opacity=100;
int			deltaX=0;
int			deltaY=0;

void shutdown(GtkWidget* widget,gpointer data)
{
	gtk_main_quit();
}

gboolean lastev(GtkWidget *widget,GdkEvent  *event,gpointer   user_data)
{
	printf("XXX\n");
	return(false);
}

void rangeCallback(GtkWidget *widget,gpointer user_data)
{
	int val;
	gpointer toval;

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

	toval=g_object_get_data(G_OBJECT(widget),"my-range-value");
	*((int*)toval)=val;
	printf("val=%i\n",val);
	printf("opacity=%i\n",opacity);
	printf("xOffset=%i\n",deltaX);
	printf("yOffset=%i\n",deltaY);
}

GtkWidget* makeRange(const char* labletext,int low,int high,gpointer data)
{
	GtkWidget*	hbox;
	GtkWidget*	range;
	GtkWidget*	spin;

	hbox=gtk_hbox_new(false,0);
	gtk_box_pack_start(GTK_BOX(hbox),gtk_label_new(labletext),false,false,4);
	spin=gtk_spin_button_new_with_range(low,high,1); 
	gtk_box_pack_start(GTK_BOX(hbox),spin,false,false,4);
	range=gtk_hscale_new_with_range(low,high,1);
	gtk_scale_set_draw_value ((GtkScale*)range,false);
	gtk_scale_set_digits((GtkScale*)range,0);

	g_signal_connect(G_OBJECT(range),"value-changed",G_CALLBACK(rangeCallback),(gpointer)spin);
	g_signal_connect(G_OBJECT(spin),"value-changed",G_CALLBACK(rangeCallback),(gpointer)range);
	g_object_set_data(G_OBJECT(range),"my-range-value",data);
	g_object_set_data(G_OBJECT(spin),"my-range-value",data);

	gtk_box_pack_start(GTK_BOX(hbox),range,true,true,4);

	return(hbox);
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

	g_signal_connect(G_OBJECT(range),"value-changed",G_CALLBACK(rangeCallback),(gpointer)spin);
	g_signal_connect(G_OBJECT(spin),"value-changed",G_CALLBACK(rangeCallback),(gpointer)range);
	g_object_set_data(G_OBJECT(range),"my-range-value",(gpointer)&opacity);
	g_object_set_data(G_OBJECT(spin),"my-range-value",(gpointer)&opacity);

	gtk_box_pack_start(GTK_BOX(hbox),range,true,true,4);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);

//delta x
	hbox=gtk_hbox_new(false,0);
	gtk_box_pack_start(GTK_BOX(hbox),gtk_label_new("Delta X:\t\t\t"),false,false,4);
	spin=gtk_spin_button_new_with_range(-64,64,1); 
	gtk_box_pack_start(GTK_BOX(hbox),spin,false,false,4);
	range=gtk_hscale_new_with_range(-64,64,1);
	gtk_scale_set_draw_value ((GtkScale*)range,false);
	gtk_scale_set_digits((GtkScale*)range,0);

	g_signal_connect(G_OBJECT(range),"value-changed",G_CALLBACK(rangeCallback),(gpointer)spin);
	g_signal_connect(G_OBJECT(spin),"value-changed",G_CALLBACK(rangeCallback),(gpointer)range);
	g_object_set_data(G_OBJECT(range),"my-range-value",(gpointer)&deltaX);
	g_object_set_data(G_OBJECT(spin),"my-range-value",(gpointer)&deltaX);

	gtk_box_pack_start(GTK_BOX(hbox),range,true,true,4);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);

//delta y
	hbox=makeRange("Delta Y:\t\t\t",-64,64,&deltaY);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);

	gtk_widget_show_all(window);
	gtk_main();
}






