#include "Playlist_Container.h"

/** Struct to define a xml_writer to string.
 ** Copied directly from the pugixml quickstart */
struct xml_string_writer: pugi::xml_writer
{
	std::string result;

	virtual void write(const void* data, size_t size)
	{
		result += std::string(static_cast<const char*>(data), size);
	}
};

Playlist_Container::Playlist_Container()
{
	temp_pair.first = "TEMP_ITEM";
	temp_pair.second = 0;
	playlist.reset(new Playlist_Multimap());
}

Playlist_Container::~Playlist_Container()
{

}

bool Playlist_Container::add_playlist_name(std::string playlist_name)
{
	if (Playlist_Container::contains_filename_in_all_playlists(playlist_name)
			!= playlist->end())
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
bool Playlist_Container::add_filename(std::string playlist_name,
		std::string filename, int pause)
{
#ifdef _DEBUG
	std::cout << "= Playlist_Container::add_filename()" << std::endl;
#endif // _DEBUG
	std::pair<std::string, int> item_pair;
	item_pair.first = filename;
	item_pair.second = pause;

	playlist->insert(
			std::pair<std::string, std::pair<std::string, int> >(playlist_name,
					item_pair));
#ifdef _DEBUG
	std::cout << " - Function should now contain item with: " << std::endl;
	std::cout << "  - Key: " << playlist_name << std::endl;
	std::cout << "  - Filename: " << filename << std::endl;
	std::cout << "  - Pause: " << pause << std::endl;
#endif // _DEBUG
	/** Removing temp item if found */
#ifdef _DEBUG
	std::cout << " - Calling remove_filename_from_playlist" << std::endl;
#endif // _DEBUG
	Playlist_Container::remove_filename_from_playlist(playlist_name,
			"TEMP_ITEM");

	return true;
}

/** Removes a playlist from the playlist variable. Parameter must
 ** be a playlist name */
void Playlist_Container::remove_playlist(std::string playlist_name)
{
#ifdef _DEBUG
	std::cout << "=Playlist_Container::remove_playlist()" << std::endl;
#endif // _DEBUG

	int count = playlist->erase(playlist_name);
#ifdef _DEBUG
	std::cout << " - Removed " << count << " items" << std::endl;
#endif // _DEBUG

}

/** Function to return the playlist to the calling class */
Playlist_Ptr Playlist_Container::get_playlist()
{
	return playlist;
}

Playlist_Multimap::iterator Playlist_Container::contains_filename_in_all_playlists(
		std::string filename)
{
	for (Playlist_Multimap::iterator it = playlist->begin();
			it != playlist->end(); it++)
	{
		Playlist_Range range = playlist->equal_range(it->first);
		for (Playlist_Multimap::iterator it2 = range.first; it2 != range.second;
				++it2)
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
		std::string playlist_name, std::string filename)
{
#ifdef _DEBUG
	std::cout << "=Playlist_Container::remove_filename_from_playlist()"
			<< std::endl;
#endif // _DEBUG
	Playlist_Range range = playlist->equal_range(playlist_name);
	for (Playlist_Multimap::iterator it2 = range.first; it2 != range.second;
			++it2)
	{
		if (playlist_name == it2->first && filename == it2->second.first)
		{
#ifdef _DEBUG
			std::cout << "  - Removing: " << filename << std::endl;
#endif // _DEBUG

			playlist->erase(it2);
			return;
		}
	}
}

Playlist_Multimap Playlist_Container::get_unique_playlists()
{
	Playlist_Multimap out_map;
	for (Playlist_Multimap::iterator it1 = playlist->begin();
			it1 != playlist->end(); it1++)
	{
		bool playlist_duplicate = false;
		for (Playlist_Multimap::iterator it2 = out_map.begin();
				it2 != out_map.end(); it2++)
		{
			if (it2->first == it1->first)
			{
				playlist_duplicate = true;
			}
		}
		if (!playlist_duplicate)
		{
			std::pair<std::string, std::pair<std::string, int> > t_pair;
			t_pair.first = it1->first;
			t_pair.second.first = it1->second.first;
			t_pair.second.second = it1->second.second;
			out_map.insert(t_pair);
		}
	}
	return out_map;
}

Playlist_Range Playlist_Container::get_files_in_playlist(
		std::string playlist_name)
{
	return playlist->equal_range(playlist_name);
}

#ifdef _DEBUG
/** Function to print the contents of Playlist_Container. Only compiled
 ** in debug mode */
void Playlist_Container::print_contents()
{
	std::cout << "= Playlist_Container::print_contents()" << std::endl;
	std::string out_string;
	Playlist_Multimap unique_map = Playlist_Container::get_unique_playlists();
	for (Playlist_Multimap::iterator it = unique_map.begin();
			it != unique_map.end(); it++)
	{
		out_string += it->first;
		out_string += "\n";
		out_string += "    ";
		Playlist_Range range = playlist->equal_range(it->first);
		for (Playlist_Multimap::iterator it2 = range.first; it2 != range.second;
				++it2)
		{
			out_string += it2->second.first;
			out_string += ":";
			out_string += boost::lexical_cast<std::string>(it2->second.second);
			out_string += "    ";
		}
		out_string += "\n";
	}
	std::cout << out_string << std::endl;
}
#endif // _DEBUG
std::string Playlist_Container::get_playlists_xml()
{
	pugi::xml_document playlist_document;
	pugi::xml_node root_node_playlist = playlist_document.append_child(
			"Playlist");
	Playlist_Multimap playlist_map = get_unique_playlists();
	for (Playlist_Multimap::iterator it = playlist_map.begin();
			it != playlist_map.end(); it++)
	{
		pugi::xml_node playlist_node = root_node_playlist.append_child(
				"Playlist_Item");
		playlist_node.append_attribute("Playlist_Name") = it->first.c_str();
		Playlist_Range playlist_range = get_files_in_playlist(it->first);
		for (Playlist_Multimap::iterator it2 = playlist_range.first;
				it2 != playlist_range.second; ++it2)
		{
			pugi::xml_node item_node = playlist_node.append_child("Item");
			pugi::xml_node filename_node = item_node.append_child("Filename");
			pugi::xml_node pause_node = item_node.append_child("Pause");
			pugi::xml_node filename_pcdata = filename_node.append_child(
					pugi::node_pcdata);
			pugi::xml_node pause_pcdata = pause_node.append_child(
					pugi::node_pcdata);

			filename_pcdata.set_value(it2->second.first.c_str());
			pause_pcdata.set_value(
					boost::lexical_cast<std::string>(it2->second.second).c_str());
		}
	}
	xml_string_writer writer;
	playlist_document.save(writer);
	return writer.result;
}

void Playlist_Container::construct_playlist(std::string playlist)
{
#ifdef _DEBUG
	std::cout << "=Playlist_Container::construct_playlist()" << std::endl;
	std::cout << playlist << std::endl;
#endif // _DEBUG
	pugi::xml_document playlist_document;
	playlist_document.load(playlist.c_str());
	pugi::xml_node root_node = playlist_document.child("Playlist");
	for (pugi::xml_node playlist_item = root_node.child("Playlist_Item");
			playlist_item;
			playlist_item = playlist_item.next_sibling("Playlist_Item"))
	{
		for (pugi::xml_node item_node = playlist_item.child("Item"); item_node;
				item_node = item_node.next_sibling("Item"))
		{
			add_filename(playlist_item.attribute("Playlist_Name").as_string(),
					item_node.child_value("Filename"),
					boost::lexical_cast<int>(item_node.child_value("Pause")));
		}
	}
}

void Playlist_Container::construct_playlist(const char *playlist_filename)
{
	/* Read playlist from file */
	pugi::xml_document playlist_document;
	playlist_document.load_file(playlist_filename);
	pugi::xml_node root_node = playlist_document.child("Playlist");
	for (pugi::xml_node playlist_item = root_node.child("Playlist_Item");
			playlist_item;
			playlist_item = playlist_item.next_sibling("Playlist_Item"))
	{
		for (pugi::xml_node item_node = playlist_item.child("Item"); item_node;
				item_node = item_node.next_sibling("Item"))
		{
			add_filename(playlist_item.attribute("Playlist_Name").as_string(),
					item_node.child_value("Filename"),
					boost::lexical_cast<int>(item_node.child_value("Pause")));
		}
	}
}

void Playlist_Container::reset_container()
{
	playlist.reset(new Playlist_Multimap());
}

