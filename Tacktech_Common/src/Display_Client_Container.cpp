#include "Display_Client_Container.h"


Display_Client_Container::Display_Client_Container(void)
{
}


Display_Client_Container::~Display_Client_Container(void)
{
}

std::vector<Display_Client_Ptr> Display_Client_Container::get_display_client_container()
{
	return m_display_client_container;
}

void Display_Client_Container::add_display_client( Display_Client_Ptr display_client)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Display_Client_Container::add_display_client" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	std::vector<Display_Client_Ptr>::iterator it =
		find_display_client_by_ident(display_client->get_identification());
	if (it != m_display_client_container.end())
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " $$ Updating: "<< display_client->get_identification() << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
		it->get()->set_last_ping(boost::posix_time::second_clock::universal_time());
	}
	else
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " ++ Adding: "<< display_client->get_identification() << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
		m_display_client_container.push_back(display_client);
	}
}

void Display_Client_Container::remove_display_client( int index)
{
	m_display_client_container.erase(m_display_client_container.begin() 
		+ (index - 1));
}

std::vector<Display_Client_Ptr>::iterator 
	Display_Client_Container::find_display_client_by_ident( std::string ident)
{
	for (std::vector<Display_Client_Ptr>::iterator it = m_display_client_container.begin();
		it != m_display_client_container.end(); ++it)
	{
		if(it->get()->get_identification() == ident)
		{
			return it;
		}
	}
	return m_display_client_container.end();
}

std::string Display_Client_Container::get_display_client_container_xml( std::string p_organization_name)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Display_Client_Container::get_display_client_container_xml"<< std::endl;
	std::cout << " - For organization: " << p_organization_name << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	std::string upload_xml;
	upload_xml += "<Tacktech>";
	upload_xml += "<Display_Client_Container>";
	for (std::vector<Display_Client_Ptr>::iterator it = get_display_client_container().begin();
		it != get_display_client_container().end(); ++it)
	{
		std::set<std::string>::iterator it2 =
			it->get()->get_organizations().find(p_organization_name);
		if(it2 != it->get()->get_organizations().end())
		{
			upload_xml += "<Display_Client_Item>";
			upload_xml += it->get()->get_display_client_xml();
			upload_xml += "</Display_Client_Item>";
		}
	}
	upload_xml += "</Display_Client_Container>";
	upload_xml += "</Tacktech>";
	return upload_xml;
}
