#include <gtk/gtk.h>
#include "bank.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

GtkWidget *entry_name, *vb_entry_accnum, *vb_entry_pin;
GtkWidget *dep_entry_accnum, *dep_entry_pin, *dep_entry_amount;
GtkWidget *wit_entry_accnum, *wit_entry_pin, *wit_entry_amount;
GtkWidget *chp_entry_accnum, *chp_entry_pin, *chp_new_pin;
GtkWidget *del_entry_accnum, *del_entry_pin;
GtkWidget *admin_username, *admin_password;
GtkWidget *hist_entry_accnum, *hist_entry_pin;

#define MSGBOX(dialog_parent, type, text) do { \
    GtkWidget *dl = gtk_message_dialog_new(GTK_WINDOW(dialog_parent), GTK_DIALOG_DESTROY_WITH_PARENT, type, GTK_BUTTONS_OK, "%s", text); \
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
static void show_admin_details(GtkWidget*, gpointer);
static void on_admin_details_submit(GtkWidget*, gpointer);
static void show_history(GtkWidget *, gpointer);
static void on_history_submit(GtkWidget*, gpointer);

int main(int argc, char *argv[]) {
    srand((unsigned) time(NULL));
    GtkWidget *window, *grid;
    GtkWidget *btn_create, *btn_viewbal, *btn_deposit, *btn_withdraw, *btn_changepin, *btn_delete, *btn_detials, *btn_exit, *btn_history;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Banking System - GUI");
    gtk_window_set_default_size(GTK_WINDOW(window), 380, 480);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 12);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 30);
    gtk_container_add(GTK_CONTAINER(window), grid);

    btn_create    = gtk_button_new_with_label("Create Account");
    btn_viewbal   = gtk_button_new_with_label("View Balance");
    btn_deposit   = gtk_button_new_with_label("Deposit");
    btn_withdraw  = gtk_button_new_with_label("Withdraw");
    btn_changepin = gtk_button_new_with_label("Change PIN");
    btn_delete    = gtk_button_new_with_label("Delete Account");
    btn_detials   = gtk_button_new_with_label("Detials (Admin)");
    btn_history   = gtk_button_new_with_label("See History");
    btn_exit      = gtk_button_new_with_label("Exit");

    gtk_grid_attach(GTK_GRID(grid), btn_create,    0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_viewbal,   0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_deposit,   0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_withdraw,  0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_changepin, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_delete,    0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_detials,   0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_history,   0, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_exit,      0, 8, 1, 1);

    g_signal_connect(btn_create,    "clicked", G_CALLBACK(show_create_account), window);
    g_signal_connect(btn_viewbal,   "clicked", G_CALLBACK(show_view_balance), window);
    g_signal_connect(btn_deposit,   "clicked", G_CALLBACK(show_deposit), window);
    g_signal_connect(btn_withdraw,  "clicked", G_CALLBACK(show_withdraw), window);
    g_signal_connect(btn_changepin, "clicked", G_CALLBACK(show_change_pin), window);
    g_signal_connect(btn_delete,    "clicked", G_CALLBACK(show_delete_account), window);
    g_signal_connect(btn_detials,   "clicked", G_CALLBACK(show_admin_details), window);
    g_signal_connect(btn_history,   "clicked", G_CALLBACK(show_history), window);
    g_signal_connect(btn_exit,      "clicked", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}

// =============== CREATE ACCOUNT ===============
static void show_create_account(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *grid, *label, *btn_submit;
    dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dialog), "Create New Account");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 120);
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(dialog), grid);

    label = gtk_label_new("Enter your name:");
    entry_name = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), label,      0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_name, 1, 0, 2, 1);

    btn_submit = gtk_button_new_with_label("Create");
    gtk_grid_attach(GTK_GRID(grid), btn_submit,  1, 1, 1, 1);

    g_signal_connect(btn_submit, "clicked", G_CALLBACK(on_create_account_submit), dialog);
    gtk_widget_show_all(dialog);
}

