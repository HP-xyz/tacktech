#include "Display_Client.h"


Display_Client::Display_Client(void)
{
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