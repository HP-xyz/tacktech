/*
 * File:   Playlist.cpp
 * Author: dvorak
 *
 * Created on 23 June 2012, 10:58
 */
#include "Playlist.h"

Playlist::Playlist()
{
    Playlist::current_index_of_queue = 0;
#ifdef _DEBUG
    QPair<QString, int> item;
    item.first = "/windows/F/Belaglik/183285456_www.kepfeltoltes.hu_.jpg";
    item.second = 2;
    add_to_playlist(item);
    item.first = "/windows/F/Belaglik/Amazing_Football_Catch.wmv";
    item.second = 0;
    add_to_playlist(item);
    item.first = "//windows/F/Belaglik/Don't Judge too Quickly 8.flv";
    item.second = 0;
    add_to_playlist(item);
#endif //_DEBUG
}

Playlist::Playlist(const Playlist& orig)
{
}

Playlist::~Playlist()
{
}

/**
 * This function will return a playlist as a QList of QPair<filepath, pause>
 * values, where filepath is an QString and pause is an int.
 * @return Playlist as QList<QPair<filepath, pause>>
 */
QList< QPair<QString, int> > Playlist::get_playlist()
{
    QList< QPair<QString, int> > playlist;
    QPair<QString, int> playlist_item;
    for(int i = 0; i < Playlist::m_filepath.size(); i++)
    {
        playlist_item.first = Playlist::m_filepath[i];
        playlist_item.second = Playlist::m_pause[i];
        playlist.append(playlist_item);
    }
    return playlist;
}

/**
 * Function will add the QPair parameter to the playlist. The format for the
 * parameters are <filepath, pause>.
 * @param playlist_item QPair<QString, int>
 * @return true {TODO: ADD ACTUAL VALIDATION}
 */
bool Playlist::add_to_playlist(QPair<QString, int> playlist_item)
{
    Playlist::m_filepath.append(playlist_item.first);
    Playlist::m_pause.append(playlist_item.second);
    return true;
}
