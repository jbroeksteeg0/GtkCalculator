#include <iostream>
#include <gtk/gtk.h>
#include <vector>
#include <iomanip>

#include "eval.h"

using namespace std;

GtkWidget *window, *mainGrid, *inputEntry, *answerFrame, *answerLabel;
GtkCssProvider *prov;

vector<GtkWidget*> buttons(20, NULL);
const string labels[] = {"1","2","3","4","5","6","7","8","9","0",".","^","","","*","/","+","-","(",")"};

pair<int64_t,int64_t> getWindowSize() {
    GtkRequisition *minSize, *actual;
    // gtk_widget_size_request(window, req);
    gtk_widget_get_preferred_size(window, minSize, actual);

    return make_pair(actual->width, actual->height);
}

const gchar* css = "window {\nfont-size: 30px;\n}";

string addCommas(string input) {
    if (!isNumber(input)){return input;}
    int end = input.length();
    bool flag = false;
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == '.') {end = i; flag=true; break;}
    }
    string ret = (flag ? "." : "");
    int cnt = 0;
    for (int i = end+1; i < input.length(); i++) {ret += input[i];}

    for (int i = end-1; i >= 0; i--) {
        ret = input[i] + ret;
        if (cnt==2 && i!=0) {
            ret = "," + ret;
            cnt=-1;
        }
        cnt++;
    }
    return ret;
}

void buttonEnter(GtkButton *button, gpointer data) {
    string* entered = (string*)data;
    string current = gtk_entry_get_text(GTK_ENTRY(inputEntry));
    gtk_entry_set_text(GTK_ENTRY(inputEntry), (current+*entered).c_str());
    gtk_label_set_text(GTK_LABEL(answerLabel), ("= " + addCommas(evaluate(current+*entered))).c_str());
    gtk_widget_show_all(window);
}

void typeCallback(GtkEditable *widget, gpointer data) {
    string current = gtk_entry_get_text(GTK_ENTRY(inputEntry));
    gtk_label_set_text(GTK_LABEL(answerLabel), ("= " + addCommas(evaluate(current))).c_str());
    gtk_widget_show_all(window);
}
void buttonClear(GtkButton *button, gpointer data) {
    gtk_entry_set_text(GTK_ENTRY(inputEntry), "");
    gtk_widget_show_all(window);
}
void buttonBackspace(GtkButton *button, gpointer data) {
    string nStr = "";
    string current = gtk_entry_get_text(GTK_ENTRY(inputEntry));
    for (int64_t i = 0; i < ((int64_t)current.length())-1; i++) {
        nStr += current[i];
    }
    gtk_entry_set_text(GTK_ENTRY(inputEntry), nStr.c_str());
    gtk_widget_show_all(window);
}

