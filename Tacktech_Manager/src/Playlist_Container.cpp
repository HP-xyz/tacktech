#include "Playlist_Container.h"
Playlist_Container::Playlist_Container()
{
	temp_pair.first = "TEMP_ITEM";
	temp_pair.second = 0;
}

Playlist_Container::~Playlist_Container()
{

}

/** Function to add a playlist name to the Playlist_Container. Returns 
 ** false if the playlist name already exists. */
bool Playlist_Container::add_playlist_name( QString playlist_name)
{
	if (playlist.contains(playlist_name))
	{
		return false;
	}
	else
	{
		playlist.insert(playlist_name, temp_pair);
		return true;
	}
}

/** Function to add a filename and pause to a playlist */
bool Playlist_Container::add_filename(
	QString playlist_name, QString filename, int pause)
{
#ifdef _DEBUG
	print_contents();
	std::cout << "= Playlist_Container::add_filename()" << std::endl;
#endif // _DEBUG
	QPair<QString, int> item_to_add;
	item_to_add.first = filename;
	item_to_add.second = pause;
#ifdef _DEBUG
	std::cout << " - playlist.values(playlist_name).at(0).first: " 
		<< qPrintable(playlist.values(playlist_name).at(0).first)
		<< std::endl;
	std::cout << " - temp_pair.first: " << qPrintable(temp_pair.first) 
		<< std::endl;
	std::cout << " - playlist.values(playlist_name).at(0).second: " 
		<< qPrintable(playlist.values(playlist_name).at(0).second)
		<< std::endl;
	std::cout << " - temp_pair.second: " << qPrintable(temp_pair.second)
		<< std::endl;
#endif // _DEBUG
	if(playlist.values(playlist_name).at(0).first == temp_pair.first
		&& playlist.values(playlist_name).at(0).second ==
		temp_pair.second)
	{//Removing the temp item, if needed
#ifdef _DEBUG
		std::cout << " - Replacing the temp variable" << std::endl;
#endif // _DEBUG
		playlist.remove(playlist_name);
	}
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
