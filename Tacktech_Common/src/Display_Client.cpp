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
	m_playlist_container.reset(new Playlist_Container());
	m_playlist_container->set_playlist_container_name("NONE");
}

Display_Client::Display_Client( std::string display_client_str)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Display_Client(STRING)" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	m_playlist_container.reset(new Playlist_Container());
	pugi::xml_document disply_client_document;
	disply_client_document.load(display_client_str.c_str());
	pugi::xml_node root_node = disply_client_document.child("Display_Client_Item").child("Display_Client");
	set_identification(root_node.attribute("Identification").as_string());
	set_last_ping(boost::posix_time::from_iso_string(root_node.attribute("Last_Ping").as_string()));
	set_groups(make_set(root_node.attribute("Groups").as_string()));
	set_organizations(make_set(root_node.attribute("Organizations").as_string()));
	set_playlist_container_name(root_node.attribute("Playlist_Container").as_string());
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
std::set<std::string> Display_Client::get_groups()
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
	m_groups = p_groups;
}
void Display_Client::set_organizations( std::set<std::string> p_organizations)
{
	m_organizations = p_organizations;
}
void Display_Client::set_playlist_container(Playlist_Container_Ptr p_playlist_container)
{
	m_playlist_container = p_playlist_container;
}

bool Display_Client::add_group(std::string p_group_name)
{
	std::set<std::string>::iterator it = get_groups().find(p_group_name);
	if(it != get_groups().end())
	{
		return false;
	}
	else
	{
		m_groups.insert(p_group_name);
		return true;
	}
}

std::set<std::string> Display_Client::get_organizations()
{
	return m_organizations;
}

bool Display_Client::add_organization( std::string p_organization_name)
{
	std::set<std::string>::iterator it =
		get_organizations().find(p_organization_name);
	if(it != get_organizations().end())
	{
		return false;
	}
	else
	{
		m_organizations.insert(p_organization_name);
		return true;
	}
}

std::string Display_Client::get_display_client_xml()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Display_Client::get_display_client_xml" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	pugi::xml_document disply_client_document;
	pugi::xml_node display_client_root 
		= disply_client_document.append_child("Display_Client");
	display_client_root.append_attribute("Identification") =
		get_identification().c_str();
	display_client_root.append_attribute("Last_Ping") =
		boost::posix_time::to_iso_string(get_last_ping()).c_str();
	display_client_root.append_attribute("Playlist_Container") =
		get_playlist_container()->get_playlist_container_name().c_str();
	display_client_root.append_attribute("Organizations") =
		make_list(get_organizations()).c_str();
	display_client_root.append_attribute("Groups") =
		make_list(get_groups()).c_str();
	xml_string_writer writer;
	disply_client_document.print(writer);
	return writer.result;
}

std::string Display_Client::make_list( std::set<std::string> p_set)
{
	std::string comma_separated_list;
	for (std::set<std::string>::iterator it = p_set.begin();
		it != p_set.end(); ++it)
	{
		comma_separated_list += *it;
		comma_separated_list += ", ";
	}
	comma_separated_list =
		comma_separated_list.substr(0, comma_separated_list.length() - 2);
	return comma_separated_list;
}

std::set<std::string> Display_Client::make_set( std::string comma_separated_list)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Display_Client::make_set" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	std::set<std::string> set_from_string;
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
	return make_list(get_groups());
}
