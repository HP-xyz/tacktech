#include "Display_Client.h"

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

Display_Client::Display_Client(void)
{
	m_groups.reset(new std::set<std::string>);
	m_organizations.reset(new std::set<std::string>);
	m_playlist_container.reset(new Playlist_Container());
	m_playlist_container->set_playlist_container_name("NONE");
}

Display_Client::Display_Client( std::string display_client_str)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Display_Client(STRING)" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	m_groups.reset(new std::set<std::string> );
	m_organizations.reset(new std::set<std::string>);
	m_playlist_container.reset(new Playlist_Container());
	pugi::xml_document disply_client_document;
	disply_client_document.load(display_client_str.c_str());
	pugi::xml_node root_node = disply_client_document.child("Display_Client_Item").child("Display_Client");

#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Creating Display_Client: " << root_node.child_value("Identification") << std::endl;
#endif _SHOW_DEBUG_OUTPUT

	set_identification(root_node.child_value("Identification"));
	set_last_ping(boost::posix_time::from_iso_string(root_node.child_value("Last_Ping")));
	set_groups(make_set(root_node.child_value("Groups")));
	set_organizations(make_set(root_node.child_value("Organizations")));
	set_playlist_container_name(root_node.child_value("Playlist_Container"));
	set_playlist_container(root_node.child_value("Display_Playlist_Container"));
}


Display_Client::~Display_Client(void)
{
}

std::string Display_Client::get_identification()
{
	return m_identification;
}
boost::posix_time::ptime Display_Client::get_last_ping()
{
	return m_last_ping;
}
boost::shared_ptr<std::set<std::string> > Display_Client::get_groups()
{
	return m_groups;
}
Playlist_Container_Ptr Display_Client::get_playlist_container()
{
	return m_playlist_container;
}

void Display_Client::set_identification(std::string p_identification)
{
	m_identification = p_identification;
}
void Display_Client::set_last_ping(boost::posix_time::ptime p_last_ping)
{
	m_last_ping = p_last_ping;
}
void Display_Client::set_groups(std::set<std::string> p_groups)
{
	boost::shared_ptr<std::set<std::string> > new_groups(new std::set<std::string>(p_groups));
	m_groups = new_groups;
}
void Display_Client::set_organizations( std::set<std::string> p_organizations)
{
	m_organizations.reset(new std::set<std::string>(p_organizations));
}
void Display_Client::set_playlist_container(Playlist_Container_Ptr p_playlist_container)
{
	m_playlist_container = p_playlist_container;
}

void Display_Client::set_playlist_container( std::string p_playlist_string)
{
	m_playlist_container.reset(new Playlist_Container(p_playlist_string));
}

bool Display_Client::add_group(std::string p_group_name)
{
#ifdef _SHOW_DEBUG_OUTPUT
    std::cout << "   -- Looking for '" << p_group_name << "'" << std::endl;
#endif //_SHOW_DEBUG_OUTPUT
	if(get_groups()->find(p_group_name) != get_groups()->end())
	{
#ifdef _SHOW_DEBUG_OUTPUT
    std::cout << "   --- Group FOUND, NOT INSERTING"  << std::endl;
#endif //_SHOW_DEBUG_OUTPUT
		return false;
	}
	else
	{
#ifdef _SHOW_DEBUG_OUTPUT
    std::cout << "   --- Group NOT FOUND, INSERTING"  << std::endl;
#endif //_SHOW_DEBUG_OUTPUT
		m_groups->insert(p_group_name);
		return true;
	}
}

boost::shared_ptr<std::set<std::string> > Display_Client::get_organizations()
{
	return m_organizations;
}

bool Display_Client::add_organization( std::string p_organization_name)
{
	std::set<std::string>::iterator it =
		get_organizations()->find(p_organization_name);
	if(it != get_organizations()->end())
	{
		return false;
	}
	else
	{
		m_organizations->insert(p_organization_name);
		return true;
	}
}

