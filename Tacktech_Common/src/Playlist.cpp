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

template <class T>
std::string make_list( T p_vector)
{
	std::string comma_separated_list;
	for (typename T::iterator it = p_vector.begin();
		it != p_vector.end(); ++it)
	{
		comma_separated_list += *it;
		comma_separated_list += ", ";
	}
	comma_separated_list =
		comma_separated_list.substr(0, comma_separated_list.length() - 2);
	return comma_separated_list;
}

Playlist::Playlist(void)
{
	m_groups.reset(new std::set<std::string>);
	m_playlist_items.reset(new std::vector<std::pair<std::string, int> >);
	m_start_time = ALL_DAY_TIME;
	m_end_time = ALL_DAY_TIME;
	set_current_item_index(-1);
	currently_active = false;
}

Playlist::Playlist( std::string playlist_str)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist::Playlist(STRING)" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	m_groups.reset(new std::set<std::string>);
	m_playlist_items.reset(new std::vector<std::pair<std::string, int> >);
	m_start_time = ALL_DAY_TIME;
	m_end_time = ALL_DAY_TIME;

	pugi::xml_document playlist_document;
	playlist_document.load(playlist_str.c_str());
	pugi::xml_node playlist_node = playlist_document.child("Playlist_Node");
	set_playlist_name(playlist_node.child("Playlist").attribute("Playlist_Name").as_string());
	set_current_item_index(playlist_node.child("Playlist").attribute("Current_Item_Index").as_int());

	/* Create time_duration objects for start and end time using substrings */
	std::string start_time_str = playlist_node.child("Playlist").attribute("Start_Time").as_string();
	std::string end_time_str = playlist_node.child("Playlist").attribute("End_Time").as_string();
	boost::posix_time::time_duration start_time(
		boost::lexical_cast<int,std::string>(start_time_str.substr(0, 2)),
		boost::lexical_cast<int,std::string>(start_time_str.substr(2, 2)),
		boost::lexical_cast<int,std::string>(start_time_str.substr(4)),
		0);
	boost::posix_time::time_duration end_time(
		boost::lexical_cast<int,std::string>(end_time_str.substr(0, 2)),
		boost::lexical_cast<int,std::string>(end_time_str.substr(2, 2)),
		boost::lexical_cast<int,std::string>(end_time_str.substr(4)),
		0);

	set_start_time(start_time);
	set_end_time(end_time);
	if (playlist_node.child("Playlist").attribute("Currently_Active").as_int() == 1)
		currently_active = true;
	else
		currently_active = false;

	for (pugi::xml_node playlist_item_node = playlist_node.child("Playlist").child("Playlist_Item");
		playlist_item_node; playlist_item_node = playlist_item_node.next_sibling("Playlist_Item"))
	{
		add_playlist_item(playlist_item_node.attribute("Filename").as_string(),
			playlist_item_node.attribute("Pause").as_int());
	}
}


Playlist::~Playlist(void)
{
}

std::string Playlist::get_playlist_name()
{
	return m_playlist_name;
}

boost::shared_ptr<std::vector< std::pair<std::string,int> > > Playlist::get_playlist_items()
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
	m_playlist_items.reset(new std::vector<std::pair<std::string, int> >(p_playlist_items));
}

void Playlist::add_playlist_item( std::string filename, int pause)
{
	m_playlist_items->push_back(std::pair<std::string, int>(filename, pause));
}

void Playlist::remove_playlist_item( int index)
{
	m_playlist_items->erase(m_playlist_items->begin() + (index-1));
}

void Playlist::set_current_item_index( int p_current_item_index)
{
	m_current_item_index = p_current_item_index;
}

void Playlist::set_start_time( boost::posix_time::time_duration p_time_of_day)
{//This is so ugly... I suppose I should've used gregorian dates or something
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist::set_start_time(TIME_DURATION)" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	std::string new_start_time = boost::posix_time::to_iso_string(p_time_of_day);
	std::string old_date_time = boost::posix_time::to_iso_string(m_start_time);
	std::string true_new_start_time = old_date_time.substr(0, 9);
	true_new_start_time += new_start_time.substr(0, 6);
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - New_Start_Date: " << new_start_time << std::endl;
	std::cout << " - Old Date_Time: " << old_date_time << std::endl;
	std::cout << " - True_New_Start_Time:" << true_new_start_time << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	m_start_time = boost::posix_time::ptime(boost::posix_time::from_iso_string(true_new_start_time));
}

void Playlist::set_start_time( boost::posix_time::ptime p_time)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist::set_start_time(PTIME)" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	m_start_time = p_time;
}

void Playlist::set_end_time( boost::posix_time::time_duration p_time_of_day)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist::set_end_time(TIME_DURATION)" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	std::string new_end_time = boost::posix_time::to_iso_string(p_time_of_day);
	std::string old_date_time = boost::posix_time::to_iso_string(m_end_time);
	std::string true_new_end_time = old_date_time.substr(0, 9);
	true_new_end_time += new_end_time.substr(0, 6);
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - New_Start_Date: " << new_end_time << std::endl;
	std::cout << " - Old Date_Time: " << old_date_time << std::endl;
	std::cout << " - True_New_Start_Time:" << true_new_end_time << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	m_end_time = boost::posix_time::ptime(boost::posix_time::from_iso_string(true_new_end_time));
}

void Playlist::set_end_time( boost::posix_time::ptime p_time)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist::set_end_time(PTIME)" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	m_end_time = p_time;
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
	if (currently_active)
		root_node.append_attribute("Currently_Active") = 1;
	else
		root_node.append_attribute("Currently_Active") = 0;
	for(std::vector< std::pair<std::string,int> >::iterator it =
		m_playlist_items->begin(); it != m_playlist_items->end(); ++it)
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

void Playlist::set_groups( std::set<std::string> p_groups)
{
	m_groups.reset(new std::set<std::string>(p_groups));
}

void Playlist::add_group( std::string p_group)
{
	if (get_groups()->find(p_group) == get_groups()->end())
		m_groups->insert(p_group);
}

boost::shared_ptr<std::set<std::string> > Playlist::get_groups()
{
	return m_groups;
}

bool Playlist::contains_group( std::string p_group)
{
	std::set<std::string>::iterator iter = get_groups()->find(p_group);
	if (iter != get_groups()->end())
		return true;
	else
		return false;
}

void Playlist::remove_item( std::string item_name)
{
	bool item_removed = false;
	std::vector< std::pair<std::string,int> >::iterator it = get_playlist_items()->begin();
	while (!item_removed && it != get_playlist_items()->end())
	{
		if (it->first == item_name)
		{
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << " REMOVED ITEM: " << it->first << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
			get_playlist_items()->erase(it++);
			item_removed = true;
		}
		if (!item_removed)
			++it;
	}
}
#ifdef _SHOW_DEBUG_OUTPUT
void Playlist::print_contents()
{
	std::cout << "Playlist_Name: " << get_playlist_name() << std::endl;
	std::cout << "Playlist_Groups: " << make_list(*get_groups()) << std::endl;
	std::cout << "Playlist_Items: " << std::endl;
	for (std::vector<std::pair<std::string, int> >::iterator it = get_playlist_items()->begin();
		it != get_playlist_items()->end(); ++it)
	{
		std::cout << " - " << it->first << std::endl;
	}
}
#endif // _SHOW_DEBUG_OUTPUT
