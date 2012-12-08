#include "Playlist.h"

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

Playlist::Playlist(void)
{
}


Playlist::~Playlist(void)
{
}

std::string Playlist::get_playlist_name()
{
	return m_playlist_name;
}

std::vector< std::pair<std::string,int> > Playlist::get_playlist_items()
{
	return m_playlist_items;
}

int Playlist::get_current_item_index()
{
	return m_current_item_index;
}

boost::posix_time::time_duration Playlist::get_start_time()
{
	return m_start_time.time_of_day();
}

boost::posix_time::time_duration Playlist::get_end_time()
{
	return m_end_time.time_of_day();
}

void Playlist::set_playlist_name( std::string p_playlist_name)
{
	m_playlist_name = p_playlist_name;
}

void Playlist::set_playlist_items( std::vector< std::pair<std::string,int> > p_playlist_items)
{
	m_playlist_items = p_playlist_items;
}

void Playlist::add_playlist_item( std::string filename, int pause)
{
	m_playlist_items.push_back(std::pair<std::string, int>(filename, pause));
}

void Playlist::remove_playlist_item( int index)
{
	m_playlist_items.erase(m_playlist_items.begin() + (index-1));
}

void Playlist::set_current_item_index( int p_current_item_index)
{
	m_current_item_index = p_current_item_index;
}

void Playlist::set_start_time( boost::posix_time::time_duration p_time_of_day)
{//This is so ugly... I suppose I should've used gregorian dates or something
	std::string new_start_time = boost::posix_time::to_iso_string(p_time_of_day);
	std::string old_date_time = boost::posix_time::to_iso_string(m_start_time);
	std::string true_new_start_time = old_date_time.substr(0, 9);
	true_new_start_time += new_start_time.substr(0, 6);
	m_start_time = boost::posix_time::ptime(boost::posix_time::from_iso_string(true_new_start_time));
}

void Playlist::set_end_time( boost::posix_time::time_duration p_time_of_day)
{
	std::string new_end_time = boost::posix_time::to_iso_string(p_time_of_day);
	std::string old_date_time = boost::posix_time::to_iso_string(m_end_time);
	std::string true_new_end_time = old_date_time.substr(0, 9);
	true_new_end_time += new_end_time.substr(0, 6);
	m_end_time = boost::posix_time::ptime(boost::posix_time::from_iso_string(true_new_end_time));
}

std::string Playlist::get_playlist_xml()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist::get_playlist_xml" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	pugi::xml_document playlist_document;
	pugi::xml_node root_node =
		playlist_document.append_child("Playlist");
	root_node.append_attribute("Playlist_Name") = get_playlist_name().c_str();
	root_node.append_attribute("Current_Item_Index") = get_current_item_index();
	root_node.append_attribute("Start_Time") = boost::posix_time::to_iso_string(get_start_time()).c_str();
	root_node.append_attribute("End_Time") = boost::posix_time::to_iso_string(get_end_time()).c_str();
	for(std::vector< std::pair<std::string,int> >::iterator it =
		m_playlist_items.begin(); it != m_playlist_items.end(); ++it)
	{
		pugi::xml_node playlist_item_node =
			root_node.append_child("Playlist_Item");
		playlist_item_node.append_attribute("Filename") = it->first.c_str();
		playlist_item_node.append_attribute("Pause") = it->second;
	}
	xml_string_writer writer;
	playlist_document.print(writer);
	return writer.result;
}

