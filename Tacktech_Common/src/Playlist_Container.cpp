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

Playlist_Container::Playlist_Container()
{
	m_playlist_container.reset(new Container());
}

Playlist_Container::Playlist_Container( std::string playlist_container_str)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist_Container::Playlist_Container(STRING)" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	m_playlist_container.reset(new Container());
	pugi::xml_document playlist_container_doc;
	playlist_container_doc.load(playlist_container_str.c_str());
	pugi::xml_node root_node = playlist_container_doc.child("Playlist_Container");
	for (pugi::xml_node playlist_group = root_node.child("Playlist_Group");
		playlist_group; playlist_group = playlist_group.next_sibling("Playlist_Group"))
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Creating Playlist" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
		xml_string_writer writer;
		playlist_group.child("Playlist_Node").print(writer);
		Playlist_Ptr playlist(new Playlist(writer.result));
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " -- Adding Playlist" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
		add_playlist(playlist, make_vector(playlist_group.attribute("GROUPLIST").as_string()));
	}
}

Playlist_Container::~Playlist_Container()
{

}

Container Playlist_Container::get_playlist_container( std::string organization_name)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist_Container::get_playlist_container()" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	Container return_container;
	for(Container::iterator it = m_playlist_container->begin();
		it!= m_playlist_container->end();
		++it)
	{
		std::vector<std::string>::const_iterator it2 =
			std::find(it->second.begin(), it->second.end(), organization_name);
		if (it2 != it->second.end())
		{//Found organization_name in list, adding to return_container
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << " - Adding: "<< it->first->get_playlist_name() << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
			return_container.insert(Playlist_Item(it->first, it->second));
		}
	}
	return return_container;
}

Playlist_Container Playlist_Container::get_playlist_container( std::string organization_name, std::string group_name)
{
	Playlist_Container return_playlist;
	/** First we get only the Playlists that can be used by the organization_name */
	Container organization_container = get_playlist_container(organization_name);
	/* Then we find only the playlists that can be used by the group_name */
	for(Container::iterator it = organization_container.begin();
		it!= organization_container.end();
		++it)
	{
		if (it->first->contains_group(group_name))
		{
			return_playlist.add_playlist(it->first, it->second);
		}
	}
	return return_playlist;
}

boost::shared_ptr<Container> Playlist_Container::get_playlist_container()
{
	return m_playlist_container;
}

void Playlist_Container::add_playlist( Playlist_Ptr playlist, std::vector<std::string> organizations)
{
	m_playlist_container->insert(Playlist_Item(playlist, organizations));
}

std::string Playlist_Container::get_playlist_container_xml( std::string group_name)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist_Container::get_playlist_container_xml" << std::endl;
	std::cout << " - For organization: " << group_name << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	std::string upload_xml;
	upload_xml += "<Playlist_Container>";
	for (Container::iterator it = m_playlist_container->begin();
		it != m_playlist_container->end(); ++it)
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

std::string Playlist_Container::get_playlist_container_xml()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist_Container::get_playlist_container_xml" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	std::string upload_xml;
	upload_xml += "<Playlist_Container>";
	for (Container::iterator it = m_playlist_container->begin();
		it != m_playlist_container->end(); ++it)
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

void Playlist_Container::update_playlist( Playlist_Container p_playlist_container, std::string p_organization_name)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist_Container::update_playlist" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	for (Container::iterator it = m_playlist_container->begin(); 
		it != m_playlist_container->end(); ++it)
	{
		std::vector<std::string> organizations_vector = it->second;
		std::vector<std::string>::iterator it2 =
			std::find(organizations_vector.begin(), organizations_vector.end(), p_organization_name);
		if (it2 != organizations_vector.end())
		{
			Playlist_Ptr playlist_ptr = get_playlist_ptr(it->first->get_playlist_name(), p_organization_name);
			if (playlist_ptr != 0)
			{//Playlist_Item not found, therefore adding it
	#ifdef _IMPORTANT_OUTPUT
				std::cout << " + ADDING NEW playlist: '" << it->first->get_playlist_name() 
					<< "' to organization: '" << p_organization_name << "'" << std::endl;
	#endif // _IMPORTANT_OUTPUT
				add_playlist(it->first, it->second);
			}
			else
			{//Updating playlist items
	#ifdef _IMPORTANT_OUTPUT
				std::cout << " $ UPDATING playlist: '" << it->first->get_playlist_name() 
					<< "' to organization: '" << p_organization_name << "'" << std::endl;
	#endif // _IMPORTANT_OUTPUT
				playlist_ptr->set_current_item_index(it->first->get_current_item_index());
				playlist_ptr->set_end_time(it->first->get_end_time());
				playlist_ptr->set_groups(*it->first->get_groups());
				playlist_ptr->set_playlist_items(*it->first->get_playlist_items());
				playlist_ptr->set_playlist_name(it->first->get_playlist_name());
				playlist_ptr->set_start_time(it->first->get_start_time());
			}
		}
	}
}