static void on_create_account_submit(GtkWidget *widget, gpointer data) {
    const char *uname = gtk_entry_get_text(GTK_ENTRY(entry_name));
    if (strlen(uname) == 0) {
        MSGBOX(data, GTK_MESSAGE_ERROR, "Please enter a name.");
        return;
    }
    Account acc;
    strncpy(acc.name, uname, sizeof(acc.name) - 1);
    acc.name[sizeof(acc.name) - 1] = 0;
    acc.account_number = generate_account_number();
    acc.pin = generate_pin();
    acc.balance = 0.0;
    save_account(&acc);

    char msg[256];
    snprintf(msg, sizeof(msg), "Account created!\n\nAccount No: %d\nPIN: %d", acc.account_number, acc.pin);
    MSGBOX(data, GTK_MESSAGE_INFO, msg);
    gtk_widget_destroy(GTK_WIDGET(data));
}

// =============== VIEW BALANCE ================
static void show_view_balance(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *grid, *label1, *label2, *btn_submit;
    dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dialog), "View Balance");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 320, 130);
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(dialog), grid);

    label1 = gtk_label_new("Account number:");
    vb_entry_accnum = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(vb_entry_accnum), GTK_INPUT_PURPOSE_NUMBER);

    label2 = gtk_label_new("PIN:");
    vb_entry_pin = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(vb_entry_pin), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(vb_entry_pin), '*');

    gtk_grid_attach(GTK_GRID(grid), label1,           0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), vb_entry_accnum,  1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label2,           0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), vb_entry_pin,     1, 1, 1, 1);

    btn_submit = gtk_button_new_with_label("View");
    gtk_grid_attach(GTK_GRID(grid), btn_submit,       1, 2, 1, 1);

    g_signal_connect(btn_submit, "clicked", G_CALLBACK(on_view_balance_submit), dialog);
    gtk_widget_show_all(dialog);
}

static void on_view_balance_submit(GtkWidget *widget, gpointer data) {
    const char *accnum_txt = gtk_entry_get_text(GTK_ENTRY(vb_entry_accnum));
    const char *pin_txt    = gtk_entry_get_text(GTK_ENTRY(vb_entry_pin));
    int accnum, pin;
    if (sscanf(accnum_txt, "%d", &accnum) != 1 || sscanf(pin_txt, "%d", &pin) != 1) {
        MSGBOX(data, GTK_MESSAGE_ERROR, "Enter valid account number and PIN!");
        return;
    }
    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) { MSGBOX(data, GTK_MESSAGE_ERROR, "No accounts found."); return; }
    Account acc;
    int found = 0;
    while (fread(&acc, sizeof(Account), 1, fp)) {
        if (acc.account_number == accnum && acc.pin == pin) {
            found = 1; break;
        }
    }
    fclose(fp);
    if (found) {
        char msg[256];
        snprintf(msg, sizeof(msg), "Welcome, %s!\n\nAccount: %d\nBalance: Rs. %.2lf", acc.name, acc.account_number, acc.balance);
        MSGBOX(data, GTK_MESSAGE_INFO, msg);
        gtk_widget_destroy(GTK_WIDGET(data));
    } else {
        MSGBOX(data, GTK_MESSAGE_ERROR, "Invalid credentials.");
    }
}

// =============== DEPOSIT ======================
static void show_deposit(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *grid, *l1, *l2, *l3, *btn;
    dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dialog), "Deposit");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 320, 160);
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(dialog), grid);

    l1 = gtk_label_new("Account number:");
    dep_entry_accnum = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(dep_entry_accnum), GTK_INPUT_PURPOSE_NUMBER);

    l2 = gtk_label_new("PIN:");
    dep_entry_pin = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(dep_entry_pin), FALSE);

    l3 = gtk_label_new("Amount to deposit:");
    dep_entry_amount = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(dep_entry_amount), GTK_INPUT_PURPOSE_NUMBER);

    gtk_grid_attach(GTK_GRID(grid), l1, 0,0,1,1); gtk_grid_attach(GTK_GRID(grid), dep_entry_accnum, 1,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), l2, 0,1,1,1); gtk_grid_attach(GTK_GRID(grid), dep_entry_pin,    1,1,1,1);
    gtk_grid_attach(GTK_GRID(grid), l3, 0,2,1,1); gtk_grid_attach(GTK_GRID(grid), dep_entry_amount, 1,2,1,1);

    btn = gtk_button_new_with_label("Deposit");
    gtk_grid_attach(GTK_GRID(grid), btn, 1,3,1,1);
    g_signal_connect(btn, "clicked", G_CALLBACK(on_deposit_submit), dialog);
    gtk_widget_show_all(dialog);
}