std::string Display_Client::get_display_client_xml()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Display_Client::get_display_client_xml" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	//pugi::xml_document disply_client_document;
	//pugi::xml_node display_client_root
	//	= disply_client_document.append_child("Display_Client");
	//display_client_root.append_attribute("Identification") =
	//	get_identification().c_str();
	//display_client_root.append_attribute("Last_Ping") =
	//	boost::posix_time::to_iso_string(get_last_ping()).c_str();
	//pugi::xml_node playlist_node = display_client_root.append_child("Playlist_Node");
	//playlist_node.append_child(pugi::node_pcdata).set_value(
	//	get_playlist_container().get()->get_playlist_container_xml().c_str());
	//display_client_root.append_attribute("Playlist_Container") =
	//	get_playlist_container()->get_playlist_container_name().c_str();
	//display_client_root.append_attribute("Organizations") =
	//	make_list(*get_organizations()).c_str();
	//display_client_root.append_attribute("Groups") =
	//	make_list(*get_groups()).c_str();
	//xml_string_writer writer;
	//disply_client_document.print(writer);
	//return writer.result;
	std::string return_string = "<Display_Client>";
	return_string += "<Identification>" + get_identification() + "</Identification>";
	return_string += "<Last_Ping>" + boost::posix_time::to_iso_string(get_last_ping()) + "</Last_Ping>";
	return_string += "<Display_Playlist_Container>" + get_playlist_container().get()->get_playlist_container_xml() + "</Display_Playlist_Container>";
	return_string += "<Organizations>" + make_list(*get_organizations()) + "</Organizations>";
	return_string += "<Groups>" + make_list(*get_groups()) + "</Groups>";
	return_string += "</Display_Client>";
	return return_string;
}

std::string Display_Client::make_list( std::set<std::string> p_set)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Display_Client::make_list" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	std::string comma_separated_list;
	for (std::set<std::string>::iterator it = p_set.begin();
		it != p_set.end(); ++it)
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Adding " << *it << ", " << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
		comma_separated_list += *it;
		comma_separated_list += ", ";
	}
	comma_separated_list =
		comma_separated_list.substr(0, comma_separated_list.length() - 2);
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Final list: " << comma_separated_list << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	return comma_separated_list;
}

std::set<std::string> Display_Client::make_set( std::string comma_separated_list)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Display_Client::make_set" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	std::set<std::string> set_from_string;
	if (comma_separated_list != "")
	{
		while(comma_separated_list.find(",") != std::string::npos)
		{
			set_from_string.insert(comma_separated_list.substr(0, comma_separated_list.find(",")));
	#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << " ++ " << comma_separated_list.substr(0, comma_separated_list.find(",")) << std::endl;
	#endif // _SHOW_DEBUG_OUTPUT
			comma_separated_list = comma_separated_list.substr(comma_separated_list.find(",") + 1);
		}
	#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " ++ " << comma_separated_list << std::endl;
	#endif // _SHOW_DEBUG_OUTPUT
		set_from_string.insert(comma_separated_list);
	}
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - List empty, returning empty set" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	return set_from_string;
}

void Display_Client::set_playlist_container_name( std::string p_name)
{
	m_playlist_container_name = p_name;
}

std::string Display_Client::get_playlist_container_name()
{
	return m_playlist_container_name;
}

std::string Display_Client::get_groups_string()
{
	return make_list(*get_groups());
}

bool Display_Client::contains_organization(std::string p_organization_name)
{
#ifdef _SHOW_DEBUG_OUTPUT
    std::cout << "  = Display_Client organizations: " << std::endl;
        for(std::set<std::string>::iterator iter = get_organizations()->begin();
        iter != get_organizations()->end(); ++iter)
    {
        std::cout << "   - " << *iter << std::endl;
    }
#endif // _SHOW_DEBUG_OUTPUT
    std::set<std::string>::iterator iter = get_organizations()->find(p_organization_name);
    if(iter != get_organizations()->end())
        return true;
    else
        return false;
}
