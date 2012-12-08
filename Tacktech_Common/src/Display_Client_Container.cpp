#include "Display_Client_Container.h"

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

Display_Client_Container::Display_Client_Container(void)
{
}

Display_Client_Container::Display_Client_Container( std::string display_client_container_str)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Display_Client_Container(STRING)"<< std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	pugi::xml_document display_client_container_doc;
	display_client_container_doc.load(display_client_container_str.c_str());
	pugi::xml_node root_node =
		display_client_container_doc.child("Dislpay_Client_Container");
	for (pugi::xml_node display_item_node = root_node.child("Display_Client_Item");
		display_item_node; display_item_node.next_sibling("Display_Client_Item"))
	{
		xml_string_writer writer;
		display_item_node.print(writer);
		Display_Client_Ptr display_client(new Display_Client(writer.result));
	}
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
	upload_xml += "<Display_Client_Container>";
	for (unsigned int i = 0; i < get_display_client_container().size(); ++i)
	{

		std::set<std::string>::iterator it2 =
			get_display_client_container()
			[i]->get_organizations().find(p_organization_name);
		if(it2 != get_display_client_container()
			[i]->get_organizations().end())
		{
			upload_xml += "<Display_Client_Item>";
			upload_xml += get_display_client_container()
				[i]->get_display_client_xml();
			upload_xml += "</Display_Client_Item>";
		}
	}
	upload_xml += "</Display_Client_Container>";
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Sending: " << std::endl;
	std::cout << upload_xml << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	return upload_xml;
}
#ifdef _SHOW_DEBUG_OUTPUT
void Display_Client_Container::print_contents()
{
	std::cout << "=Display_Client_Container::print_contents" << std::endl;
	for (unsigned int i = 0; i < get_display_client_container().size(); ++i)
	{
		std::cout << " -- Display_Name: " 
			<< get_display_client_container()[i]->get_identification() 
			<< std::endl;
	}
}
#endif // _SHOW_DEBUG_OUTPUT