static void on_deposit_submit(GtkWidget *widget, gpointer data) {
    int accnum, pin; double amount;
    if (sscanf(gtk_entry_get_text(GTK_ENTRY(dep_entry_accnum)), "%d", &accnum) != 1 ||
        sscanf(gtk_entry_get_text(GTK_ENTRY(dep_entry_pin)), "%d", &pin) != 1 ||
        sscanf(gtk_entry_get_text(GTK_ENTRY(dep_entry_amount)), "%lf", &amount) != 1 || amount <= 0) {
        MSGBOX(data, GTK_MESSAGE_ERROR, "Enter valid account, PIN, and positive amount!");
        return;
    }
    FILE *fp = fopen(FILENAME, "rb+");
    if (!fp) { MSGBOX(data, GTK_MESSAGE_ERROR, "No accounts found."); return; }
    Account acc;
    int found = 0;
    long pos;
    while ((pos = ftell(fp)), fread(&acc, sizeof(Account), 1, fp) == 1) {
        if (acc.account_number == accnum && acc.pin == pin) {
            acc.balance += amount;
            fseek(fp, pos, SEEK_SET);
            fwrite(&acc, sizeof(Account), 1, fp);
            register_transaction(accnum, "DEPOSIT", amount, acc.balance);
            found = 1; break;
        }
    }
    fclose(fp);
    if (found) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Deposit successful.\nNew balance: Rs. %.2lf", acc.balance);
        MSGBOX(data, GTK_MESSAGE_INFO, msg);
        gtk_widget_destroy(GTK_WIDGET(data));
    } else {
        MSGBOX(data, GTK_MESSAGE_ERROR, "Account not found or PIN incorrect.");
    }
}

// =============== WITHDRAW ======================
static void show_withdraw(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *grid, *l1, *l2, *l3, *btn;
    dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dialog), "Withdraw");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 320, 160);
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(dialog), grid);

    l1 = gtk_label_new("Account number:");
    wit_entry_accnum = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(wit_entry_accnum), GTK_INPUT_PURPOSE_NUMBER);

    l2 = gtk_label_new("PIN:");
    wit_entry_pin = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(wit_entry_pin), FALSE);

    l3 = gtk_label_new("Amount to withdraw:");
    wit_entry_amount = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(wit_entry_amount), GTK_INPUT_PURPOSE_NUMBER);

    gtk_grid_attach(GTK_GRID(grid), l1, 0,0,1,1); gtk_grid_attach(GTK_GRID(grid), wit_entry_accnum, 1,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), l2, 0,1,1,1); gtk_grid_attach(GTK_GRID(grid), wit_entry_pin,    1,1,1,1);
    gtk_grid_attach(GTK_GRID(grid), l3, 0,2,1,1); gtk_grid_attach(GTK_GRID(grid), wit_entry_amount, 1,2,1,1);

    btn = gtk_button_new_with_label("Withdraw");
    gtk_grid_attach(GTK_GRID(grid), btn, 1,3,1,1);
    g_signal_connect(btn, "clicked", G_CALLBACK(on_withdraw_submit), dialog);
    gtk_widget_show_all(dialog);
}

