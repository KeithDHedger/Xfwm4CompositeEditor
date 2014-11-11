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
#define SUBTITLE "Xfwm4 compositor settings"

#define MYEMAIL "kdhedger68713@gmail.com"
#define MYWEBSITE "http://keithhedger.hostingsiteforfree.com"

#define INT 1
#define BOOL 2

#define COMPOSITE "/general/use_compositing"
#define DOCKSHADOW "/general/show_dock_shadow"
#define FRAMESHADOW "/general/show_frame_shadow"
#define POPUPSHADOW "/general/show_popup_shadow"

#define SHADOWOPACITY "/general/shadow_opacity"
#define MOVEOPACITY "/general/move_opacity"
#define RESIZEOPACITY "/general/resize_opacity"
#define POPUPOPACITY "/general/popup_opacity"
#define INACTIVEOPACITY "/general/inactive_opacity"
#define FRAMEOPACITY "/general/frame_opacity"

#define DELTAX "/general/shadow_delta_x"
#define DELTAY "/general/shadow_delta_y"
#define DELTAH "/general/shadow_delta_height"
#define DELTAW "/general/shadow_delta_width"

#define SLEEP 100000

GtkWidget*	window=NULL;

int			shadowOpacity;
int			deltaX;
int			deltaY;
int			deltaW;
int			deltaH;
int			moveOpacity;
int			inactiveOpacity;
int			frameOpacity;
int			resizeOpacity;
int			popupOpacity;
bool		composite;
bool		dockShadow;
bool		frameShadow;
bool		popupShadow;

int			shadowOpacityOrig;
int			deltaXOrig;
int			deltaYOrig;
int			deltaWOrig;
int			deltaHOrig;
int			moveOpacityOrig;
int			inactiveOpacityOrig;
int			frameOpacityOrig;
int			resizeOpacityOrig;
int			popupOpacityOrig;
bool		compositeOrig;
bool		dockShadowOrig;
bool		frameShadowOrig;
bool		popupShadowOrig;


void shutdown(GtkWidget* widget,gpointer data)
{
	gtk_main_quit();
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
				system("xfwm4 --replace &");
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
				setValue(DOCKSHADOW,BOOL,(void*)dockShadow);
				break;
			case 3:
				frameShadow=gtk_toggle_button_get_active(widget);
				setValue(FRAMESHADOW,BOOL,(void*)frameShadow);
				break;
			case 4:
				popupShadow=gtk_toggle_button_get_active(widget);
				setValue(POPUPSHADOW,BOOL,(void*)popupShadow);
				break;
		}
}

void setData(void)
{
	setValue(SHADOWOPACITY,INT,(void*)(long)shadowOpacity);
	setValue(MOVEOPACITY,INT,(void*)(long)moveOpacity);
	setValue(RESIZEOPACITY,INT,(void*)(long)resizeOpacity);
	setValue(POPUPOPACITY,INT,(void*)(long)popupOpacity);
	setValue(FRAMEOPACITY,INT,(void*)(long)frameOpacity);

	setValue(DELTAX,INT,(void*)(long)deltaX);
	setValue(DELTAY,INT,(void*)(long)deltaY);
	setValue(DELTAH,INT,(void*)(long)deltaH);
	setValue(DELTAW,INT,(void*)(long)deltaW);
}

gboolean buttonUp(GtkWidget *widget,GdkEvent *event,gpointer user_data)
{
	setValue(INACTIVEOPACITY,INT,(void*)(long)(inactiveOpacity-1));
	setData();
	usleep(SLEEP);
	setValue(INACTIVEOPACITY,INT,(void*)(long)inactiveOpacity);

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
}

