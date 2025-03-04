#include <gtk/gtk.h>

// Create the GTK Application, activate the main window for it and return the run status.
int main(int argc, char *argv[]) {

    GtkApplication * main_application;
    int application_status;

    main_application = gtk_application_new("com.bank.GentlixBank", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(main_application, "activate", G_CALLBACK(activate_main_menu), NULL);
    application_status = g_application_run(G_APPLICATION(main_application), argc, argv);
    g_object_unref(main_application);

    return application_status;
}