static void on_withdraw_submit(GtkWidget *widget, gpointer data) {
    int accnum, pin; double amount;
    if (sscanf(gtk_entry_get_text(GTK_ENTRY(wit_entry_accnum)), "%d", &accnum) != 1 ||
        sscanf(gtk_entry_get_text(GTK_ENTRY(wit_entry_pin)), "%d", &pin) != 1 ||
        sscanf(gtk_entry_get_text(GTK_ENTRY(wit_entry_amount)), "%lf", &amount) != 1 || amount <= 0) {
        MSGBOX(data, GTK_MESSAGE_ERROR, "Enter valid account, PIN, and positive amount!");
        return;
    }
    FILE *fp = fopen(FILENAME, "rb+");
    if (!fp) { MSGBOX(data, GTK_MESSAGE_ERROR, "No accounts found."); return; }
    Account acc;
    int found = 0, err = 0;
    long pos;
    while ((pos = ftell(fp)), fread(&acc, sizeof(Account), 1, fp) == 1) {
        if (acc.account_number == accnum && acc.pin == pin) {
            if (amount > acc.balance) { err = 1; break; }
            acc.balance -= amount;
            fseek(fp, pos, SEEK_SET);
            fwrite(&acc, sizeof(Account), 1, fp);
            register_transaction(accnum, "WITHDRAW", amount, acc.balance);
            found = 1; break;
        }
    }
    fclose(fp);
    if (found) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Withdraw successful.\nNew balance: Rs. %.2lf", acc.balance);
        MSGBOX(data, GTK_MESSAGE_INFO, msg);
        gtk_widget_destroy(GTK_WIDGET(data));
    } else if(err) {
        MSGBOX(data, GTK_MESSAGE_ERROR, "Insufficient balance.");
    } else {
        MSGBOX(data, GTK_MESSAGE_ERROR, "Account not found or PIN incorrect.");
    }
}

// =============== CHANGE PIN =====================
static void show_change_pin(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *grid, *l1, *l2, *l3, *btn;
    dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dialog), "Change PIN");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 340, 150);
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(dialog), grid);

    l1 = gtk_label_new("Account number:");
    chp_entry_accnum = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(chp_entry_accnum), GTK_INPUT_PURPOSE_NUMBER);

    l2 = gtk_label_new("Current PIN:");
    chp_entry_pin = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(chp_entry_pin), FALSE);

    l3 = gtk_label_new("New 4-digit PIN:");
    chp_new_pin = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(chp_new_pin), 4);
    gtk_entry_set_input_purpose(GTK_ENTRY(chp_new_pin), GTK_INPUT_PURPOSE_NUMBER);
    gtk_entry_set_visibility(GTK_ENTRY(chp_new_pin), FALSE);

    gtk_grid_attach(GTK_GRID(grid), l1, 0,0,1,1); gtk_grid_attach(GTK_GRID(grid), chp_entry_accnum, 1,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), l2, 0,1,1,1); gtk_grid_attach(GTK_GRID(grid), chp_entry_pin,    1,1,1,1);
    gtk_grid_attach(GTK_GRID(grid), l3, 0,2,1,1); gtk_grid_attach(GTK_GRID(grid), chp_new_pin,      1,2,1,1);

    btn = gtk_button_new_with_label("Change PIN");
    gtk_grid_attach(GTK_GRID(grid), btn, 1,3,1,1);
    g_signal_connect(btn, "clicked", G_CALLBACK(on_change_pin_submit), dialog);
    gtk_widget_show_all(dialog);
}

static void on_change_pin_submit(GtkWidget *widget, gpointer data) {
    int accnum, pin, newpin;
    if (sscanf(gtk_entry_get_text(GTK_ENTRY(chp_entry_accnum)), "%d", &accnum) != 1 ||
        sscanf(gtk_entry_get_text(GTK_ENTRY(chp_entry_pin)), "%d", &pin) != 1 ||
        sscanf(gtk_entry_get_text(GTK_ENTRY(chp_new_pin)), "%d", &newpin) != 1 ||
        newpin < 1000 || newpin > 9999) {
        MSGBOX(data, GTK_MESSAGE_ERROR, "Please enter valid details (PIN must be 4 digits).");
        return;
    }
    FILE *fp = fopen(FILENAME, "rb+");
    if (!fp) { MSGBOX(data, GTK_MESSAGE_ERROR, "No accounts found."); return; }
    Account acc;
    int found = 0;
    long pos;
    while ((pos = ftell(fp)), fread(&acc, sizeof(Account), 1, fp) == 1) {
        if (acc.account_number == accnum && acc.pin == pin) {
            acc.pin = newpin;
            fseek(fp, pos, SEEK_SET);
            fwrite(&acc, sizeof(Account), 1, fp);
            found = 1; break;
        }
    }
    fclose(fp);
    if (found) {
        MSGBOX(data, GTK_MESSAGE_INFO, "PIN changed successfully.");
        gtk_widget_destroy(GTK_WIDGET(data));
    } else {
        MSGBOX(data, GTK_MESSAGE_ERROR, "Account not found or current PIN incorrect.");
    }
}

