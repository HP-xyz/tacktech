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

Playlist_Container::Playlist_Container( std::string playlist_container_str, std::set<std::string> organization_names)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist_Container::Playlist_Container(STRING)" << std::endl;
	std::cout << " - Creating from str:" << std::endl;
	std::cout << playlist_container_str << std::endl;
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
		std::vector<std::string> group_list = make_vector(playlist_group.child("Groups").attribute("GROUPLIST").as_string());
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << "   - Playlist_Groups: " << make_list(group_list) << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
		add_playlist(playlist, group_list, organization_names);
	}
#ifdef _SHOW_DEBUG_OUTPUT
	print_contents();
#endif // _SHOW_DEBUG_OUTPUT
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

void Playlist_Container::add_playlist( Playlist_Ptr playlist, std::vector<std::string> groups, std::set<std::string> organization_names)
{
	Playlist_Item item(playlist, std::vector<std::string>(organization_names.begin(), organization_names.end()));
	item.first->set_groups(std::set<std::string>(groups.begin(), groups.end()));
	m_playlist_container->insert(item);
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
			upload_xml += make_list(*it->first->get_groups());
			upload_xml += "\"/>";
			upload_xml += "<Playlist_Node>";
			upload_xml += it->first->get_playlist_xml();
			upload_xml += "</Playlist_Node>";
			upload_xml += "</Playlist_Group>";
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
		upload_xml += make_list(*it->first->get_groups());
		upload_xml += "\"/>";
		upload_xml += "<Playlist_Node>";
		upload_xml += it->first->get_playlist_xml();
		upload_xml += "</Playlist_Node>";
		upload_xml += "</Playlist_Group>";
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

bool Playlist_Container::update_playlist( Playlist_Container p_playlist_container, std::string p_organization_name)
{
	bool changed = false;
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist_Container::update_playlist" << std::endl;
	std::cout << " - Playlist_Container size: " << m_playlist_container->size() << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	if (m_playlist_container->size() == 0)
	{
#ifdef _IMPORTANT_OUTPUT
		std::cout << " $ Replacing Playlist_Container" << std::endl;
#endif // _IMPORTANT_OUTPUT
		m_playlist_container.reset(new Container(*p_playlist_container.get_playlist_container()));
		changed = true;
	}
	else
	{
		for (Container::iterator it = p_playlist_container.get_playlist_container()->begin();
			it != p_playlist_container.get_playlist_container()->end(); ++it)
		{
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << " - Checking playlist: " << it->first->get_playlist_name() << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
			std::vector<std::string> organizations_vector = it->second;
			std::vector<std::string>::iterator it2 =
				std::find(organizations_vector.begin(), organizations_vector.end(), p_organization_name);
			if (it2 != organizations_vector.end())
			{
#ifdef _SHOW_DEBUG_OUTPUT
				std::cout << "  - Organization Found" << std::endl;
				std::cout << "  - Looking for playlist '" << it->first->get_playlist_name() 
					<< "' in organization '" << p_organization_name << "'" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
				Playlist_Ptr playlist_ptr = get_playlist_ptr(it->first->get_playlist_name(), p_organization_name);
				if (playlist_ptr.get() == 0)
				{//Playlist_Item not found, therefore adding it
#ifdef _IMPORTANT_OUTPUT
					std::cout << "  + ADDING NEW playlist: '" << it->first->get_playlist_name()
						<< "' to organization: '" << p_organization_name << "'" << std::endl;
#endif // _IMPORTANT_OUTPUT
					add_playlist(it->first, it->second);
					changed = true;
				}
				else
				{//Updating playlist items
#ifdef _IMPORTANT_OUTPUT
					std::cout << "  $ UPDATING playlist: '" << it->first->get_playlist_name()
						<< "' in organization: '" << p_organization_name << "'" << std::endl;
#endif // _IMPORTANT_OUTPUT
					if (playlist_ptr->get_playlist_items()->size()
						== it->first->get_playlist_items()->size())
					{
#ifdef _SHOW_DEBUG_OUTPUT
						std::cout << "  # Playlist sizes are identical" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
						bool playlist_identical = true;
						for (unsigned int i = 0; i < playlist_ptr->get_playlist_items()->size(); ++i)
						{
							if (playlist_ptr->get_playlist_items()->at(i).first
								!= it->first->get_playlist_items()->at(i).first)
							{
								playlist_identical = false;
							}
						}
						if(!playlist_identical)
						{//Playlists are different, replace current with parameter
						 //Playlists are the same size, but we still replace our current
						 //playlist with the one we recieved as a parameter.
#ifdef _IMPORTANT_OUTPUT
							std::cout << "  $ REPLACING playlist: '" << playlist_ptr->get_playlist_name()
								<< "' containing '" << playlist_ptr->get_playlist_items()->size()
								<< "' items with playlist '" << it->first->get_playlist_name()
								<< "' containing '" << it->first->get_playlist_items()->size() << "' items" << std::endl;
#endif // _IMPORTANT_OUTPUT
							playlist_ptr->set_current_item_index(-1);
							playlist_ptr->set_end_time(it->first->get_end_time());
							playlist_ptr->set_groups(*it->first->get_groups());
							playlist_ptr->set_playlist_items(*it->first->get_playlist_items());
							playlist_ptr->set_playlist_name(it->first->get_playlist_name());
							playlist_ptr->set_start_time(it->first->get_start_time());
							changed = true;
						}
					}
					else
					{//Playlists are different, replace current with parameter
					 //Playlists are different sizes
#ifdef _IMPORTANT_OUTPUT
						std::cout << "  $ REPLACING playlist: '" << playlist_ptr->get_playlist_name()
							<< "' containing '" << playlist_ptr->get_playlist_items()->size()
							<< "' items with playlist '" << it->first->get_playlist_name()
							<< "' containing '" << it->first->get_playlist_items()->size() << "' items" << std::endl;
#endif // _IMPORTANT_OUTPUT
						playlist_ptr->set_current_item_index(-1);
						playlist_ptr->set_end_time(it->first->get_end_time());
						playlist_ptr->set_groups(*it->first->get_groups());
						playlist_ptr->set_playlist_items(*it->first->get_playlist_items());
						playlist_ptr->set_playlist_name(it->first->get_playlist_name());
						playlist_ptr->set_start_time(it->first->get_start_time());
						changed = true;
					}
				}
			}
			else
			{
#ifdef _SHOW_DEBUG_OUTPUT
				std::cout << "  - Organization NOT Found" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
			}
		}
	}
	return changed;
}

void Playlist_Container::update_playlist_on_server( Playlist_Container p_playlist_container, std::string p_organization_name )
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist_Container::update_playlist_on_server" << std::endl;
	std::cout << " - Playlist_Container size: " << m_playlist_container->size() << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	if (m_playlist_container->size() > 0)
	{
		for (Container::iterator it = p_playlist_container.get_playlist_container()->begin();
			it != p_playlist_container.get_playlist_container()->end(); ++it)
		{
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << " - Checking playlist: " << it->first->get_playlist_name() << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
			std::vector<std::string> organizations_vector = it->second;
			std::vector<std::string>::iterator it2 =
				std::find(organizations_vector.begin(), organizations_vector.end(), p_organization_name);
			if (it2 != organizations_vector.end())
			{
#ifdef _SHOW_DEBUG_OUTPUT
				std::cout << "  - Organization Found" << std::endl;
				std::cout << "  - Looking for playlist '" << it->first->get_playlist_name() 
					<< "' in organization '" << p_organization_name << "'" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
				Playlist_Ptr playlist_ptr = get_playlist_ptr(it->first->get_playlist_name(), p_organization_name);
				if (playlist_ptr.get() != 0)
				{//Updating playlist items
#ifdef _SHOW_DEBUG_OUTPUT
					std::cout << "  - Setting current_item index to: " << it->first->get_current_item_index() << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
					playlist_ptr->set_current_item_index(it->first->get_current_item_index());
					playlist_ptr->currently_active = it->first->currently_active;
				}
			}
			else
			{
#ifdef _SHOW_DEBUG_OUTPUT
				std::cout << "  - Organization NOT Found" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
			}
		}
	}
}


