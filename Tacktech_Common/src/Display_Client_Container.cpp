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
	std::cout << " - String: " << display_client_container_str << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	m_display_client_container.reset(new std::vector<Display_Client_Ptr>);
	pugi::xml_document display_client_container_doc;
	display_client_container_doc.load(display_client_container_str.c_str());
	pugi::xml_node root_node =
		display_client_container_doc.child("Display_Client_Container");
	for (pugi::xml_node display_item_node = root_node.child("Display_Client_Item");
		display_item_node;
		display_item_node = display_item_node.next_sibling("Display_Client_Item"))
	{
		xml_string_writer writer;
		display_item_node.print(writer);
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Sending to Display_Client"<< writer.result << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
		Display_Client_Ptr display_client(new Display_Client(writer.result));
		add_display_client(display_client);
	}
}

Display_Client_Container::~Display_Client_Container(void)
{
}

boost::shared_ptr<std::vector<Display_Client_Ptr> > Display_Client_Container::get_display_client_container()
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
	if (it != m_display_client_container->end())
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
		m_display_client_container->push_back(display_client);
	}
}

void Display_Client_Container::remove_display_client( int index)
{
	m_display_client_container->erase(m_display_client_container->begin()
		+ (index - 1));
}

std::vector<Display_Client_Ptr>::iterator
	Display_Client_Container::find_display_client_by_ident( std::string ident)
{
	for (std::vector<Display_Client_Ptr>::iterator it = m_display_client_container->begin();
		it != m_display_client_container->end(); ++it)
	{
		if(it->get()->get_identification() == ident)
		{
			return it;
		}
	}
	return m_display_client_container->end();
}

std::string Display_Client_Container::get_display_client_container_xml( std::string p_organization_name)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Display_Client_Container::get_display_client_container_xml"<< std::endl;
	std::cout << " - For organization: " << p_organization_name << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	std::string upload_xml;
	upload_xml += "<Display_Client_Container>";
	for (unsigned int i = 0; i < get_display_client_container()->size(); ++i)
	{

		std::set<std::string>::iterator it2 =
			get_display_client_container()->at(i)
			->get_organizations().find(p_organization_name);
		if(it2 != get_display_client_container()
			->at(i)->get_organizations().end())
		{
			upload_xml += "<Display_Client_Item>";
			upload_xml += get_display_client_container()
				->at(i)->get_display_client_xml();
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
	for (unsigned int i = 0; i < get_display_client_container()->size(); ++i)
	{
		std::cout << " -- Display_Name: "
			<< get_display_client_container()->at(i)->get_identification()
			<< std::endl;
	}
}
#endif // _SHOW_DEBUG_OUTPUT


std::vector<std::string> Display_Client_Container::get_unique_group_names()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Display_Client_Container::get_unique_group_names()" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	std::vector<std::string> unique_groups;
	for (unsigned int i = 0; i < m_display_client_container->size(); ++i)
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Checking Display_Client: " << m_display_client_container->at(i)->get_identification() << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
		Display_Client_Ptr display_client = m_display_client_container->at(i);
		std::set<std::string> group_set = *display_client->get_groups();
		for (std::set<std::string>::iterator iter = group_set.begin(); iter != group_set.end(); ++iter)
		{
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << "  - Checking Group: " << *iter << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
			std::string group_name = *iter;
			std::vector<std::string>::iterator it2 = std::find(unique_groups.begin(), unique_groups.end(), group_name);
			if (it2 != unique_groups.end())
			{
#ifdef _SHOW_DEBUG_OUTPUT
				std::cout << "    + " << group_name << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
				unique_groups.push_back(group_name);
			}
		}
	}
	return unique_groups;
}

void Display_Client_Container::update_groups_and_playlist(Display_Client_Container p_display_client_container)
{
    for(std::vector<Display_Client_Ptr>::iterator it
        = p_display_client_container.get_display_client_container()->begin();
        it != p_display_client_container.get_display_client_container()->end();
        ++it)
    {
        std::vector<Display_Client_Ptr>::iterator it2 = find_display_client_by_ident(it->get()->get_identification());
        if(it2 != p_display_client_container.get_display_client_container()->end())
        {
            it2->get()->set_groups(*it->get()->get_groups());
            it2->get()->set_playlist_container(it->get()->get_playlist_container());
        }
    }
}
