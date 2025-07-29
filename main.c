#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "bank.h"

static int cmp_acc(const void *a, const void *b) {
    int an = ((const Account*)a)->account_number; int bn = ((const Account*)b)->account_number; return (an - bn);
}
GtkWidget *entry_name, *vb_entry_accnum, *vb_entry_pin;
GtkWidget *dep_entry_accnum, *dep_entry_pin, *dep_entry_amount;
GtkWidget *wit_entry_accnum, *wit_entry_pin, *wit_entry_amount;
GtkWidget *chp_entry_accnum, *chp_entry_pin, *chp_new_pin;
GtkWidget *del_entry_accnum, *del_entry_pin;
GtkWidget *hist_entry_accnum, *hist_entry_pin;
GtkWidget *admin_dialog, *admin_search_entry, *admin_details_box;
typedef struct { int accnum; GtkWidget *parent; } AdminHistContext;
#define MSGBOX(parent, type, text) do { \
    GtkWidget *dl = gtk_message_dialog_new(GTK_WINDOW(parent), GTK_DIALOG_DESTROY_WITH_PARENT, type, GTK_BUTTONS_OK, "%s", text); \
    gtk_dialog_run(GTK_DIALOG(dl)); gtk_widget_destroy(dl); \
} while(0)
static void show_create_account(GtkWidget*, gpointer);
static void on_create_account_submit(GtkWidget*, gpointer);
static void show_view_balance(GtkWidget*, gpointer);
static void on_view_balance_submit(GtkWidget*, gpointer);
static void show_deposit(GtkWidget*, gpointer);
static void on_deposit_submit(GtkWidget*, gpointer);
static void show_withdraw(GtkWidget*, gpointer);
static void on_withdraw_submit(GtkWidget*, gpointer);
static void show_change_pin(GtkWidget*, gpointer);
static void on_change_pin_submit(GtkWidget*, gpointer);
static void show_delete_account(GtkWidget*, gpointer);
static void on_delete_account_submit(GtkWidget*, gpointer);
static void show_admin_login(GtkWidget*, gpointer);
static void on_admin_login_submit(GtkWidget*, gpointer);
static void show_admin_details(GtkWidget*, gpointer);
static void on_admin_search(GtkWidget*, gpointer);
static void on_admin_history(GtkWidget*, gpointer);
static void show_history(GtkWidget *, gpointer);
static void on_history_submit(GtkWidget*, gpointer);

int main(int argc, char *argv[]) {
    srand((unsigned) time(NULL));
    GtkWidget *window, *main_vbox, *spacer_top, *spacer_between;
    GtkWidget *btn_create, *grid_2x4;
    GtkWidget *btn_viewbal, *btn_deposit, *btn_withdraw, *btn_changepin;
    GtkWidget *btn_delete, *btn_detials, *btn_history, *btn_exit;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Banking System");
    gtk_window_set_default_size(GTK_WINDOW(window), 480, 430);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_vbox);

    spacer_top = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(spacer_top, -1, 40);
    gtk_box_pack_start(GTK_BOX(main_vbox), spacer_top, FALSE, FALSE, 0);

    btn_create = gtk_button_new_with_label("Create Account");
    gtk_widget_set_size_request(btn_create, 220, 34);
    gtk_widget_set_halign(btn_create, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(main_vbox), btn_create, FALSE, FALSE, 0);

    spacer_between = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(spacer_between, -1, 16);
    gtk_box_pack_start(GTK_BOX(main_vbox), spacer_between, FALSE, FALSE, 0);

    grid_2x4 = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid_2x4), 13);
    gtk_grid_set_column_spacing(GTK_GRID(grid_2x4), 44);
    gtk_widget_set_halign(grid_2x4, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(main_vbox), grid_2x4, TRUE, FALSE, 0);

    btn_viewbal   = gtk_button_new_with_label("View Balance");
    btn_delete    = gtk_button_new_with_label("Delete Account");
    btn_deposit   = gtk_button_new_with_label("Deposit");
    btn_detials   = gtk_button_new_with_label("Detials (Admin)");
    btn_withdraw  = gtk_button_new_with_label("Withdraw");
    btn_history   = gtk_button_new_with_label("See History");
    btn_changepin = gtk_button_new_with_label("Change PIN");
    btn_exit      = gtk_button_new_with_label("Exit");

    gtk_grid_attach(GTK_GRID(grid_2x4), btn_viewbal,   0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_2x4), btn_delete,    1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_2x4), btn_deposit,   0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_2x4), btn_detials,   1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_2x4), btn_withdraw,  0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_2x4), btn_history,   1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_2x4), btn_changepin, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_2x4), btn_exit,      1, 3, 1, 1);

    g_signal_connect(btn_create,    "clicked", G_CALLBACK(show_create_account), window);
    g_signal_connect(btn_viewbal,   "clicked", G_CALLBACK(show_view_balance), window);
    g_signal_connect(btn_deposit,   "clicked", G_CALLBACK(show_deposit), window);
    g_signal_connect(btn_withdraw,  "clicked", G_CALLBACK(show_withdraw), window);
    g_signal_connect(btn_changepin, "clicked", G_CALLBACK(show_change_pin), window);
    g_signal_connect(btn_delete,    "clicked", G_CALLBACK(show_delete_account), window);
    g_signal_connect(btn_detials,   "clicked", G_CALLBACK(show_admin_login), window);
    g_signal_connect(btn_history,   "clicked", G_CALLBACK(show_history), window);
    g_signal_connect(btn_exit,      "clicked", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}

