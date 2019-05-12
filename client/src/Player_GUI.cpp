//
// Created by amburkoff on 11.05.19.
//

#include "../include/Room.h"
#include "../include/Client.h"
#include "../include/Player.h"

void on_room_pick(GtkWidget *widget, gpointer data)
{
    Room* room = (Room*) data;

    Client::getClient()->getServer()->selectRoom(room);
    //Client::getClient()->getPlayer()->clearLeftList();
    gtk_revealer_set_reveal_child(GTK_REVEALER(Client::getClient()->getPlayer()->left_revealer), false);
}

void on_show_rooms(GtkWidget *widget, gpointer data)
{
    Player* player = (Player*) data;

    if (gtk_revealer_get_child_revealed(GTK_REVEALER(player->left_revealer)))
        gtk_revealer_set_reveal_child(GTK_REVEALER(player->left_revealer), false);
    else
        player->fillRoomsList();
}

void on_show_playlist(GtkWidget *widget, gpointer data)
{
    Player* player = (Player*) data;

    if (gtk_revealer_get_child_revealed(GTK_REVEALER(player->right_revealer)))
        gtk_revealer_set_reveal_child(GTK_REVEALER(player->right_revealer), false);
    else
        player->fillPlayList();
}


void Player::init(int argc, char* argv[]) {

    gtk_init(&argc, &argv);

    // setup window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_container_set_border_width(GTK_CONTAINER (window), 0);
    gtk_window_set_title(GTK_WINDOW(window), "VKino");

    g_signal_connect(window, "destroy", G_CALLBACK(destroy_cb), this);

    auto provider = gtk_css_provider_new();
    auto display = gdk_display_get_default();
    auto screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(provider),"styles.css",NULL);


    main_vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), main_vbox);

    room_hbox = gtk_hbox_new(FALSE, 10);
    gtk_box_pack_start(GTK_BOX(main_vbox), room_hbox, TRUE, TRUE, 0);

    player_widget = gtk_drawing_area_new();

    left_revealer = gtk_revealer_new();
    gtk_revealer_set_transition_type(GTK_REVEALER(left_revealer), GTK_REVEALER_TRANSITION_TYPE_SLIDE_RIGHT);
    gtk_revealer_set_transition_duration(GTK_REVEALER(left_revealer), 200);
    list_left = gtk_vbutton_box_new();
    gtk_button_box_set_layout(GTK_BUTTON_BOX(list_left), GTK_BUTTONBOX_START);
    gtk_container_add(GTK_CONTAINER(left_revealer), list_left);

    right_revealer = gtk_revealer_new();
    gtk_revealer_set_transition_type(GTK_REVEALER(right_revealer), GTK_REVEALER_TRANSITION_TYPE_SLIDE_LEFT);
    gtk_revealer_set_transition_duration(GTK_REVEALER(right_revealer), 200);
    list_right = gtk_vbutton_box_new();
    gtk_button_box_set_layout(GTK_BUTTON_BOX(list_right), GTK_BUTTONBOX_START);
    gtk_container_add(GTK_CONTAINER(right_revealer), list_right);

    gtk_box_pack_start(GTK_BOX(room_hbox), left_revealer, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(room_hbox), player_widget, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(room_hbox), right_revealer, FALSE, FALSE, 0);

    //setup bottom controls
    button_hbox = gtk_hbox_new(FALSE, 10);
    gtk_box_pack_end(GTK_BOX(main_vbox), button_hbox, FALSE, FALSE, 10);

    //setup buttons
    //playpause_button = gtk_button_new_from_stock(GTK_STOCK_MEDIA_PLAY);
    //playpause_button = gtk_button_new_with_label("gtk-media-play");
    //gtk_button_set_use_stock(GTK_BUTTON(playpause_button), TRUE);
    playpause_button = gtk_button_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_BUTTON);
    g_signal_connect(playpause_button, "clicked", G_CALLBACK(on_playpause), this);

    show_rooms_button = gtk_button_new_from_icon_name("go-home", GTK_ICON_SIZE_BUTTON);
    g_signal_connect(show_rooms_button, "clicked", G_CALLBACK(on_show_rooms), this);

    show_playlist_button = gtk_button_new_from_icon_name("video-x-generic", GTK_ICON_SIZE_BUTTON);
    g_signal_connect(show_playlist_button, "clicked", G_CALLBACK(on_show_playlist), this);


    left_box = gtk_hbutton_box_new();
    gtk_button_box_set_layout(GTK_BUTTON_BOX(left_box), GTK_BUTTONBOX_CENTER);
    gtk_box_pack_start(GTK_BOX(button_hbox), left_box, TRUE, FALSE, 0);

    playpause_box = gtk_hbutton_box_new();
    gtk_button_box_set_layout(GTK_BUTTON_BOX(playpause_box), GTK_BUTTONBOX_CENTER);
    gtk_box_pack_start(GTK_BOX(button_hbox), playpause_box, TRUE, FALSE, 0);

    right_box = gtk_hbutton_box_new();
    gtk_button_box_set_layout(GTK_BUTTON_BOX(right_box), GTK_BUTTONBOX_CENTER);
    gtk_box_pack_end(GTK_BOX(button_hbox), right_box, TRUE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(playpause_box), playpause_button, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(left_box), show_rooms_button, FALSE, FALSE, 0);

    gtk_box_pack_end(GTK_BOX(right_box), show_playlist_button, FALSE, FALSE, 0);



    //setup vlc
    char* argv_vlc[] = {
            "",
            "--no-xlib"
    };
    int argc_vlc = sizeof(argv_vlc) / sizeof(char*);

    vlc_inst = libvlc_new(0, NULL);
    media_player = libvlc_media_player_new(vlc_inst);

    g_signal_connect(G_OBJECT(player_widget), "realize", G_CALLBACK(player_widget_on_realize), this);

    gtk_widget_show_all(window);

    errno = 0;
}