GtkWidget* makeRange(const char* labletext,int low,int high,gpointer data,gpointer resetdata)
{
	GtkWidget*	hbox;
	GtkWidget*	range;
	GtkWidget*	spin;

	hbox=gtk_hbox_new(false,0);
	gtk_box_pack_start(GTK_BOX(hbox),gtk_label_new(labletext),false,false,4);
	spin=gtk_spin_button_new_with_range(low,high,1);
	gtk_spin_button_set_value((GtkSpinButton*)spin,*(int*)data);
	gtk_box_pack_start(GTK_BOX(hbox),spin,false,false,4);
	range=gtk_hscale_new_with_range(low,high,1);
	gtk_range_set_value((GtkRange*)range,*(int*)data);
	gtk_scale_set_draw_value ((GtkScale*)range,false);
	gtk_scale_set_digits((GtkScale*)range,0);

	g_signal_connect(G_OBJECT(range),"button-release-event",G_CALLBACK(buttonUp),NULL);
	g_signal_connect(G_OBJECT(spin),"button-release-event",G_CALLBACK(buttonUp),NULL);
	g_signal_connect(G_OBJECT(spin),"key-release-event",G_CALLBACK(buttonUp),NULL);
	g_signal_connect(G_OBJECT(range),"key-release-event",G_CALLBACK(buttonUp),NULL);
	g_signal_connect(G_OBJECT(range),"value-changed",G_CALLBACK(rangeCallback),(gpointer)spin);
	g_signal_connect(G_OBJECT(spin),"value-changed",G_CALLBACK(rangeCallback),(gpointer)range);

	g_object_set_data(G_OBJECT(range),"my-range-value",data);
	g_object_set_data(G_OBJECT(spin),"my-range-value",data);
	g_object_set_data(G_OBJECT(range),"my-range-value-reset",resetdata);
	g_object_set_data(G_OBJECT(spin),"my-range-value-reset",resetdata);

	gtk_box_pack_start(GTK_BOX(hbox),range,true,true,4);

	return(hbox);
}

void resetControls(GtkWidget *widget,gpointer data)
{
	gpointer	ptr=NULL;

	if(strcmp(G_OBJECT_TYPE_NAME(widget),"GtkCheckButton")==0)
		{
			ptr=g_object_get_data(G_OBJECT(widget),"my-range-value-reset");
			if(ptr!=NULL)
				{
					gtk_toggle_button_set_active((GtkToggleButton*)widget,*((bool*)ptr));
					return;
				}
		}

	if(GTK_IS_CONTAINER(widget))
		gtk_container_foreach((GtkContainer*)widget,resetControls,NULL);
	else
		{
			ptr=g_object_get_data(G_OBJECT(widget),"my-range-value-reset");
			if(ptr!=NULL)
				{
					if(strcmp(G_OBJECT_TYPE_NAME(widget),"GtkSpinButton")==0)
						gtk_spin_button_set_value((GtkSpinButton*)widget,*((int*)ptr));

					if(strcmp(G_OBJECT_TYPE_NAME(widget),"GtkHScale")==0)
						gtk_range_set_value((GtkRange*)widget,*((int*)ptr));

				}
		}
}

void resetData(void)
{
	setValue(SHADOWOPACITY,INT,(void*)(long)shadowOpacityOrig);
	setValue(MOVEOPACITY,INT,(void*)(long)moveOpacityOrig);
	setValue(RESIZEOPACITY,INT,(void*)(long)resizeOpacityOrig);
	setValue(POPUPOPACITY,INT,(void*)(long)popupOpacityOrig);
	setValue(FRAMEOPACITY,INT,(void*)(long)frameOpacityOrig);

	setValue(DELTAX,INT,(void*)(long)deltaXOrig);
	setValue(DELTAY,INT,(void*)(long)deltaYOrig);
	setValue(DELTAH,INT,(void*)(long)deltaHOrig);
	setValue(DELTAW,INT,(void*)(long)deltaWOrig);

	setValue(COMPOSITE,BOOL,(void*)compositeOrig);
	setValue(DOCKSHADOW,BOOL,(void*)dockShadowOrig);
	setValue(FRAMESHADOW,BOOL,(void*)frameShadowOrig);
	setValue(POPUPSHADOW,BOOL,(void*)popupShadowOrig);

	setValue(INACTIVEOPACITY,INT,(void*)(long)(inactiveOpacityOrig-1));
	usleep(SLEEP);
	gtk_container_foreach((GtkContainer*)window,resetControls,NULL);
	setValue(INACTIVEOPACITY,INT,(void*)(long)inactiveOpacityOrig);
}

