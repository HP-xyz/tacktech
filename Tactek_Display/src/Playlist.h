/*
 * File:   Playlist.h
 * Author: dvorak
 *
 * Created on 23 June 2012, 10:58
 */

#ifndef PLAYLIST_H
#define	PLAYLIST_H

#include <QString>
#include <QList>
#include <QPair>

#ifdef _DEBUG
#include <iostream>
#endif //_DEBUG

class Playlist
{
public:
    Playlist();
    Playlist(const Playlist& orig);
    virtual ~Playlist();
    QList< QPair<QString, int> > get_playlist();
    bool add_to_playlist(QPair<QString, int>);
    int current_index_of_queue;
private:
    QList<QString> m_filepath;
    QList<int> m_pause;
};

#endif	/* PLAYLIST_H */

