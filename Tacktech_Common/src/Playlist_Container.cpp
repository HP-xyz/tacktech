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
	std::string upload_xml;
	upload_xml += "<Tacktech>";
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
	upload_xml += "</Tacktech>";
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
