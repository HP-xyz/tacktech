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
	m_display_client_container.push_back(display_client);
}

void Display_Client_Container::remove_display_client( int index)
{
	m_display_client_container.erase(m_display_client_container.begin() 
		+ (index - 1));
}