Playlist_Ptr Playlist_Container::get_playlist_ptr( std::string p_playlist_name, std::string p_organization_name)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist_Container::get_playlist_ptr()" << std::endl;
#endif //_SHOW_DEBUG_OUTPUT
	boost::shared_ptr<Playlist> temp_playlist;
	for (Container::iterator it = m_playlist_container->begin();
		it != m_playlist_container->end(); ++it)
	{
		if (it->first->get_playlist_name() == p_playlist_name)
		{
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << " - " << p_playlist_name << " found" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
			for (unsigned int i = 0; i < it->second.size(); ++i)
			{
				if (it->second[i] == p_organization_name)
				{
#ifdef _SHOW_DEBUG_OUTPUT
					std::cout << " - Returning found playlist" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
					return it->first;
				}
			}
		}
	}
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Returning NULL playlist" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
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
					it->first->get_current_item_index() - 1).first;
			}
		}
		return "NOTHING CURRENTLY PLAYING";
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

//TODO
//IF MORE THAN 1 PLAYLIST MATCHES THE TIME TO PLAY, WHAT SHOULD HAPPEN?
std::pair<std::string,int> Playlist_Container::get_next_item()
{
    try
    {
        std::string current_time_str = boost::posix_time::to_iso_string(boost::posix_time::second_clock::universal_time());
        current_time_str = current_time_str.substr(current_time_str.find("T") + 1);
#ifdef _SHOW_DEBUG_OUTPUT
        std::cout << "[" << current_time_str.substr(0, 2) << ":" << current_time_str.substr(2,2) << ":" << current_time_str.substr(4) << "]" << " => ";
#endif // _SHOW_DEBUG_OUTPUT
        boost::posix_time::time_duration current_time(
            boost::lexical_cast<int,std::string>(current_time_str.substr(0, 2)),
            boost::lexical_cast<int,std::string>(current_time_str.substr(2, 2)),
            boost::lexical_cast<int,std::string>(current_time_str.substr(4)),
            0);
        for(Container::iterator it = get_playlist_container()->begin();
            it != get_playlist_container()->end(); ++it)
        {
            if(it->first->get_playlist_items()->size() > 0)
            {
    #ifdef _SHOW_DEBUG_OUTPUT
                std::cout << "INDEX: " << it->first->get_current_item_index() << " of " << it->first->get_playlist_items()->size() << " <-> ";
    #endif // _SHOW_DEBUG_OUTPUT
                if((it->first->get_start_time() < current_time &&
                    it ->first->get_end_time() > current_time) ||
                    (it->first->get_start_time() == ALL_DAY_TIME_DURATION &&
                    it->first->get_end_time() == ALL_DAY_TIME_DURATION))
                {
                    if (it->first->get_current_item_index() == -1)
                    {//First time the playlist is being played
                        it->first->currently_active = true;
                        it->first->set_current_item_index(1);
    #ifdef _SHOW_DEBUG_OUTPUT
                        std::cout << " '" << it->first->get_playlist_items()->at(0).first << "' ";
    #endif // _SHOW_DEBUG_OUTPUT
                        return it->first->get_playlist_items()->at(0);
                    }
                    else if(it->first->get_current_item_index() >= (it->first->get_playlist_items()->size() - 1) )
                    {//Looping playlist
                        it->first->currently_active = true;
                        int index = it->first->get_current_item_index();
                        it->first->set_current_item_index(0);
    #ifdef _SHOW_DEBUG_OUTPUT
                        std::cout << " '" << it->first->get_playlist_items()->at(index).first << "' ";
    #endif // _SHOW_DEBUG_OUTPUT
                        return it->first->get_playlist_items()->at(index);
                    }
                    else
                    {//Playing next item normally
                        it->first->currently_active = true;
                        it->first->set_current_item_index(it->first->get_current_item_index() + 1);
    #ifdef _SHOW_DEBUG_OUTPUT
                        std::cout << " '" << it->first->get_playlist_items()->at(it->first->get_current_item_index() - 1).first << "' ";
    #endif // _SHOW_DEBUG_OUTPUT
                        return it->first->get_playlist_items()->at(it->first->get_current_item_index() - 1);
                    }
                }
                else
                {//Making sure that the playlist is set to not be currently active
                    it->first->currently_active = false;
                    it->first->set_current_item_index(-1);
                }
            }
            else
            {
#ifdef _IMPORTANT_OUTPUT
                std::cout << "WARNING: Playlist exists but has no items" << std::endl;
#endif // _IMPORTANT_OUTPUT
            }
        }
    }
    catch(std::exception &e)
    {
        std::cerr << "CRITICAL (Playlist_Container::get_next_item()): " << e.what() << std::endl;
    }
    return std::pair<std::string,int>("NO ITEMS", 0);
}


