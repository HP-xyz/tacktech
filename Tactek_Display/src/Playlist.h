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
#include <QDate>

#ifdef _DEBUG
#include <iostream>
#endif //_DEBUG
class Playlist
{
public:
	Playlist();
	Playlist(const Playlist& orig);
	virtual ~Playlist();
	QList<QPair<QString, int> > get_playlist();
	bool add_to_playlist(QPair<QString, int>);
	void set_display_time(QDate p_display_time);
	int current_index_of_queue;
private:
	QList<QString> m_filepath;
	QList<int> m_pause;
	QDate display_time;
};

#endif	/* PLAYLIST_H */

