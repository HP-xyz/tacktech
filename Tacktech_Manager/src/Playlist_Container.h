#ifndef PLAYLIST_CONTAINER_H
#define PLAYLIST_CONTAINER_H
#include <QMultiMap>
#include <QPair>

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG
class Playlist_Container
{
public:
	Playlist_Container();
	~Playlist_Container();
	bool add_playlist_name(QString);
	bool add_filename(QString, QString, int);
	QMultiMap<QString, QPair<QString, int> > get_playlist();

#ifdef _DEBUG
	void print_contents();
#endif // _DEBUG
private:
	QMultiMap<QString, QPair<QString, int> > playlist;
	QPair<QString, int> temp_pair;
};

#endif //PLAYLIST_CONTAINER_H