void init(void)
{
	getValue(COMPOSITE,BOOL,(void*)&composite);
	getValue(DOCKSHADOW,BOOL,(void*)&dockShadow);
	getValue(FRAMESHADOW,BOOL,(void*)&frameShadow);
	getValue(POPUPSHADOW,BOOL,(void*)&popupShadow);

	getValue(SHADOWOPACITY,INT,(void*)&shadowOpacity);
	getValue(MOVEOPACITY,INT,(void*)&moveOpacity);
	getValue(RESIZEOPACITY,INT,(void*)&resizeOpacity);
	getValue(POPUPOPACITY,INT,(void*)&popupOpacity);
	getValue(INACTIVEOPACITY,INT,(void*)&inactiveOpacity);
	getValue(FRAMEOPACITY,INT,(void*)&frameOpacity);

	getValue(DELTAX,INT,(void*)&deltaX);
	getValue(DELTAY,INT,(void*)&deltaY);
	getValue(DELTAH,INT,(void*)&deltaH);
	getValue(DELTAW,INT,(void*)&deltaW);

	shadowOpacityOrig=shadowOpacity;
	deltaXOrig=deltaX;
	deltaYOrig=deltaY;
	deltaWOrig=deltaW;
	deltaHOrig=deltaH;
	moveOpacityOrig=moveOpacity;
	inactiveOpacityOrig=inactiveOpacity;
	frameOpacityOrig=frameOpacity;
	resizeOpacityOrig=resizeOpacity;
	popupOpacityOrig=popupOpacity;
	compositeOrig=composite;
	dockShadowOrig=dockShadow;
	frameShadowOrig=frameShadow;
	popupShadowOrig=popupShadow;
}

void doAbout(void)
{
	const char*	authors[]={"K.D.Hedger <"MYEMAIL">\n",MYWEBSITE,"\nMore by the same author\n","Xfce-Theme-Manager\nhttp://xfce-look.org/content/show.php?content=149647\n","KKEdit\nhttp://gtk-apps.org/content/show.php?content=158161\n","Manpage Editor\nhttp://gtk-apps.org/content/show.php?content=160219\n","GtkSu\nhttp://gtk-apps.org/content/show.php?content=158974",NULL};
	const char	copyright[] ="Copyright \xc2\xa9 2012-2013 K.D.Hedger";
	const char*	aboutboxstring="A config editor for the Xfwm4 compositor";
	char*		licence;

	g_file_get_contents(DATADIR"/docs/gpl-3.0.txt",&licence,NULL,NULL);

	gtk_show_about_dialog(NULL,"authors",authors,"comments",aboutboxstring,"copyright",copyright,"version",VERSION,"website",MYWEBSITE,"program-name","Xfce4 Composite Editor","logo-icon-name","Xfwm4CompositeEditor","license",licence,NULL); 

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
			xfce_titled_dialog_set_subtitle((XfceTitledDialog*)window,SUBTITLE);
			vbox=gtk_dialog_get_content_area((GtkDialog *)window);
#else
			window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
			vbox=gtk_vbox_new(false,0);
			gtk_container_add(GTK_CONTAINER(window),(GtkWidget*)vbox);
#endif

	gtk_window_set_default_size((GtkWindow*)window,400,320);
	g_signal_connect(G_OBJECT(window),"delete-event",G_CALLBACK(shutdown),NULL);
	gtk_window_set_icon_name((GtkWindow*)window,"preferences-desktop-theme");

	gtk_box_pack_start(GTK_BOX(vbox),gtk_label_new("Drop Shadow Config"),false,false,8);

//shadaow opac
	hbox=makeRange("Shadow Opacity:\t",0,200,&shadowOpacity,&shadowOpacityOrig);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);

//delta x
	hbox=makeRange("Delta X:\t\t\t",-64,64,&deltaX,&deltaXOrig);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);

//delta y
	hbox=makeRange("Delta Y:\t\t\t",-64,64,&deltaY,&deltaYOrig);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);