// ============== DELETE ACCOUNT ================
static void show_delete_account(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *grid, *l1, *l2, *btn;
    dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dialog), "Delete Account");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 320, 120);
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(dialog), grid);

    l1 = gtk_label_new("Account number:");
    del_entry_accnum = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(del_entry_accnum), GTK_INPUT_PURPOSE_NUMBER);

    l2 = gtk_label_new("PIN:");
    del_entry_pin = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(del_entry_pin), FALSE);

    gtk_grid_attach(GTK_GRID(grid), l1, 0,0,1,1); gtk_grid_attach(GTK_GRID(grid), del_entry_accnum, 1,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), l2, 0,1,1,1); gtk_grid_attach(GTK_GRID(grid), del_entry_pin,    1,1,1,1);

    btn = gtk_button_new_with_label("Delete");
    gtk_grid_attach(GTK_GRID(grid), btn, 1,2,1,1);
    g_signal_connect(btn, "clicked", G_CALLBACK(on_delete_account_submit), dialog);
    gtk_widget_show_all(dialog);
}

static void on_delete_account_submit(GtkWidget *widget, gpointer data) {
    int accnum, pin, found = 0;
    if (sscanf(gtk_entry_get_text(GTK_ENTRY(del_entry_accnum)), "%d", &accnum) != 1 ||
        sscanf(gtk_entry_get_text(GTK_ENTRY(del_entry_pin)), "%d", &pin) != 1) {
        MSGBOX(data, GTK_MESSAGE_ERROR, "Enter valid account number and PIN!");
        return;
    }
    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) { MSGBOX(data, GTK_MESSAGE_ERROR, "No accounts found."); return; }
    FILE *tmp = fopen("temp.dat", "wb");
    if (!tmp) { fclose(fp); MSGBOX(data, GTK_MESSAGE_ERROR, "Cannot create temp file."); return; }
    Account acc;
    while (fread(&acc, sizeof(Account), 1, fp)) {
        if (acc.account_number == accnum && acc.pin == pin) {
            found = 1; continue;
        }
        fwrite(&acc, sizeof(Account), 1, tmp);
    }
    fclose(fp); fclose(tmp);
    if (found) {
        remove(FILENAME); rename("temp.dat", FILENAME);
        MSGBOX(data, GTK_MESSAGE_INFO, "Account deleted successfully.");
        gtk_widget_destroy(GTK_WIDGET(data));
    } else {
        remove("temp.dat");
        MSGBOX(data, GTK_MESSAGE_ERROR, "Account not found or PIN incorrect.");
    }
}

// ============= ADMIN DETAILS ===================
static void show_admin_details(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *grid, *l1, *l2, *btn;

    dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dialog), "Admin Login");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 320, 120);
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(dialog), grid);

    l1 = gtk_label_new("Username: "); admin_username = gtk_entry_new();
    l2 = gtk_label_new("Password: "); admin_password = gtk_entry_new(); gtk_entry_set_visibility(GTK_ENTRY(admin_password), FALSE);

    gtk_grid_attach(GTK_GRID(grid), l1, 0,0,1,1); gtk_grid_attach(GTK_GRID(grid), admin_username, 1,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), l2, 0,1,1,1); gtk_grid_attach(GTK_GRID(grid), admin_password, 1,1,1,1);

    btn = gtk_button_new_with_label("Show Accounts");
    gtk_grid_attach(GTK_GRID(grid), btn, 1,2,1,1);
    g_signal_connect(btn, "clicked", G_CALLBACK(on_admin_details_submit), dialog);
    gtk_widget_show_all(dialog);
}

