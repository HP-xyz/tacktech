#include "Playlist_Container_Server.h"


Playlist_Container_Server::Playlist_Container_Server(void)
{
}


Playlist_Container_Server::~Playlist_Container_Server(void)
{
}

bool Playlist_Container_Server::add_organization(
	std::string organization_name, Playlist_Container playlist)
{
	if (organization_map.find(organization_name) == organization_map.end())
	{
		organization_map.insert(std::pair<std::string, Playlist_Container>
			(organization_name, playlist));
		return true;
	}
	else
		return false;
}

Playlist_Container_Server_Map& Playlist_Container_Server::get_organization_map()
{
	return organization_map;
}

std::string Playlist_Container_Server::get_playlists_xml()
{
	std::string return_str;
	return_str += "<Tacktech>";
	for (Playlist_Container_Server_Map::iterator it =
		organization_map.begin();
		it !=  organization_map.end();
		it++)
	{
		return_str += "<Playlist_Container_Item Organization_Name=\"";
		return_str += it->first;
		return_str += "\"/>";
		return_str += it->second.get_playlists_xml();
		return_str += "</Playlist_Container_Item>";
	}
	return_str += "</Tacktech>";
	return return_str;
}

void Playlist_Container_Server::construct_playlist(
	std::string organization_name, std::string playlist )
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist_Container_Server::construct_playlist(STRING)"
		<< std::endl;
	int items_added = 0;
#endif // _DEBUG
	pugi::xml_document playlist_document;
	playlist_document.load(playlist.c_str());
	pugi::xml_node root_node = playlist_document.child("Playlist");
	for (pugi::xml_node playlist_item = root_node.child("Playlist_Item");
		playlist_item;
		playlist_item = playlist_item.next_sibling("Playlist_Item"))
	{
		for (pugi::xml_node item_node = playlist_item.child("Item");
			item_node;
			item_node = item_node.next_sibling("Item"))
		{
			
			if(organization_map[organization_name].add_filename(
				playlist_item.attribute("Playlist_Name").as_string(),
				item_node.child_value("Filename"),
				boost::lexical_cast<int>(item_node.child_value("Pause"))))
#ifdef _SHOW_DEBUG_OUTPUT
				items_added += 1;
#endif // _SHOW_DEBUG_OUTPUT
		}
	}
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Added " << items_added << " new items" << std::endl;
#endif // _DEBUG
}

void Playlist_Container_Server::construct_playlist( 
	const char* playlist_filename )
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Playlist_Container_Server::construct_playlist(FILE)"
		<< std::endl;
#endif // _DEBUG
	/* Read groups_and_computers from file */
	pugi::xml_document playlist_document;
	playlist_document.load_file(playlist_filename);
	pugi::xml_node root_node = playlist_document.child("Tacktech");
	for (pugi::xml_node organization_node =
		root_node.child("Group_Container_Item");
		organization_node;
		organization_node = organization_node.next_sibling("Group_Container_Item"))
	{
		std::string organization_name =
			organization_node.attribute("Organization_Name").as_string();
		pugi::xml_node playlist_node = organization_node.child("Playlist");
		for (pugi::xml_node playlist_item = playlist_node.child("Playlist_Item");
			playlist_item;
			playlist_item = playlist_item.next_sibling("Playlist_Item"))
		{
			for (pugi::xml_node item_node = playlist_item.child("Item");
				item_node;
				item_node = item_node.next_sibling("Item"))
			{
				organization_map[organization_name].add_filename(
					playlist_item.attribute("Playlist_Name").as_string(),
					item_node.child_value("Filename"),
					boost::lexical_cast<int>(item_node.child_value("Pause")));
			}
		}
	}
}
