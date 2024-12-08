#include <gtk/gtk.h>
#include "PandACW_logic.h"


GtkWidget *entry_name, *entry_phone, *entry_address, *entry_email;  

void on_add_contact_clicked(GtkWidget *widget, gpointer data) {
    const char *name = gtk_entry_get_text(GTK_ENTRY(entry_name));
    const char *phone = gtk_entry_get_text(GTK_ENTRY(entry_phone));
    const char *address = gtk_entry_get_text(GTK_ENTRY(entry_address));
    const char *email = gtk_entry_get_text(GTK_ENTRY(entry_email));

    addContact(name, phone, address, email);

    gtk_entry_set_text(GTK_ENTRY(entry_name), "");
    gtk_entry_set_text(GTK_ENTRY(entry_phone), "");
    gtk_entry_set_text(GTK_ENTRY(entry_address), "");
    gtk_entry_set_text(GTK_ENTRY(entry_email), "");
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkBuilder *builder = gtk_builder_new_from_file("contact_manager.glade");
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    entry_name = GTK_WIDGET(gtk_builder_get_object(builder, "entry_name"));
    entry_phone = GTK_WIDGET(gtk_builder_get_object(builder, "entry_phone"));
    entry_address = GTK_WIDGET(gtk_builder_get_object(builder, "entry_address"));
    entry_email = GTK_WIDGET(gtk_builder_get_object(builder, "entry_email"));

    GtkWidget *add_button = GTK_WIDGET(gtk_builder_get_object(builder, "add_button"));
    g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_contact_clicked), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
