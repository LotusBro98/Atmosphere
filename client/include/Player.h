//
// Created by amburkoff on 11.05.19.
//

#ifndef ATMOSPHERE_PLAYER_H
#define ATMOSPHERE_PLAYER_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <vlc/vlc.h>

#include "../../include/Message.h"

#define BORDER_WIDTH 6

class Player {
public:
    Player();

    void init(int argc, char* argv[]);
    void start();
    void destroy();

    friend void destroy_cb(GtkWidget *widget, gpointer data);
    friend void player_widget_on_realize(GtkWidget *widget, gpointer data);
    friend void on_open(GtkWidget *widget, gpointer data);
    friend void on_playpause(GtkWidget *widget, gpointer data);
    friend void on_stop(GtkWidget *widget, gpointer data);
    friend void on_connect(GtkWidget *widget, gpointer data);


    void play(void);
    void pause_player(void);

    void notifyUpdateSource();
    void notifyUpdatePlayState();

private:
    char source[MAX_MSG_SIZE];

    libvlc_media_player_t *media_player;
    libvlc_instance_t *vlc_inst;

    GtkWidget *window,
            *playpause_button,
            *vbox,
            *menubar,
            *filemenu,
            *fileitem,
            *filemenu_openitem,
            *player_widget,
            *hbuttonbox,
            *stop_button,
            *connect_button;

    bool alive;

    void lock();
    void unlock();

    bool sourceChanged;
    void updateSource();

    bool playStateChanged;
    void updatePlayState();

    void open_media(const char* uri);

    pthread_mutex_t mutex;
};


#endif //ATMOSPHERE_PLAYER_H