void onActivate(GtkApplication *app, gpointer data) {
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    prov = gtk_css_provider_new();
    // gtk_css_provider_load_from_path(prov, "./style.css", NULL);
    gtk_css_provider_load_from_data(prov, css, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(prov), GTK_STYLE_PROVIDER_PRIORITY_USER);

    mainGrid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), mainGrid);

    inputEntry = gtk_entry_new();
    // answerFrame = gtk_frame_new(NULL);
    answerFrame = gtk_scrolled_window_new(NULL, NULL);

    answerLabel = gtk_label_new("=");
    gtk_container_add(GTK_CONTAINER(answerFrame), answerLabel);

    gtk_label_set_xalign(GTK_LABEL(answerLabel), 0.02);
    
    gtk_grid_attach(GTK_GRID(mainGrid), inputEntry, 0,0,5,1);
    gtk_grid_attach(GTK_GRID(mainGrid), answerFrame, 0,1,5,1);

    g_signal_connect(inputEntry, "changed", G_CALLBACK(typeCallback), NULL);
    gtk_widget_set_size_request(inputEntry, 640, 75);
    gtk_widget_set_size_request(answerFrame, 640, 75);
    gtk_widget_set_size_request(mainGrid, 640, 480);

    gtk_widget_set_vexpand(mainGrid, TRUE);

    buttons[0] = gtk_button_new_with_label("1");
    gtk_grid_attach(GTK_GRID(mainGrid), buttons[0],0,2,1,1);
    g_signal_connect(buttons[0], "clicked", G_CALLBACK(buttonEnter), (gpointer) &labels[0]);

    buttons[1] = gtk_button_new_with_label("2");
    gtk_grid_attach(GTK_GRID(mainGrid), buttons[1],1,2,1,1);
    g_signal_connect(buttons[1], "clicked", G_CALLBACK(buttonEnter), (gpointer) &labels[1]);

    buttons[2] = gtk_button_new_with_label("3");
    gtk_grid_attach(GTK_GRID(mainGrid), buttons[2],2,2,1,1);
    g_signal_connect(buttons[2], "clicked", G_CALLBACK(buttonEnter), (gpointer) &labels[2]);

    buttons[3] = gtk_button_new_with_label("4");
    gtk_grid_attach(GTK_GRID(mainGrid), buttons[3],0,3,1,1);
    g_signal_connect(buttons[3], "clicked", G_CALLBACK(buttonEnter), (gpointer) &labels[3]);

    buttons[4] = gtk_button_new_with_label("5");
    gtk_grid_attach(GTK_GRID(mainGrid), buttons[4],1,3,1,1);
    g_signal_connect(buttons[4], "clicked", G_CALLBACK(buttonEnter), (gpointer) &labels[4]);

    buttons[5] = gtk_button_new_with_label("6");
    gtk_grid_attach(GTK_GRID(mainGrid), buttons[5],2,3,1,1);
    g_signal_connect(buttons[5], "clicked", G_CALLBACK(buttonEnter), (gpointer) &labels[5]);

    buttons[6] = gtk_button_new_with_label("7");
    gtk_grid_attach(GTK_GRID(mainGrid), buttons[6],0,4,1,1);
    g_signal_connect(buttons[6], "clicked", G_CALLBACK(buttonEnter), (gpointer) &labels[6]);

    buttons[7] = gtk_button_new_with_label("8");
    gtk_grid_attach(GTK_GRID(mainGrid), buttons[7],1,4,1,1);
    g_signal_connect(buttons[7], "clicked", G_CALLBACK(buttonEnter), (gpointer) &labels[7]);

    buttons[8] = gtk_button_new_with_label("9");
    gtk_grid_attach(GTK_GRID(mainGrid), buttons[8],2,4,1,1);
    g_signal_connect(buttons[8], "clicked", G_CALLBACK(buttonEnter), (gpointer) &labels[8]);

    buttons[9] = gtk_button_new_with_label("0");
    gtk_grid_attach(GTK_GRID(mainGrid), buttons[9],0,5,1,1);
    g_signal_connect(buttons[9], "clicked", G_CALLBACK(buttonEnter), (gpointer) &labels[9]);

    buttons[10] = gtk_button_new_with_label(".");
    gtk_grid_attach(GTK_GRID(mainGrid), buttons[10],1,5,1,1);
    g_signal_connect(buttons[10], "clicked", G_CALLBACK(buttonEnter), (gpointer) &labels[10]);

    buttons[11] = gtk_button_new_with_label("^");
    gtk_grid_attach(GTK_GRID(mainGrid), buttons[11],2,5,1,1);
    g_signal_connect(buttons[11], "clicked", G_CALLBACK(buttonEnter), (gpointer) &labels[11]);

    buttons[12] = gtk_button_new_with_label("C");
    gtk_grid_attach(GTK_GRID(mainGrid), buttons[12],3,2,1,1);
    g_signal_connect(buttons[12], "clicked", G_CALLBACK(buttonClear), NULL);
    buttons[13] = gtk_button_new_with_label("\342\214\253");
    gtk_grid_attach(GTK_GRID(mainGrid), buttons[13],4,2,1,1);
    g_signal_connect(buttons[13], "clicked", G_CALLBACK(buttonBackspace), NULL);

    buttons[14] = gtk_button_new_with_label("*");
    gtk_grid_attach(GTK_GRID(mainGrid), buttons[14],3,3,1,1);
    g_signal_connect(buttons[14], "clicked", G_CALLBACK(buttonEnter), (gpointer) &labels[14]);
    buttons[15] = gtk_button_new_with_label("/");
    gtk_grid_attach(GTK_GRID(mainGrid), buttons[15],4,3,1,1);
    g_signal_connect(buttons[15], "clicked", G_CALLBACK(buttonEnter), (gpointer) &labels[15]);

    buttons[16] = gtk_button_new_with_label("+");
    gtk_grid_attach(GTK_GRID(mainGrid), buttons[16],3,4,1,1);
    g_signal_connect(buttons[16], "clicked", G_CALLBACK(buttonEnter), (gpointer) &labels[16]);
    buttons[17] = gtk_button_new_with_label("-");
    gtk_grid_attach(GTK_GRID(mainGrid), buttons[17],4,4,1,1);
    g_signal_connect(buttons[17], "clicked", G_CALLBACK(buttonEnter), (gpointer) &labels[17]);

    buttons[18] = gtk_button_new_with_label("(");
    gtk_grid_attach(GTK_GRID(mainGrid), buttons[18],3,5,1,1);
    g_signal_connect(buttons[18], "clicked", G_CALLBACK(buttonEnter), (gpointer) &labels[18]);
    buttons[19] = gtk_button_new_with_label(")");
    gtk_grid_attach(GTK_GRID(mainGrid), buttons[19],4,5,1,1);
    g_signal_connect(buttons[19], "clicked", G_CALLBACK(buttonEnter), (gpointer) &labels[19]);

    for (int64_t i = 0; i < buttons.size(); i++){
        g_object_set(buttons[i], "expand", TRUE, NULL);
    }
    gtk_widget_show_all(window);
}

int main(int64_t argc, char **argv) {

    GtkApplication *app = gtk_application_new("org.jbroeksteeg.calculator", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(onActivate), NULL);
    g_application_run( G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return 0;
}