/*
 *
 * K.D.Hedger 2012 <kdhedger68713@gmail.com>
 *
 */

#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <sys/stat.h>
#include <xfconf/xfconf.h>

#include "config.h"

#ifdef GOT_LIBXFCEUI
#include <libxfce4ui/libxfce4ui.h>
#endif

#define WINDOWNAME "Xfwm4-Composite-Editor"
#define INT 1
#define BOOL 2

#define COMPOSITE "/general/use_compositing"

GtkWidget*	window=NULL;
int			shadowOpacity=100;
int			deltaX=0;
int			deltaY=0;
int			deltaW=0;
int			deltaH=0;
int			moveOpacity;
int			inactiveOpacity;
int			frameOpacity;
int			resizeOpacity;
int			popupOpacity;
bool		composite;
bool		dockShadow;
bool		frameShadow;
bool		popupShadow;

void shutdown(GtkWidget* widget,gpointer data)
{
	gtk_main_quit();
}

gboolean lastev(GtkWidget *widget,GdkEvent *event,gpointer user_data)
{
	printf("XXX\n");
	return(false);
}

void setValue(const char* property,int type,void* data)
{
	XfconfChannel*	channelptr=xfconf_channel_get("xfwm4");

	switch(type)
		{
			case INT:
				xfconf_channel_set_int(channelptr,property,(int)(long)data);
				break;

			case BOOL:
				xfconf_channel_set_bool(channelptr,property,(bool)data);
				break;
		}
}

void getValue(const char* property,int type,void* ptr)
{
	int		intdata;
	bool	booldata;


	XfconfChannel*	channelptr=xfconf_channel_get("xfwm4");

	switch(type)
		{
			case INT:
				intdata=xfconf_channel_get_int(channelptr,property,-1000);
				if(intdata!=-1)
					*(int*)ptr=intdata;
				break;

			case BOOL:
				booldata=xfconf_channel_get_bool(channelptr,property,-1000);
				if(booldata!=-1)
					*(bool*)ptr=booldata;
				break;
		}
}

void buttonCallback(GtkToggleButton* widget,gpointer user_data)
{
	switch((long)user_data)
		{
			case 1:
				
				break;
			case 2:
				
				break;
			case 3:
				shutdown(NULL,NULL);
				break;
		}
}

void checkCallback(GtkToggleButton* widget,gpointer user_data)
{
	switch((long)user_data)
		{
			case 1:
				composite=gtk_toggle_button_get_active(widget);
				setValue(COMPOSITE,BOOL,(void*)composite);
				break;
			case 2:
				dockShadow=gtk_toggle_button_get_active(widget);
				break;
			case 3:
				frameShadow=gtk_toggle_button_get_active(widget);
				break;
			case 4:
				popupShadow=gtk_toggle_button_get_active(widget);
				break;
		}
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

void init(void)
{
	getValue(COMPOSITE,BOOL,(void*)&composite);
}

int main(int argc,char **argv)
{
	GtkWidget*	hbox;
	GtkWidget*	vbox;
	GtkWidget*	button;

	gtk_init(&argc,&argv);
	xfconf_init(NULL);
	init();

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

	gtk_box_pack_start(GTK_BOX(vbox),gtk_label_new("Requires WM restart"),false,false,8);

//shadaow opac
	hbox=makeRange("Shadow Opacity:\t",0,200,&shadowOpacity);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);

//delta x
	hbox=makeRange("Delta X:\t\t\t",-64,64,&deltaX);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);

//delta y
	hbox=makeRange("Delta Y:\t\t\t",-64,64,&deltaY);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);

//delta w
	hbox=makeRange("Delta Width:\t\t",-64,64,&deltaW);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);
//delta h
	hbox=makeRange("Delta Height:\t\t",-64,64,&deltaH);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(),false,false,0);
	gtk_box_pack_start(GTK_BOX(vbox),gtk_label_new("Live Update"),false,false,8);

//move opac
	hbox=makeRange("Move Opacity:\t\t",0,100,&moveOpacity);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);

//inactive opac
	hbox=makeRange("Inactive Opacity:\t",0,100,&inactiveOpacity);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);
//frame opac
	hbox=makeRange("Frame Opacity:\t",0,100,&frameOpacity);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);
//resize opac
	hbox=makeRange("Resize Opacity:\t",0,100,&resizeOpacity);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);
//popup opac
	hbox=makeRange("Pop-up Opacity:\t",0,100,&popupOpacity);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(),false,false,0);

	hbox=gtk_hbox_new(false,0);
//composite
		button=gtk_check_button_new_with_label("Composite");
		gtk_toggle_button_set_active((GtkToggleButton*)button,composite);
		gtk_box_pack_start(GTK_BOX(hbox),button,false,false,4);
		g_signal_connect(G_OBJECT(button),"toggled",G_CALLBACK(checkCallback),(gpointer)1);
//dock shad
		button=gtk_check_button_new_with_label("Dock Shadow");
		gtk_box_pack_start(GTK_BOX(hbox),button,false,false,4);
		g_signal_connect(G_OBJECT(button),"toggled",G_CALLBACK(checkCallback),(gpointer)2);
//frame shad
		button=gtk_check_button_new_with_label("Frame Shadow");
		gtk_box_pack_start(GTK_BOX(hbox),button,false,false,4);
		g_signal_connect(G_OBJECT(button),"toggled",G_CALLBACK(checkCallback),(gpointer)3);
//popup shad
		button=gtk_check_button_new_with_label("Pop-Up Shadow");
		gtk_box_pack_start(GTK_BOX(hbox),button,false,false,4);
		g_signal_connect(G_OBJECT(button),"toggled",G_CALLBACK(checkCallback),(gpointer)4);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(),false,false,0);

	hbox=gtk_hbox_new(true,0);
//restart
		button=gtk_button_new_with_label("Restart WM");
		gtk_box_pack_start(GTK_BOX(hbox),button,false,false,4);
		g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(buttonCallback),(gpointer)1);
//defaults
		button=gtk_button_new_with_label("Defaults");
		gtk_box_pack_start(GTK_BOX(hbox),button,false,false,4);
		g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(buttonCallback),(gpointer)2);
//quit
		button=gtk_button_new_from_stock(GTK_STOCK_QUIT);
		gtk_box_pack_start(GTK_BOX(hbox),button,false,false,4);
		g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(buttonCallback),(gpointer)3);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);

	gtk_widget_show_all(window);
	gtk_main();
}