void Player::clearLeftList() {
    GList *children, *iter;

    children = gtk_container_get_children(GTK_CONTAINER(list_left));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);
}

void Player::fillRoomsList() {
    clearLeftList();

    GtkWidget* label0 = gtk_label_new("Комнаты");
    gtk_box_pack_start(GTK_BOX(list_left), label0, FALSE, FALSE, 0);


    for (Room* room : Client::getClient()->getServer()->getRooms())
    {
        GtkWidget* entry = gtk_hbutton_box_new();
        gtk_button_box_set_layout(GTK_BUTTON_BOX(entry), GTK_BUTTONBOX_CENTER);
        gtk_widget_set_size_request(entry, 160, 72);

        std::string text = std::to_string(room->getID());

        GtkWidget* label = gtk_label_new(text.data());

        GtkWidget* button = gtk_button_new_from_icon_name("go-home", GTK_ICON_SIZE_BUTTON);
        gtk_widget_set_size_request(button, 40, 40);

        auto context = gtk_widget_get_style_context(button);
        gtk_style_context_add_class(context,"room_button");

        gtk_box_pack_start(GTK_BOX(entry), label, TRUE, TRUE, 0);
        gtk_box_pack_end(GTK_BOX(entry), button, FALSE, FALSE, 0);

        gtk_box_pack_start(GTK_BOX(list_left), entry, FALSE, FALSE, 0);

        g_signal_connect(button, "clicked", G_CALLBACK(on_room_pick), room);
    }

    gtk_revealer_set_reveal_child(GTK_REVEALER(left_revealer), true);

    gtk_widget_show_all(window);
}

void Player::clearRightList() {
    GList *children, *iter;

    children = gtk_container_get_children(GTK_CONTAINER(list_right));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);
}

void Player::fillPlayList() {
    clearRightList();

    GtkWidget* label0 = gtk_label_new("Плейлист");
    gtk_box_pack_start(GTK_BOX(list_right), label0, FALSE, FALSE, 0);


    for (Room* room : Client::getClient()->getServer()->getRooms())
    {
        GtkWidget* entry = gtk_hbutton_box_new();
        gtk_button_box_set_layout(GTK_BUTTON_BOX(entry), GTK_BUTTONBOX_CENTER);
        gtk_widget_set_size_request(entry, 160, 72);

        std::string text = std::to_string(room->getID());

        GtkWidget* label = gtk_label_new(text.data());

        GtkWidget* button = gtk_button_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_BUTTON);
        gtk_widget_set_size_request(button, 30, 30);

        gtk_box_pack_start(GTK_BOX(entry), button, FALSE, FALSE, 0);
        gtk_box_pack_end(GTK_BOX(entry), label, TRUE, TRUE, 0);

        gtk_box_pack_start(GTK_BOX(list_right), entry, FALSE, FALSE, 0);

        g_signal_connect(button, "clicked", G_CALLBACK(on_room_pick), room);
    }

    gtk_revealer_set_reveal_child(GTK_REVEALER(right_revealer), true);

    gtk_widget_show_all(window);
}