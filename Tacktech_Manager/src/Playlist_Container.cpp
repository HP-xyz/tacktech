#include "Playlist_Container.h"
Playlist_Container::Playlist_Container()
{

}
Playlist_Container::~Playlist_Container()
{

}

/** Function to add a playlist name to the Playlist_Container. Returns 
 ** false if the playlist name already exists. */
bool Playlist_Container::add_playlist_name( QString playlist_name)
{
	if (!playlist.contains(playlist_name))
	{
		return false;
	}
	else
	{
		QPair<QString, int> temp_pair;
		temp_pair.first = "";
		temp_pair.second = 0;
		playlist.insert(playlist_name, temp_pair);
		return true;
	}
}

/** Function to add a filename and pause to a playlist */
bool Playlist_Container::add_filename(
	QString playlist_name, QString filename, int pause)
{
	/* Removing the temp item, if it exists */
	QPair<QString, int> *temp_item = new QPair<QString, int>;
	temp_item->first = "";
	temp_item->second = 0;
	playlist.remove(playlist_name, *temp_item);
	delete temp_item;

	/* Adding the actual item */
	QPair<QString, int> item_to_add;
	item_to_add.first = filename;
	item_to_add.second = pause;
	playlist.insert(playlist_name, item_to_add);
	return true;
}

/** Function to return the playlist to the calling class */
QMultiMap<QString, QPair<QString, int> > Playlist_Container::get_playlist()
{
	return playlist;
}

#ifdef _DEBUG
/** Function to print the contents of Playlist_Container. Only compiled
 ** in debug mode */
void Playlist_Container::print_contents()
{
	std::cout << "= Playlist_Container::print_contents()" << std::endl;
	foreach(QString playlist_name, playlist.keys())
	{
		std::cout << qPrintable(playlist_name) << std::endl;
		for (int i =0; i < playlist.values(playlist_name).size(); i++)
		{
			std::cout << "  - " 
				<< qPrintable(playlist.values(playlist_name).at(i).first) 
				<< " : "
				<< playlist.values(playlist_name).at(i).second
				<< std::endl;
		}
	}
}
#endif // _DEBUG
