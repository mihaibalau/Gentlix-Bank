#ifndef GENTLIX_BANK_GUI_H
#define GENTLIX_BANK_GUI_H

void activate_main_menu(GtkApplication *app, gpointer data);
void show_register_interface(GtkWidget *widget, gpointer data);
void show_login_interface(GtkWidget *widget, gpointer data);
void show_account_interface();
void show_edit_account_interface(GtkApplication *app, gpointer data);
void show_all_transactions_interface(GtkWidget *widget, gpointer data);
void show_new_transaction_interface(GtkWidget *widget, gpointer data);

#endif //GENTLIX_BANK_GUI_H