static void on_admin_details_submit(GtkWidget *widget, gpointer data) {
    const char *user = gtk_entry_get_text(GTK_ENTRY(admin_username));
    const char *pass = gtk_entry_get_text(GTK_ENTRY(admin_password));
    if (strcmp(user,"admin") || strcmp(pass,"90253")) {
        MSGBOX(data, GTK_MESSAGE_ERROR, "Access denied. Invalid admin credentials.");
        return;
    }
    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) { MSGBOX(data, GTK_MESSAGE_ERROR, "No accounts found."); return; }
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget *textview = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(scrolled), textview);
    GtkTextBuffer *buff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE);

    char all[4096] = "";
    Account acc;
    strcat(all,"--- Stored Accounts ---\n");
    while (fread(&acc, sizeof(Account), 1, fp)) {
        char line[256];
        snprintf(line, sizeof(line), "Acc: %d, Name: %s, PIN: %d, Bal: %.2lf\n",
                  acc.account_number, acc.name, acc.pin, acc.balance);
        strncat(all, line, sizeof(all)-strlen(all)-1);
    }
    fclose(fp);

    gtk_text_buffer_set_text(buff, all, -1);

    GtkWidget *out_dialog = gtk_dialog_new_with_buttons("All Accounts", GTK_WINDOW(data),
        GTK_DIALOG_DESTROY_WITH_PARENT, "OK", GTK_RESPONSE_OK, NULL);
    gtk_window_set_default_size(GTK_WINDOW(out_dialog), 460, 350);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(out_dialog))), scrolled, TRUE, TRUE, 0);

    gtk_widget_show_all(out_dialog);
    gtk_dialog_run(GTK_DIALOG(out_dialog));
    gtk_widget_destroy(out_dialog);
    gtk_widget_destroy(GTK_WIDGET(data));
}

// ============== HISTORY (SEE HISTORY PER USER) ==============
static void show_history(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *grid, *l1, *l2, *btn;
    dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dialog), "See Transaction History");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 320, 105);
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(dialog), grid);

    l1 = gtk_label_new("Account number:");
    hist_entry_accnum = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(hist_entry_accnum), GTK_INPUT_PURPOSE_NUMBER);
    l2 = gtk_label_new("PIN:");
    hist_entry_pin = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(hist_entry_pin), FALSE);

    gtk_grid_attach(GTK_GRID(grid), l1, 0,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), hist_entry_accnum, 1,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), l2, 0,1,1,1);
    gtk_grid_attach(GTK_GRID(grid), hist_entry_pin, 1,1,1,1);

    btn = gtk_button_new_with_label("Show");
    gtk_grid_attach(GTK_GRID(grid), btn, 1,2,1,1);

    g_signal_connect(btn, "clicked", G_CALLBACK(on_history_submit), dialog);

    gtk_widget_show_all(dialog);
}

static void on_history_submit(GtkWidget *widget, gpointer data) {
    int accnum, pin;
    char *accstr = (char*)gtk_entry_get_text(GTK_ENTRY(hist_entry_accnum));
    char *pinstr = (char*)gtk_entry_get_text(GTK_ENTRY(hist_entry_pin));
    if (sscanf(accstr, "%d", &accnum) != 1 || sscanf(pinstr, "%d", &pin) != 1) {
        MSGBOX(data, GTK_MESSAGE_ERROR, "Enter valid account # and PIN!");
        return;
    }
    char result[4096] = "";
    int pin_ok = 0;
    get_transaction_history(accnum, result, sizeof(result), pin, &pin_ok);

    if (!pin_ok) {
        MSGBOX(data, GTK_MESSAGE_ERROR, "Invalid credentials.");
    } else {
        GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
        GtkWidget *textview = gtk_text_view_new();
        gtk_container_add(GTK_CONTAINER(scrolled), textview);
        GtkTextBuffer *buff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
        gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE);
        gtk_text_buffer_set_text(buff, result, -1);

        GtkWidget *out_dialog = gtk_dialog_new_with_buttons("Your History", GTK_WINDOW(data),
            GTK_DIALOG_DESTROY_WITH_PARENT, "OK", GTK_RESPONSE_OK, NULL);
        gtk_window_set_default_size(GTK_WINDOW(out_dialog), 480, 340);
        gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(out_dialog))), scrolled, TRUE, TRUE, 0);

        gtk_widget_show_all(out_dialog);
        gtk_dialog_run(GTK_DIALOG(out_dialog));
        gtk_widget_destroy(out_dialog);
        gtk_widget_destroy(GTK_WIDGET(data));
    }
}