Playlist_Ptr Playlist_Container::get_playlist_ptr( std::string p_playlist_name, std::string p_organization_name)
{
	boost::shared_ptr<Playlist> temp_playlist;
	for (Container::iterator it = m_playlist_container->begin(); 
		it != m_playlist_container->end(); ++it)
	{
		if (it->first->get_playlist_name() == p_playlist_name)
		{
			for (unsigned int i = 0; i < it->second.size(); ++i)
			{
				if (it->second[i] == p_organization_name)
				{
					return it->first;
				}
			}
		}
	}
	return temp_playlist;
}

std::string Playlist_Container::get_current_playing_item()
{
	if (get_playlist_container()->size() == 0)
	{
		return "PLAYLIST EMPTY";
	}
	else
	{
		for (Container::iterator it = get_playlist_container()->begin();
			it != get_playlist_container()->end(); ++it)
		{
			//std::string current_time_str = boost::posix_time::to_iso_string(boost::posix_time::second_clock::universal_time());
			//boost::posix_time::time_duration current_time(
			//	boost::lexical_cast<int,std::string>(current_time_str.substr(0, 2)),
			//	boost::lexical_cast<int,std::string>(current_time_str.substr(2, 2)),
			//	boost::lexical_cast<int,std::string>(current_time_str.substr(4)),
			//	0);
			///* Get time_durations so we can calculate if the current playlist should,
			//   be playing now */
			//std::string start_time_str = boost::posix_time::to_iso_string(it->first->get_start_time());
			//std::string end_time_str = boost::posix_time::to_iso_string(it->first->get_end_time());
			//boost::posix_time::time_duration start_time(
			//	boost::lexical_cast<int,std::string>(start_time_str.substr(0, 2)),
			//	boost::lexical_cast<int,std::string>(start_time_str.substr(2, 2)),
			//	boost::lexical_cast<int,std::string>(start_time_str.substr(4)),
			//	0);
			//boost::posix_time::time_duration end_time(
			//	boost::lexical_cast<int,std::string>(end_time_str.substr(0, 2)),
			//	boost::lexical_cast<int,std::string>(end_time_str.substr(2, 2)),
			//	boost::lexical_cast<int,std::string>(end_time_str.substr(4)),
			//	0);
			//if (current_time > start_time && current_time < end_time)
			//{
			//}
			if (it->first->currently_active)
			{
				if (it->first->get_current_item_index() == -1)
					return "NONE PLAYING";
				else
					return it->first->get_playlist_items()->at(
					it->first->get_current_item_index()).first;
			}
		}
		return "COULD NOT GET PLAYLIST ITEM";
	}
}

boost::shared_ptr<std::set<Playlist_Ptr> > Playlist_Container::get_playlists_of_group( std::string group_name)
{
	boost::shared_ptr<std::set<Playlist_Ptr> > return_set(new std::set<Playlist_Ptr>);
	for (Container::iterator it = get_playlist_container()->begin();
		it != get_playlist_container()->end(); ++it)
	{
		if (it->first->contains_group(group_name))
		{
			return_set->insert(it->first);
		}
	}
	return return_set;
}

#ifdef _SHOW_DEBUG_OUTPUT
void Playlist_Container::print_contents()
{
	std::cout << "=Playlist_Container::print_contents" << std::endl;
	for (Container::iterator it = m_playlist_container->begin();
		it != m_playlist_container->end(); ++it)
	{
		std::cout << " - Playlist Name: " << it->first->get_playlist_name()
			<< ", Playlist Organizations: " << make_list(it->second)
			<< ", Playlist Groups:" << make_list(*it->first->get_groups()) << std::endl;
	}
}
#endif // _SHOW_DEBUG_OUTPUT
