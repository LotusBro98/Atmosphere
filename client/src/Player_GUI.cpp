//
// Created by amburkoff on 11.05.19.
//

#include "../include/Room.h"
#include "../include/Client.h"
#include "../include/Player.h"

void Player::init(int argc, char* argv[]) {

    gtk_init(&argc, &argv);

    // setup window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(destroy_cb), this);
    gtk_container_set_border_width(GTK_CONTAINER (window), 0);
    gtk_window_set_title(GTK_WINDOW(window), "VKino");

    //hbox_main = gtk_hbox_new(FALSE, 0);

    //setup left list
    //left_revealer = gtk_revealer_new();
    //gtk_box_pack_start(GTK_BOX(hbox_main), left_revealer, FALSE, FALSE, 0);

    //list_left = gtk_vbutton_box_new();
    //gtk_button_box_set_layout(GTK_BUTTON_BOX(list_left), GTK_BUTTONBOX_START);
    //gtk_container_add(GTK_CONTAINER(left_revealer), list_left);

    //setup vbox
    vbox = gtk_vbox_new(FALSE, 0);
    //gtk_box_pack_start(GTK_BOX(hbox_main), vbox, TRUE, TRUE, 0);

    //setup player widget
    player_widget = gtk_drawing_area_new();
    gtk_box_pack_start(GTK_BOX(vbox), player_widget, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(window), vbox);
    //gtk_container_add(GTK_CONTAINER(overlay_main), player_widget);
    //gtk_overlay_add_overlay(GTK_OVERLAY(overlay_main), vbox);
    //gtk_overlay_add_overlay(GTK_OVERLAY(overlay_main), hbox_main);
    //gtk_overlay_set_overlay_pass_through(GTK_OVERLAY(overlay_main), hbox_main, TRUE);


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
    gtk_box_pack_end(GTK_BOX(vbox), hbuttonbox, FALSE, FALSE, 0);

    //setup vlc
    vlc_inst = libvlc_new(argc, argv);
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

    for (Room* room : Client::getClient()->getServer()->getRooms())
    {
        GtkWidget* button = gtk_button_new_with_label(std::to_string(room->getID()).data());
        gtk_box_pack_start(GTK_BOX(list_left), button, FALSE, FALSE, 0);

        std::cout << room->getID() << "\n";
    }

    gtk_widget_show_all(window);

    gtk_revealer_set_transition_type(GTK_REVEALER(left_revealer), GtkRevealerTransitionType::GTK_REVEALER_TRANSITION_TYPE_SLIDE_RIGHT);
    gtk_revealer_set_transition_duration(GTK_REVEALER(left_revealer), 200);
    gtk_revealer_set_reveal_child(GTK_REVEALER(left_revealer), true);
}