//delta w
	hbox=makeRange("Delta Width:\t\t",-64,64,&deltaW,&deltaWOrig);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);
//delta h
	hbox=makeRange("Delta Height:\t\t",-64,64,&deltaH,&deltaHOrig);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(),false,false,0);
	gtk_box_pack_start(GTK_BOX(vbox),gtk_label_new("Window Opacity"),false,false,8);

//move opac
	hbox=makeRange("Move Opacity:\t\t",0,100,&moveOpacity,&moveOpacityOrig);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);

//inactive opac
	hbox=makeRange("Inactive Opacity:\t",0,100,&inactiveOpacity,&inactiveOpacityOrig);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);
//frame opac
	hbox=makeRange("Frame Opacity:\t",0,100,&frameOpacity,&frameOpacityOrig);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);
//resize opac
	hbox=makeRange("Resize Opacity:\t",0,100,&resizeOpacity,&resizeOpacityOrig);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);
//popup opac
	hbox=makeRange("Pop-up Opacity:\t",0,100,&popupOpacity,&popupOpacityOrig);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(),false,false,0);

	hbox=gtk_hbox_new(false,0);
//composite
		button=gtk_check_button_new_with_label("Composite");
		gtk_toggle_button_set_active((GtkToggleButton*)button,composite);
		gtk_box_pack_start(GTK_BOX(hbox),button,false,false,4);
		g_signal_connect(G_OBJECT(button),"toggled",G_CALLBACK(checkCallback),(gpointer)1);
		g_object_set_data(G_OBJECT(button),"my-range-value-reset",&compositeOrig);
//dock shad
		button=gtk_check_button_new_with_label("Dock Shadow");
		gtk_toggle_button_set_active((GtkToggleButton*)button,dockShadow);
		gtk_box_pack_start(GTK_BOX(hbox),button,false,false,4);
		g_signal_connect(G_OBJECT(button),"toggled",G_CALLBACK(checkCallback),(gpointer)2);
		g_object_set_data(G_OBJECT(button),"my-range-value-reset",&dockShadowOrig);
//frame shad
		button=gtk_check_button_new_with_label("Frame Shadow");
		g_object_set_data(G_OBJECT(button),"my-range-value-reset",&frameShadowOrig);
		gtk_toggle_button_set_active((GtkToggleButton*)button,frameShadow);
		gtk_box_pack_start(GTK_BOX(hbox),button,false,false,4);
		g_signal_connect(G_OBJECT(button),"toggled",G_CALLBACK(checkCallback),(gpointer)3);
//popup shad
		button=gtk_check_button_new_with_label("Pop-Up Shadow");
		gtk_toggle_button_set_active((GtkToggleButton*)button,popupShadow);
		gtk_box_pack_start(GTK_BOX(hbox),button,false,false,4);
		g_signal_connect(G_OBJECT(button),"toggled",G_CALLBACK(checkCallback),(gpointer)4);
		g_object_set_data(G_OBJECT(button),"my-range-value-reset",&popupShadowOrig);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(),false,false,0);

	hbox=gtk_hbox_new(true,0);
//about
		button=gtk_button_new_from_stock(GTK_STOCK_ABOUT);
		g_signal_connect_after(G_OBJECT(button),"clicked",G_CALLBACK(doAbout),NULL);
		gtk_box_pack_start(GTK_BOX(hbox),button,false,false,4);

//restart
		button=gtk_button_new_with_label("Restart WM");
		gtk_box_pack_start(GTK_BOX(hbox),button,false,false,4);
		g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(buttonCallback),(gpointer)1);
//reset
		button=gtk_button_new_with_label("Reset");
		gtk_box_pack_start(GTK_BOX(hbox),button,false,false,4);
		g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(resetData),NULL);
//quit
		button=gtk_button_new_from_stock(GTK_STOCK_QUIT);
		gtk_box_pack_start(GTK_BOX(hbox),button,false,false,4);
		g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(buttonCallback),(gpointer)3);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,4);

	gtk_widget_show_all(window);
	gtk_main();
}