std::vector<std::string> Playlist_Container::get_needed_items( std::vector<std::string> dislpay_files)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist_Container::get_needed_items()" << std::endl;
	print_contents();
#endif // _SHOW_DEBUG_OUTPUT
	std::vector<std::string> needed_items;
	for (Container::iterator it = get_playlist_container()->begin();
		it != get_playlist_container()->end(); ++it)
	{//Iterating through playlists
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Playlist: " << it->first->get_playlist_name() << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
		for (std::vector< std::pair<std::string,int> >::iterator it2 = it->first->get_playlist_items()->begin();
			it2 != it->first->get_playlist_items()->end(); ++it2)
		{//Iterating through playlist items
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << "  - Playlist_Item: " << it2->first << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
			std::vector<std::string>::iterator file_found =
				std::find(dislpay_files.begin(), dislpay_files.end(), it2->first);
			if (file_found != dislpay_files.end())
			{//File needs to be uploaded
#ifdef _SHOW_DEBUG_OUTPUT
				std::cout << "   - ADDING: " << it2->first << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
				needed_items.push_back(it2->first);
			}
		}
	}
	return needed_items;
}


void Playlist_Container::remove_playlist( std::string p_playlist_name)
{
	for (Container::iterator it = get_playlist_container()->begin();
		it != get_playlist_container()->end();)
	{
		if (it->first->get_playlist_name() == p_playlist_name)
		{//We need to remove this playlist
			get_playlist_container()->erase(it++);
		}
	}
}

void Playlist_Container::remove_playlist_item( std::string playlist_name, std::string playlist_item_name)
{
	for (Container::iterator it = get_playlist_container()->begin();
		it != get_playlist_container()->end(); ++it)
	{
		if (it->first->get_playlist_name() == playlist_name)
		{//We need to remove an item in this playlist
			it->first->remove_item(playlist_item_name);
		}
	}
}

void Playlist_Container::remove_playlist_of_group( std::string group_name)
{
	for (Container::iterator it = get_playlist_container()->begin(); it != get_playlist_container()->end(); )
	{
		if (it->first->contains_group(group_name))
		{
			get_playlist_container()->erase(it++);
		}
		else
		{
			++it;
		}
	}
}

#ifdef _SHOW_DEBUG_OUTPUT
void Playlist_Container::print_contents()
{
	std::cout << "=Playlist_Container::print_contents" << std::endl;
	for (Container::iterator it = m_playlist_container->begin();
		it != m_playlist_container->end(); ++it)
	{
		it->first->print_contents();
	}
}
#endif // _SHOW_DEBUG_OUTPUT