static void show_create_account(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dialog), "Create New Account");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 250, 90);
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 6);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 6);
    gtk_container_add(GTK_CONTAINER(dialog), grid);
    GtkWidget *label = gtk_label_new("Enter your name:"); entry_name = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), label, 0,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), entry_name, 1,0,2,1);
    GtkWidget *btn = gtk_button_new_with_label("Create");
    gtk_grid_attach(GTK_GRID(grid), btn, 1,1,1,1);
    g_signal_connect(btn, "clicked", G_CALLBACK(on_create_account_submit), dialog);
    gtk_widget_show_all(dialog);
}
static void on_create_account_submit(GtkWidget *widget, gpointer data) {
    const char *uname = gtk_entry_get_text(GTK_ENTRY(entry_name));
    if (strlen(uname) == 0) { MSGBOX(data, GTK_MESSAGE_ERROR, "Please enter a name."); return; }
    int accnum, pin;
    if (!create_account_gui(uname, &accnum, &pin)) { MSGBOX(data, GTK_MESSAGE_ERROR, "File error."); return; }
    char msg[128]; snprintf(msg, sizeof(msg), "Account created!\n\nAccount No: %d\nPIN: %d", accnum, pin);
    MSGBOX(data, GTK_MESSAGE_INFO, msg); gtk_widget_destroy(GTK_WIDGET(data));
}
static void show_view_balance(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dialog), "View Balance");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 320, 130);
    GtkWidget *grid = gtk_grid_new(); gtk_container_add(GTK_CONTAINER(dialog), grid);
    GtkWidget *label1 = gtk_label_new("Account number:");
    vb_entry_accnum = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(vb_entry_accnum), GTK_INPUT_PURPOSE_NUMBER);
    GtkWidget *label2 = gtk_label_new("PIN:");
    vb_entry_pin = gtk_entry_new(); gtk_entry_set_visibility(GTK_ENTRY(vb_entry_pin), FALSE);
    gtk_grid_attach(GTK_GRID(grid), label1, 0,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), vb_entry_accnum, 1,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), label2, 0,1,1,1);
    gtk_grid_attach(GTK_GRID(grid), vb_entry_pin, 1,1,1,1);
    GtkWidget *btn = gtk_button_new_with_label("View");
    gtk_grid_attach(GTK_GRID(grid), btn, 1,2,1,1);
    g_signal_connect(btn, "clicked", G_CALLBACK(on_view_balance_submit), dialog);
    gtk_widget_show_all(dialog);
}
static void on_view_balance_submit(GtkWidget *widget, gpointer data) {
    int accnum, pin;
    if (sscanf(gtk_entry_get_text(GTK_ENTRY(vb_entry_accnum)), "%d", &accnum) != 1 || sscanf(gtk_entry_get_text(GTK_ENTRY(vb_entry_pin)), "%d", &pin) != 1) { MSGBOX(data, GTK_MESSAGE_ERROR, "Enter valid account number and PIN!"); return; }
    Account *acc = login_fast(accnum, pin);
    if (acc) {
        char msg[128]; snprintf(msg, sizeof(msg), "Welcome, %s!\n\nAccount: %d\nBalance: Rs. %.2lf", acc->name, acc->account_number, acc->balance);
        MSGBOX(data, GTK_MESSAGE_INFO, msg); gtk_widget_destroy(GTK_WIDGET(data));
    } else { MSGBOX(data, GTK_MESSAGE_ERROR, "Invalid credentials."); }
}
static void show_deposit(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dialog), "Deposit");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 320, 160);
    GtkWidget *grid = gtk_grid_new(); gtk_container_add(GTK_CONTAINER(dialog), grid);
    GtkWidget *l1 = gtk_label_new("Account number:"); dep_entry_accnum = gtk_entry_new(); gtk_entry_set_input_purpose(GTK_ENTRY(dep_entry_accnum), GTK_INPUT_PURPOSE_NUMBER);
    GtkWidget *l2 = gtk_label_new("PIN:"); dep_entry_pin = gtk_entry_new(); gtk_entry_set_visibility(GTK_ENTRY(dep_entry_pin), FALSE);
    GtkWidget *l3 = gtk_label_new("Amount to deposit:"); dep_entry_amount = gtk_entry_new(); gtk_entry_set_input_purpose(GTK_ENTRY(dep_entry_amount), GTK_INPUT_PURPOSE_NUMBER);
    gtk_grid_attach(GTK_GRID(grid), l1, 0,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), dep_entry_accnum, 1,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), l2, 0,1,1,1);
    gtk_grid_attach(GTK_GRID(grid), dep_entry_pin,    1,1,1,1);
    gtk_grid_attach(GTK_GRID(grid), l3, 0,2,1,1);
    gtk_grid_attach(GTK_GRID(grid), dep_entry_amount, 1,2,1,1);
    GtkWidget *btn = gtk_button_new_with_label("Deposit");
    gtk_grid_attach(GTK_GRID(grid), btn, 1,3,1,1);
    g_signal_connect(btn, "clicked", G_CALLBACK(on_deposit_submit), dialog);
    gtk_widget_show_all(dialog);
}
static void on_deposit_submit(GtkWidget *widget, gpointer data) {
    int accnum, pin; double amt, balance;
    if (sscanf(gtk_entry_get_text(GTK_ENTRY(dep_entry_accnum)), "%d", &accnum) != 1 || sscanf(gtk_entry_get_text(GTK_ENTRY(dep_entry_pin)), "%d", &pin) != 1 || sscanf(gtk_entry_get_text(GTK_ENTRY(dep_entry_amount)), "%lf", &amt) != 1 || amt <= 0) { MSGBOX(data, GTK_MESSAGE_ERROR, "Enter valid account, PIN, and positive amount!"); return; }
    if (deposit_fast(accnum, pin, amt, &balance)) {
        char msg[128]; snprintf(msg, sizeof(msg), "Deposit successful.\nNew balance: Rs. %.2lf", balance);
        MSGBOX(data, GTK_MESSAGE_INFO, msg); gtk_widget_destroy(GTK_WIDGET(data));
    } else { MSGBOX(data, GTK_MESSAGE_ERROR, "Account not found or PIN incorrect."); }
}
static void show_withdraw(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dialog), "Withdraw");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 320, 160);
    GtkWidget *grid = gtk_grid_new(); gtk_container_add(GTK_CONTAINER(dialog), grid);
    GtkWidget *l1 = gtk_label_new("Account number:"); wit_entry_accnum = gtk_entry_new(); gtk_entry_set_input_purpose(GTK_ENTRY(wit_entry_accnum), GTK_INPUT_PURPOSE_NUMBER);
    GtkWidget *l2 = gtk_label_new("PIN:"); wit_entry_pin = gtk_entry_new(); gtk_entry_set_visibility(GTK_ENTRY(wit_entry_pin), FALSE);
    GtkWidget *l3 = gtk_label_new("Amount to withdraw:"); wit_entry_amount = gtk_entry_new(); gtk_entry_set_input_purpose(GTK_ENTRY(wit_entry_amount), GTK_INPUT_PURPOSE_NUMBER);
    gtk_grid_attach(GTK_GRID(grid), l1, 0,0,1,1); gtk_grid_attach(GTK_GRID(grid), wit_entry_accnum, 1,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), l2, 0,1,1,1); gtk_grid_attach(GTK_GRID(grid), wit_entry_pin,    1,1,1,1);
    gtk_grid_attach(GTK_GRID(grid), l3, 0,2,1,1); gtk_grid_attach(GTK_GRID(grid), wit_entry_amount, 1,2,1,1);
    GtkWidget *btn = gtk_button_new_with_label("Withdraw");
    gtk_grid_attach(GTK_GRID(grid), btn, 1,3,1,1);
    g_signal_connect(btn, "clicked", G_CALLBACK(on_withdraw_submit), dialog);
    gtk_widget_show_all(dialog);
}
static void on_withdraw_submit(GtkWidget *widget, gpointer data) {
    int accnum, pin; double amt, balance;
    if (sscanf(gtk_entry_get_text(GTK_ENTRY(wit_entry_accnum)), "%d", &accnum) != 1 || sscanf(gtk_entry_get_text(GTK_ENTRY(wit_entry_pin)), "%d", &pin) != 1 || sscanf(gtk_entry_get_text(GTK_ENTRY(wit_entry_amount)), "%lf", &amt) != 1 || amt <= 0) { MSGBOX(data, GTK_MESSAGE_ERROR, "Enter valid account, PIN, and positive amount!"); return; }
    if (withdraw_fast(accnum, pin, amt, &balance)) {
        char msg[128]; snprintf(msg, sizeof(msg), "Withdraw successful.\nNew balance: Rs. %.2lf", balance);
        MSGBOX(data, GTK_MESSAGE_INFO, msg); gtk_widget_destroy(GTK_WIDGET(data));
    } else { MSGBOX(data, GTK_MESSAGE_ERROR, "Account not found, PIN incorrect, or insufficient funds."); }
}
static void show_change_pin(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dialog), "Change PIN");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 320, 150);
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(dialog), grid);
    GtkWidget *l1 = gtk_label_new("Account number:"); chp_entry_accnum = gtk_entry_new(); gtk_entry_set_input_purpose(GTK_ENTRY(chp_entry_accnum), GTK_INPUT_PURPOSE_NUMBER);
    GtkWidget *l2 = gtk_label_new("Current PIN:"); chp_entry_pin = gtk_entry_new(); gtk_entry_set_visibility(GTK_ENTRY(chp_entry_pin), FALSE);
    GtkWidget *l3 = gtk_label_new("New 4-digit PIN:"); chp_new_pin = gtk_entry_new(); gtk_entry_set_max_length(GTK_ENTRY(chp_new_pin), 4); gtk_entry_set_input_purpose(GTK_ENTRY(chp_new_pin), GTK_INPUT_PURPOSE_NUMBER); gtk_entry_set_visibility(GTK_ENTRY(chp_new_pin), FALSE);
    gtk_grid_attach(GTK_GRID(grid), l1, 0,0,1,1); gtk_grid_attach(GTK_GRID(grid), chp_entry_accnum, 1,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), l2, 0,1,1,1); gtk_grid_attach(GTK_GRID(grid), chp_entry_pin,    1,1,1,1);
    gtk_grid_attach(GTK_GRID(grid), l3, 0,2,1,1); gtk_grid_attach(GTK_GRID(grid), chp_new_pin,      1,2,1,1);
    GtkWidget *btn = gtk_button_new_with_label("Change PIN");
    gtk_grid_attach(GTK_GRID(grid), btn, 1,3,1,1);
    g_signal_connect(btn, "clicked", G_CALLBACK(on_change_pin_submit), dialog);
    gtk_widget_show_all(dialog);
}
static void on_change_pin_submit(GtkWidget *widget, gpointer data) {
    int accnum, pin, newpin;
    if (sscanf(gtk_entry_get_text(GTK_ENTRY(chp_entry_accnum)), "%d", &accnum) != 1 || sscanf(gtk_entry_get_text(GTK_ENTRY(chp_entry_pin)), "%d", &pin) != 1 || sscanf(gtk_entry_get_text(GTK_ENTRY(chp_new_pin)), "%d", &newpin) != 1 || newpin < 1000 || newpin > 9999) { MSGBOX(data, GTK_MESSAGE_ERROR, "Please enter valid details (PIN must be 4 digits)."); return;}
    if (change_pin_fast(accnum, pin, newpin)) { MSGBOX(data, GTK_MESSAGE_INFO, "PIN changed successfully."); gtk_widget_destroy(GTK_WIDGET(data)); }
    else { MSGBOX(data, GTK_MESSAGE_ERROR, "Account not found or current PIN incorrect."); }
}
static void show_delete_account(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dialog), "Delete Account");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 320, 120);
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(dialog), grid);
    GtkWidget *l1 = gtk_label_new("Account number:"); del_entry_accnum = gtk_entry_new(); gtk_entry_set_input_purpose(GTK_ENTRY(del_entry_accnum), GTK_INPUT_PURPOSE_NUMBER);
    GtkWidget *l2 = gtk_label_new("PIN:"); del_entry_pin = gtk_entry_new(); gtk_entry_set_visibility(GTK_ENTRY(del_entry_pin), FALSE);
    gtk_grid_attach(GTK_GRID(grid), l1, 0,0,1,1); gtk_grid_attach(GTK_GRID(grid), del_entry_accnum, 1,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), l2, 0,1,1,1); gtk_grid_attach(GTK_GRID(grid), del_entry_pin,    1,1,1,1);
    GtkWidget *btn = gtk_button_new_with_label("Delete");
    gtk_grid_attach(GTK_GRID(grid), btn, 1,2,1,1);
    g_signal_connect(btn, "clicked", G_CALLBACK(on_delete_account_submit), dialog);
    gtk_widget_show_all(dialog);
}
static void on_delete_account_submit(GtkWidget *widget, gpointer data) {
    int accnum, pin;
    if (sscanf(gtk_entry_get_text(GTK_ENTRY(del_entry_accnum)), "%d", &accnum) != 1 || sscanf(gtk_entry_get_text(GTK_ENTRY(del_entry_pin)), "%d", &pin) != 1) { MSGBOX(data, GTK_MESSAGE_ERROR, "Enter valid account number and PIN!"); return; }
    if (delete_account_fast(accnum, pin)) { MSGBOX(data, GTK_MESSAGE_INFO, "Account deleted successfully."); gtk_widget_destroy(GTK_WIDGET(data)); }
    else { MSGBOX(data, GTK_MESSAGE_ERROR, "Account not found or PIN incorrect."); }
}
static void show_admin_login(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dialog), "Admin Login");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 320, 120);
    GtkWidget *grid = gtk_grid_new(); gtk_container_add(GTK_CONTAINER(dialog), grid);
    GtkWidget *label1 = gtk_label_new("Username:");
    GtkWidget *user = gtk_entry_new();
    GtkWidget *label2 = gtk_label_new("Password:");
    GtkWidget *pass = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(pass), FALSE);
    GtkWidget *btn = gtk_button_new_with_label("Login");
    gtk_grid_attach(GTK_GRID(grid), label1, 0,0,1,1); gtk_grid_attach(GTK_GRID(grid), user, 1,0,2,1);
    gtk_grid_attach(GTK_GRID(grid), label2, 0,1,1,1); gtk_grid_attach(GTK_GRID(grid), pass, 1,1,2,1);
    gtk_grid_attach(GTK_GRID(grid), btn, 1,2,1,1);
    g_signal_connect_swapped(btn, "clicked", G_CALLBACK(on_admin_login_submit), dialog);
    g_object_set_data(G_OBJECT(dialog), "admin_user", user);
    g_object_set_data(G_OBJECT(dialog), "admin_pass", pass);
    gtk_widget_show_all(dialog);
}
static void on_admin_login_submit(GtkWidget *dialog, gpointer unused) {
    GtkEntry *user = GTK_ENTRY(g_object_get_data(G_OBJECT(dialog), "admin_user"));
    GtkEntry *pass = GTK_ENTRY(g_object_get_data(G_OBJECT(dialog), "admin_pass"));
    const char *u = gtk_entry_get_text(user); const char *p = gtk_entry_get_text(pass);
    if (strcmp(u,"admin") == 0 && strcmp(p,"90253") == 0) { gtk_widget_destroy(dialog); show_admin_details(NULL, NULL); }
    else { MSGBOX(dialog, GTK_MESSAGE_ERROR, "Invalid credentials!"); }
}
static void show_admin_details(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dialog), "Admin Details Panel");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 650, 390);
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(dialog), grid);
    GtkWidget *lbl = gtk_label_new("Account Number:");
    admin_search_entry = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(admin_search_entry), GTK_INPUT_PURPOSE_NUMBER);
    GtkWidget *btn_search = gtk_button_new_with_label("Search");
    gtk_grid_attach(GTK_GRID(grid), lbl, 0,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), admin_search_entry, 1,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), btn_search, 2,0,1,1);
    admin_details_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_grid_attach(GTK_GRID(grid), admin_details_box, 0, 1, 3, 20);
    g_signal_connect(btn_search, "clicked", G_CALLBACK(on_admin_search), dialog);
    on_admin_search(NULL, dialog);
    gtk_widget_show_all(dialog);
    admin_dialog = dialog;
}
static void on_admin_search(GtkWidget *widget, gpointer dialog) {
    GList *children = gtk_container_get_children(GTK_CONTAINER(admin_details_box));
    for (GList *ch = children; ch != NULL; ch = ch->next) gtk_widget_destroy(GTK_WIDGET(ch->data));
    g_list_free(children);
    const char *accstr = gtk_entry_get_text(GTK_ENTRY(admin_search_entry));
    Account *arr; int count;
    if (!load_accounts(&arr, &count) || count == 0) {
        GtkWidget *lbl = gtk_label_new("No accounts found.");
        gtk_box_pack_start(GTK_BOX(admin_details_box), lbl, FALSE, FALSE, 0);
        gtk_widget_show_all(admin_details_box); if (arr) free(arr); return;
    }
    qsort(arr, count, sizeof(Account), cmp_acc);
    int search = -1, found = 0;
    if (accstr && strlen(accstr) > 0 && sscanf(accstr,"%d",&search)==1) {
        Account *acc = find_account_bsearch(search, arr, count);
        if (acc) {
            GtkWidget *frame = gtk_frame_new(NULL);
            char info[256];
            snprintf(info, sizeof(info), "Acc: %d, Name: %s, PIN: %d, Bal: %.2lf", acc->account_number, acc->name, acc->pin, acc->balance);
            GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
            GtkWidget *label = gtk_label_new(info);
            GtkWidget *btn = gtk_button_new_with_label("See History");
            AdminHistContext *ctx = g_malloc(sizeof(AdminHistContext));
            ctx->accnum = acc->account_number; ctx->parent = dialog;
            g_signal_connect(btn, "clicked", G_CALLBACK(on_admin_history), ctx);
            gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
            gtk_box_pack_end(GTK_BOX(hbox), btn, FALSE, FALSE, 0);
            gtk_container_add(GTK_CONTAINER(frame), hbox); gtk_box_pack_start(GTK_BOX(admin_details_box), frame, FALSE, FALSE, 4);
            found = 1;
        }
    } else {
        for (int i=0; i<count; ++i) {
            GtkWidget *frame = gtk_frame_new(NULL);
            char info[256];
            snprintf(info, sizeof(info), "Acc: %d, Name: %s, PIN: %d, Bal: %.2lf", arr[i].account_number, arr[i].name, arr[i].pin, arr[i].balance);
            GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
            GtkWidget *label = gtk_label_new(info);
            GtkWidget *btn = gtk_button_new_with_label("See History");
            AdminHistContext *ctx = g_malloc(sizeof(AdminHistContext));
            ctx->accnum = arr[i].account_number; ctx->parent = dialog;
            g_signal_connect(btn, "clicked", G_CALLBACK(on_admin_history), ctx);
            gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
            gtk_box_pack_end(GTK_BOX(hbox), btn, FALSE, FALSE, 0);
            gtk_container_add(GTK_CONTAINER(frame), hbox); gtk_box_pack_start(GTK_BOX(admin_details_box), frame, FALSE, FALSE, 4);
            found = 1;
        }
    }
    if (!found) {
        GtkWidget *lbl = gtk_label_new("No matching users.");
        gtk_box_pack_start(GTK_BOX(admin_details_box), lbl, FALSE, FALSE, 0);
    }
    gtk_widget_show_all(admin_details_box);
    free(arr);
}
static void on_admin_history(GtkWidget *widget, gpointer user_data) {
    AdminHistContext *ctx = (AdminHistContext*)user_data;
    char result[4096] = "";
    get_transaction_history_admin(ctx->accnum, result, sizeof(result));
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget *textview = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(scrolled), textview);
    GtkTextBuffer *buff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE);
    gtk_text_buffer_set_text(buff, result, -1);
    GtkWidget *out_dialog = gtk_dialog_new_with_buttons("User History", GTK_WINDOW(ctx->parent), GTK_DIALOG_DESTROY_WITH_PARENT, "OK", GTK_RESPONSE_OK, NULL);
    gtk_window_set_default_size(GTK_WINDOW(out_dialog), 480, 340);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(out_dialog))), scrolled, TRUE, TRUE, 0);
    gtk_widget_show_all(out_dialog);
    gtk_dialog_run(GTK_DIALOG(out_dialog));
    gtk_widget_destroy(out_dialog);
    g_free(ctx);
}
static void show_history(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dialog), "See Transaction History");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 320, 105);
    GtkWidget *grid = gtk_grid_new(); gtk_container_add(GTK_CONTAINER(dialog), grid);
    GtkWidget *l1 = gtk_label_new("Account number:"); hist_entry_accnum = gtk_entry_new(); gtk_entry_set_input_purpose(GTK_ENTRY(hist_entry_accnum), GTK_INPUT_PURPOSE_NUMBER);
    GtkWidget *l2 = gtk_label_new("PIN:"); hist_entry_pin = gtk_entry_new(); gtk_entry_set_visibility(GTK_ENTRY(hist_entry_pin), FALSE);
    gtk_grid_attach(GTK_GRID(grid), l1, 0,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), hist_entry_accnum, 1,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), l2, 0,1,1,1);
    gtk_grid_attach(GTK_GRID(grid), hist_entry_pin, 1,1,1,1);
    GtkWidget *btn = gtk_button_new_with_label("Show");
    gtk_grid_attach(GTK_GRID(grid), btn, 1,2,1,1);
    g_signal_connect(btn, "clicked", G_CALLBACK(on_history_submit), dialog);
    gtk_widget_show_all(dialog);
}
static void on_history_submit(GtkWidget *widget, gpointer data) {
    int accnum, pin;
    char *accstr = (char*)gtk_entry_get_text(GTK_ENTRY(hist_entry_accnum));
    char *pinstr = (char*)gtk_entry_get_text(GTK_ENTRY(hist_entry_pin));
    if (sscanf(accstr, "%d", &accnum) != 1 || sscanf(pinstr, "%d", &pin) != 1) { MSGBOX(data, GTK_MESSAGE_ERROR, "Enter valid account # and PIN!"); return; }
    char result[4096] = "";
    int pin_ok = 0;
    get_transaction_history(accnum, result, sizeof(result), pin, &pin_ok);
    if (!pin_ok) { MSGBOX(data, GTK_MESSAGE_ERROR, "Invalid credentials."); }
    else {
        GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
        GtkWidget *textview = gtk_text_view_new();
        gtk_container_add(GTK_CONTAINER(scrolled), textview);
        GtkTextBuffer *buff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
        gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE);
        gtk_text_buffer_set_text(buff, result, -1);
        GtkWidget *out_dialog = gtk_dialog_new_with_buttons("Your History", GTK_WINDOW(data), GTK_DIALOG_DESTROY_WITH_PARENT, "OK", GTK_RESPONSE_OK, NULL);
        gtk_window_set_default_size(GTK_WINDOW(out_dialog), 480, 340);
        gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(out_dialog))), scrolled, TRUE, TRUE, 0);
        gtk_widget_show_all(out_dialog);
        gtk_dialog_run(GTK_DIALOG(out_dialog));
        gtk_widget_destroy(out_dialog);
        gtk_widget_destroy(GTK_WIDGET(data));
    }
}
