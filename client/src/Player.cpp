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

void on_playpause(GtkWidget *widget, gpointer data) {
    Player* player = (Player*) data;
    Room* room = Client::getClient()->getServer()->getCurrentRoom();
    float progress = libvlc_media_player_get_time(player->media_player);

    if(libvlc_media_player_is_playing(player->media_player) == 1) {
        room->pause();
        room->seek(progress);
    }
    else {
        room->play();
        room->seek(progress);
    }
}

void on_stop(GtkWidget *widget, gpointer data) {
    Player* player = (Player*) data;

    //player->pause_player();
    //libvlc_media_player_stop(player->media_player);
    player->fillRoomsList();
}

void on_connect(GtkWidget *widget, gpointer data) {
    Player* player = (Player*) data;

    fprintf(stderr, "Clicked connect\n");

    Room* room = Client::getClient()->getServer()->getRoom(1);
    room->requestUpdateSource();
}

void Player::notifyUpdateSource() {
    //lock();
    sourceChanged = true;
    //unlock();
    //fprintf(stderr, "[!!!] Notified Update Source\n");
}

void Player::notifyUpdatePlayState() {
    //lock();
    playStateChanged = true;
    //unlock();
    //fprintf(stderr, "[!!!] Notified Update Play State\n");
}

void Player::updateSource()
{
    char* uri = Client::getClient()->getServer()->getCurrentRoom()->getSource();
    open_media(uri);
}

void Player::updatePlayState() {
    Room* room = Client::getClient()->getServer()->getCurrentRoom();

    bool playing = room->playing;
    float progress = room->progress;

    seek(progress);

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

void Player::seek(float progress) {
    libvlc_media_player_set_time(media_player, progress);
}




Player::Player() {
    pthread_mutex_init(&mutex, NULL);
}



void Player::start() {
    alive = True;
    while (alive) {
        if (!gtk_main_iteration_do(false))
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
        unlock();
        usleep(1000);
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

    exit(0);
}

void Player::lock() {
    pthread_mutex_lock(&mutex);
}

void Player::unlock() {
    pthread_mutex_unlock(&mutex);
}



