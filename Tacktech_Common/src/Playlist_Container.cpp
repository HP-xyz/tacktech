#include "Playlist_Container.h"

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

Playlist_Container::Playlist_Container()
{
}

Playlist_Container::Playlist_Container( std::string playlist_container_str)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist_Container(STRING)" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	pugi::xml_document playlist_container_doc;
	playlist_container_doc.load(playlist_container_str.c_str());
	pugi::xml_node root_node = playlist_container_doc.child("Playlist_Container");
	for (pugi::xml_node playlist_group = root_node.child("Playlist_Group");
		playlist_group; playlist_group.next_sibling("Playlist_Group"))
	{
		xml_string_writer writer;
		playlist_group.child("Playlist_Node").print(writer);
		Playlist_Ptr playlist(new Playlist(writer.result));
		add_playlist(playlist, make_vector(playlist_group.attribute("GROUPLIST").as_string()));
	}
}

Playlist_Container::~Playlist_Container()
{

}

Container Playlist_Container::get_playlist_container( std::string group_name)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist_Container::get_playlist_container()" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	Container return_container;
	for(Container::iterator it = m_playlist_container.begin();
		it!= m_playlist_container.end();
		++it)
	{
		std::vector<std::string>::const_iterator it2 =
			std::find(it->second.begin(), it->second.end(), group_name);
		if (it2 != it->second.end())
		{//Found group_name in list, adding to return_container
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << " - Adding: "<< it->first->get_playlist_name() << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
			return_container.insert(Playlist_Item(it->first, it->second));
		}
	}
	return return_container;
}

void Playlist_Container::add_playlist( Playlist_Ptr playlist, std::vector<std::string> groups)
{
	m_playlist_container.insert(Playlist_Item(playlist, groups));
}

std::string Playlist_Container::get_playlist_container_xml( std::string group_name)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist_Container::get_playlist_container_xml" << std::endl;
	std::cout << " - For organization: " << group_name << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	std::string upload_xml;
	upload_xml += "<Playlist_Container>";
	for (Container::iterator it = m_playlist_container.begin();
		it != m_playlist_container.end(); ++it)
	{
		std::vector<std::string>::const_iterator it2 =
			std::find(it->second.begin(), it->second.end(), group_name);
		if(it2 != it->second.end())
		{
			upload_xml += "<Playlist_Group>";
			upload_xml += "<Groups GROUPLIST=\"";
			for (unsigned int i = 0; i < it->second.size(); ++i)
			{
				if (i != (it->second.size() -1))
				{
					upload_xml += it->second[i];
					upload_xml += ",";
				}
				else
				{
					upload_xml += it->second[i];
				}
			}
			upload_xml += "\"/>";
			upload_xml += "<Playlist_Node>";
			upload_xml += it->first->get_playlist_xml();
			upload_xml += "</Playlist_Node>";
			upload_xml += "</Playlist_Group";
		}
	}	
	upload_xml += "</Playlist_Container>";
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Sending: " << std::endl;
	std::cout << upload_xml << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	return upload_xml;
}

void Playlist_Container::set_playlist_container_name( std::string p_playlist_container_name)
{
	m_playlist_container_name = p_playlist_container_name;
}

std::string Playlist_Container::get_playlist_container_name()
{
	return m_playlist_container_name;
}

std::vector<std::string> Playlist_Container::make_vector( std::string comma_separated_list)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist_Container::make_vector" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	std::vector<std::string> vector_from_string;
	while(comma_separated_list.find(",") != std::string::npos)
	{
		vector_from_string.push_back(comma_separated_list.substr(0, comma_separated_list.find(",")));
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " ++ " << comma_separated_list.substr(0, comma_separated_list.find(",")) << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
		comma_separated_list = comma_separated_list.substr(comma_separated_list.find(",") + 1);
	}
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " ++ " << comma_separated_list << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	vector_from_string.push_back(comma_separated_list);
	return vector_from_string;
}

#ifdef _SHOW_DEBUG_OUTPUT
void Playlist_Container::print_contents()
{
	std::cout << "=Playlist_Container::print_contents" << std::endl;
	for (Container::iterator it = m_playlist_container.begin();
		it != m_playlist_container.end(); ++it)
	{
		std::cout << " - Playlist Name: " << it->first->get_playlist_name()
			<< ", Pause: " << make_list(it->second) << std::endl;
	}
}
#endif // _SHOW_DEBUG_OUTPUT

std::string Playlist_Container::make_list( std::vector<std::string> p_vector)
{
	std::string comma_separated_list;
	for (std::vector<std::string>::iterator it = p_vector.begin();
		it != p_vector.end(); ++it)
	{
		comma_separated_list += *it;
		comma_separated_list += ", ";
	}
	comma_separated_list =
		comma_separated_list.substr(0, comma_separated_list.length() - 2);
	return comma_separated_list;
}