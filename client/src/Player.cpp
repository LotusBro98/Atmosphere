//
// Created by amburkoff on 11.05.19.
//

#include <Client.h>
#include "Player.h"

void destroy_cb(GtkWidget *widget, gpointer data) {
    Player* player = (Player*) data;

    player->destroy();
}

void player_widget_on_realize(GtkWidget *widget, gpointer data) {
    Player* player = (Player*) data;
    libvlc_media_player_set_xwindow(player->media_player, GDK_WINDOW_XID(gtk_widget_get_window(widget)));
}

void on_open(GtkWidget *widget, gpointer data) {
    Player* player = (Player*) data;
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new("Choose Media", GTK_WINDOW(widget), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
    if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *uri;
        uri = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(dialog));
        player->open_media(uri);
        g_free(uri);
    }
    gtk_widget_destroy(dialog);
}

void on_playpause(GtkWidget *widget, gpointer data) {
    Player* player = (Player*) data;
    Room* room = Client::getClient()->getServer()->getCurrentRoom();
    if(libvlc_media_player_is_playing(player->media_player) == 1) {
        room->pause();
    }
    else {
        room->play();
    }
}

void on_stop(GtkWidget *widget, gpointer data) {
    Player* player = (Player*) data;

    player->pause_player();
    libvlc_media_player_stop(player->media_player);
}

void on_connect(GtkWidget *widget, gpointer data) {
    Player* player = (Player*) data;

    fprintf(stderr, "Clicked connect\n");

    Room* room = Client::getClient()->getServer()->getRoom(1);
    room->requestUpdateSource();
}



void Player::notifyUpdateSource() {
    lock();
    sourceChanged = true;
    unlock();
}

void Player::notifyUpdatePlayState() {
    lock();
    playStateChanged = true;
    unlock();
}



void Player::updateSource()
{
    char* uri = Client::getClient()->getServer()->getCurrentRoom()->getSource();
    open_media(uri);
}

void Player::updatePlayState() {
    bool playing = Client::getClient()->getServer()->getCurrentRoom()->playing;
    if (playing)
        play();
    else
        pause_player();
}

void Player::open_media(const char* uri) {
    fprintf(stderr, "Opening '%s'\n", uri);

    libvlc_media_t *media;
    media = libvlc_media_new_location(vlc_inst, uri);
    libvlc_media_player_set_media(media_player, media);
    libvlc_media_release(media);
}

void Player::play(void) {
    libvlc_media_player_play(media_player);
    gtk_button_set_label(GTK_BUTTON(playpause_button), "gtk-media-pause");
}

void Player::pause_player(void) {
    libvlc_media_player_pause(media_player);
    gtk_button_set_label(GTK_BUTTON(playpause_button), "gtk-media-play");
}

Player::Player() {
    pthread_mutex_init(&mutex, NULL);
}

void Player::init(int argc, char* argv[]) {

    gtk_init(&argc, &argv);

    // setup window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(destroy_cb), this);
    gtk_container_set_border_width(GTK_CONTAINER (window), 0);
    gtk_window_set_title(GTK_WINDOW(window), "GTK+ libVLC Demo");

    //setup vbox
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    //setup menu
    menubar = gtk_menu_bar_new();
    filemenu = gtk_menu_new();
    fileitem = gtk_menu_item_new_with_label("File");
    filemenu_openitem = gtk_menu_item_new_with_label("Open");
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), filemenu_openitem);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileitem), filemenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileitem);
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
    g_signal_connect(filemenu_openitem, "activate", G_CALLBACK(on_open), this);

    //setup player widget
    player_widget = gtk_drawing_area_new();
    gtk_box_pack_start(GTK_BOX(vbox), player_widget, TRUE, TRUE, 0);

    //setup controls
    //playpause_button = gtk_button_new_from_stock(GTK_STOCK_MEDIA_PLAY);
    playpause_button = gtk_button_new_with_label("gtk-media-play");
    gtk_button_set_use_stock(GTK_BUTTON(playpause_button), TRUE);
    stop_button = gtk_button_new_from_stock(GTK_STOCK_MEDIA_STOP);
    connect_button = gtk_button_new_from_stock(GTK_STOCK_CONNECT);
    g_signal_connect(playpause_button, "clicked", G_CALLBACK(on_playpause), this);
    g_signal_connect(stop_button, "clicked", G_CALLBACK(on_stop), this);
    g_signal_connect(connect_button, "clicked", G_CALLBACK(on_connect), this);
    hbuttonbox = gtk_hbutton_box_new();
    gtk_container_set_border_width(GTK_CONTAINER(hbuttonbox), BORDER_WIDTH);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(hbuttonbox), GTK_BUTTONBOX_START);
    gtk_box_pack_start(GTK_BOX(hbuttonbox), playpause_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbuttonbox), stop_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbuttonbox), connect_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbuttonbox, FALSE, FALSE, 0);

    //setup vlc
    vlc_inst = libvlc_new(argc, argv);
    media_player = libvlc_media_player_new(vlc_inst);

    g_signal_connect(G_OBJECT(player_widget), "realize", G_CALLBACK(player_widget_on_realize), this);

    gtk_widget_show_all(window);

    errno = 0;
}

void Player::start() {
    alive = True;
    while (alive) {
        if (!gtk_main_iteration())
            break;

        lock();
        if (sourceChanged) {
            updateSource();
            sourceChanged = false;
        }

        if (playStateChanged) {
            updatePlayState();
            playStateChanged = false;
        }
        //usleep(10000);
        unlock();
    }
    gtk_main_quit();
    gtk_main_quit();
    gtk_main_iteration();
    gtk_main_quit();
}

void Player::destroy() {
    //gtk_main_quit();
    alive = False;
    libvlc_media_player_release(media_player);
    libvlc_release(vlc_inst);
}

void Player::lock() {
    pthread_mutex_lock(&mutex);
}

void Player::unlock() {
    pthread_mutex_unlock(&mutex);
}

