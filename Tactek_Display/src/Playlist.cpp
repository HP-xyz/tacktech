/*
 * File:   Playlist.cpp
 * Author: dvorak
 *
 * Created on 23 June 2012, 10:58
 */
#include "Playlist.h"

Playlist::Playlist()
{
#ifdef _DEBUG
	std::cout << "= Playlist()" << std::endl;
#endif //_DEBUG
	Playlist::current_index_of_queue = 0;
}

Playlist::Playlist(const Playlist& orig)
{
	current_index_of_queue = 0;
}

Playlist::~Playlist()
{
}

/**
 * This function will return a playlist as a QList of QPair<filepath, pause>
 * values, where filepath is an QString and pause is an int.
 * @return Playlist as QList<QPair<filepath, pause>>
 */
QList<QPair<QString, int> > Playlist::get_playlist()
{
	QList<QPair<QString, int> > playlist;
	QPair<QString, int> playlist_item;
	for (int i = 0; i < Playlist::m_filepath.size(); i++)
	{
		playlist_item.first = Playlist::m_filepath[i];
		playlist_item.second = Playlist::m_pause[i];
		playlist.append(playlist_item);
	}
	return (playlist);
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
	return (true);
}

void Playlist::set_display_time(QDate p_display_time)
{
	display_time = p_display_time;
}
