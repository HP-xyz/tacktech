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
