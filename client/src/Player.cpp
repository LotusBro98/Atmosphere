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

    //if(libvlc_media_player_is_playing(player->media_player) == 1) {
    if(room->playing) {
        room->pause();
        room->seek(progress);
    }
    else {
        room->play();
        room->seek(progress);
    }
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
    sourceChanged = false;
}

void Player::updatePlayState() {
    Room* room = Client::getClient()->getServer()->getCurrentRoom();

    libvlc_state_t state = libvlc_media_player_get_state(media_player);
    printf("state = %d\n", state);
    if (state == 1 || state == 2)
        return;
    if (state == 5) {
        play();
        room->pause();
    }
    if (state == 7)
    {
        fprintf(stderr, "Player Error!");
        playStateChanged = false;
        return;
    }
    if (state == 6)
    {
        libvlc_media_player_stop(media_player);
        room->seek(1000);
        printf("Konets :)\n");
    }

    bool playing = room->playing;
    float progress = room->progress;

    if (playing)
        play();
    else
        pause_player();

    seek(progress);
    //playStateChanged = false;
}

bool Player::checkWrongState()
{
    Room* room = Client::getClient()->getServer()->getCurrentRoom();
    libvlc_state_t state = libvlc_media_player_get_state(media_player);

    if (state == 1 || state == 2)
        return false;

    if (room == NULL)
        return false;

    if (room->playing && state != libvlc_Playing)
        return true;

    if (!room->playing && state != libvlc_Paused)
        return true;

    return false;
}

void Player::open_media(const char* uri) {
    fprintf(stderr, "Opening '%s'\n", uri);

    libvlc_media_t *media;
    media = libvlc_media_new_location(vlc_inst, uri);
    libvlc_media_player_set_media(media_player, media);
    //libvlc_media_player_stop(media_player);
    play();
    libvlc_media_release(media);

    //pause_player();
}

void Player::play(void) {
    libvlc_state_t state = libvlc_media_player_get_state(media_player);
    libvlc_media_player_play(media_player);
    fprintf(stderr, "Playing!!!! >_< %d\n", state);
    //gtk_button_set_label(GTK_BUTTON(playpause_button), "gtk-media-pause");
    state = libvlc_media_player_get_state(media_player);
    if (state == libvlc_Playing)
        playStateChanged = false;
    fprintf(stderr, "state %d\n", state);
}

void Player::pause_player(void) {
    libvlc_state_t state = libvlc_media_player_get_state(media_player);
    fprintf(stderr, "Pausing!!!! >_< %d\n", state);
    libvlc_media_player_set_pause(media_player, 1);
    //gtk_button_set_label(GTK_BUTTON(playpause_button), "gtk-media-play");
    state = libvlc_media_player_get_state(media_player);
    if (state == libvlc_Paused || state == libvlc_Ended)
        playStateChanged = false;
    fprintf(stderr, "state %d\n", state);
}

void Player::seek(float progress) {
    printf("%f\n", progress);

    if (progress == -1) {
        return;
    }

    libvlc_media_player_set_time(media_player, progress);
    printf("%ld\n", libvlc_media_player_get_time(media_player));
}




Player::Player() {
    pthread_mutex_init(&mutex, NULL);
}



void Player::start() {
    alive = True;
    int check_cnt = 0;
    int check_period = 10000;

    while (alive) {
        if (!gtk_main_iteration_do(false))
            break;

        usleep(1000);
        lock();
        if (sourceChanged) {
            updateSource();
        }

        check_cnt++;
        check_cnt %= check_period;
        if ((check_cnt == 0) && !playStateChanged && checkWrongState()) {
            playStateChanged = true;
        }


        if (playStateChanged) {
            updatePlayState();
        }
        unlock();

        //libvlc_state_t state = libvlc_media_player_get_state(media_player);
        //printf("=== %d ===\n", state);
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



