#include "Playlist_Container.h"
Playlist_Container::Playlist_Container()
{
    temp_pair.first = "TEMP_ITEM";
    temp_pair.second = 0;
	playlist.reset(new std::multimap<std::string, std::pair<std::string, int> >);
}

Playlist_Container::~Playlist_Container()
{

}

bool Playlist_Container::add_playlist_name( std::string playlist_name)
{
    if (Playlist_Container::contains_filename_in_all_playlists(
		playlist_name) == playlist->end())
    {
        return false;
    }
    else
    {
		std::pair<std::string, std::pair<std::string, int> > temp_pair2;
		temp_pair2.first = playlist_name;
		temp_pair2.second = temp_pair;
        playlist->insert(temp_pair2);
        return true;
    }
}

/** Function to add a filename and pause to a playlist.
 ** NOTE: This function takes linear time to complete*/
bool Playlist_Container::add_filename(
    std::string playlist_name, std::string filename, int pause)
{
#ifdef _DEBUG
    std::cout << "= Playlist_Container::add_filename()" << std::endl;
#endif // _DEBUG
    std::pair<std::string, int> item_to_add;
    item_to_add.first = filename;
    item_to_add.second = pause;
    if (!playlist->size() == 0)
    {
		Playlist_Container::remove_filename_from_playlist(playlist_name,
			"TEMP_ITEM");
    }
	std::pair<std::string, std::pair<std::string, int>> temp_pair2;
	temp_pair2.first = playlist_name;
	temp_pair2.second = item_to_add;
    playlist->insert(temp_pair2);
#ifdef _DEBUG
    std::cout << " - Function should now contain item with: " << std::endl;
    std::cout << "  - Key: " << playlist_name.c_str() << std::endl;
    std::cout << "  - Filename: " << filename.c_str() << std::endl;
    std::cout << "  - Pause: " << pause << std::endl;
#endif // _DEBUG
    return true;
}

/** Removes a playlist from the playlist variable. Parameter must
 ** be a playlist name */
void Playlist_Container::remove_playlist(std::string playlist_name)
{
	std::cout << "=Playlist_Container::remove_playlist()" 
		<< std::endl;
	int count = 0;
	for(Playlist_Multimap::iterator it = playlist->begin();
		it != playlist->end(); it++)
	{
		if (it->first.c_str() == playlist_name.c_str())
		{
			std::cout << "  - Removing: " 
				<< it->first.c_str() << " : " 
				<< it->second.first.c_str() << std::endl;
			playlist->erase(it);
			count += 1;
		}
	}
	std::cout << " - Removed " << count << " items" << std::endl;
}

/** Function to return the playlist to the calling class */
Playlist_Ptr Playlist_Container::get_playlist()
{
    return playlist;
}

Playlist_Multimap::iterator 
	Playlist_Container::contains_filename_in_all_playlists(
	std::string filename)
{
	for(Playlist_Multimap::iterator it = playlist->begin();
		it != playlist->end(); it++)
	{
		Playlist_Range range = playlist->equal_range(it->first);
		for (Playlist_Multimap::iterator it2 = range.first;
			it2 != range.second; ++it2)
		{
			Playlist_Item temp_map = *it2;
			std::string temp_filename = temp_map.first;
			if (temp_filename.c_str() == filename.c_str())
			{
				return it2;
			}
		}
	}
	return playlist->end();
}

void Playlist_Container::remove_filename_from_playlist( 
	std::string playlist_name, std::string filename ) 
{
	std::cout << "=Playlist_Container::remove_filename_from_playlist()" 
		<< std::endl;
	int count = 0;
	for(Playlist_Multimap::iterator it = playlist->begin();
		it != playlist->end(); it++)
	{
		Playlist_Range range = playlist->equal_range(it->first);
		for (Playlist_Multimap::iterator it2 = range.first;
			it2 != range.second; ++it2)
		{
			Playlist_Item temp_map = *it2;
			std::string temp_playlist_name = it->first;
			std::string temp_filename = temp_map.first;
			if (temp_filename.c_str() == filename.c_str()
				&& temp_playlist_name.c_str() == temp_playlist_name.c_str())
			{
				std::cout << "  - Removing: " 
					<< temp_filename.c_str() << std::endl;
				playlist->erase(it2);
				count += 1;
			}
		}
	}
	std::cout << " - Removed " << count << " items" << std::endl;
}

#ifdef _DEBUG
/** Function to print the contents of Playlist_Container. Only compiled
 ** in debug mode */
void Playlist_Container::print_contents()
{
    std::cout << "= Playlist_Container::print_contents()" << std::endl;
	std::string out_string;
	for(Playlist_Multimap::iterator it = playlist->begin();
		it != playlist->end(); it++)
	{
		out_string += it->first;
		out_string += "\n";
		out_string += "    ";
		Playlist_Range range = playlist->equal_range(it->first);
		for (Playlist_Multimap::iterator it2 = range.first;
			it2 != range.second; ++it2)
		{
			out_string += it2->first;
			out_string += ":";
			out_string += boost::lexical_cast<std::string>(it2->second.second);
			out_string += "    ";
		}
		out_string += "\n";
	}
	std::cout << out_string << std::endl;
}
#endif // _DEBUG
