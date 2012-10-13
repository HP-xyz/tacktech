#include "Group_Playlist_Container_Server.h"


Group_Playlist_Container_Server::Group_Playlist_Container_Server(void)
{
}


Group_Playlist_Container_Server::~Group_Playlist_Container_Server(void)
{
}

bool Group_Playlist_Container_Server::add_organization(
	std::string organization_name,
	Group_Playlist_Container group_playlist )
{
	if (organization_map.find(organization_name) == organization_map.end())
	{
		organization_map.insert(std::pair<std::string, Group_Playlist_Container>
			(organization_name, group_playlist));
		return true;
	}
	else
		return false;
}

Group_Playlist_Container_Server_Map& 
	Group_Playlist_Container_Server::get_organization_map()
{
	return organization_map;
}

std::string Group_Playlist_Container_Server::get_group_playlist_xml()
{
	std::string return_str;
	return_str += "<Tacktech>";
	for (Group_Playlist_Container_Server_Map::iterator it =
		organization_map.begin();
		it !=  organization_map.end();
	it++)
	{
		return_str += "<Group_Playlist_Item Organization_Name=\"";
		return_str += it->first;
		return_str += "\"/>";
		return_str += it->second.get_group_playlist_xml();
		return_str += "</Group_Playlist_Item>";
	}
	return_str += "</Tacktech>";
	return return_str;
}

void Group_Playlist_Container_Server::construct_group_playlist(
	std::string organization_name,
	std::string group_playlist )
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Group_Playlist_Container_Server::construct_group_playlist(STRING)"
		<< std::endl;
#endif // _DEBUG
	pugi::xml_document group_playlist_document;
	group_playlist_document.load(group_playlist.c_str());
	pugi::xml_node root_node = group_playlist_document.child("Group_Playlist");
	for (pugi::xml_node group_playlist_item = root_node.child(
		"Group_Playlist_Item"); group_playlist_item; group_playlist_item =
		group_playlist_item.next_sibling("Group_Playlist_Item"))
	{
		organization_map[organization_name].connect_group_to_playlist(
			group_playlist_item.child_value("Group_Name"),
			group_playlist_item.child_value("Playlist_Name"));
	}
}

void Group_Playlist_Container_Server::construct_group_playlist( 
	const char* group_playlist_filename )
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Group_Playlist_Container_Server::construct_group_playlist(FILE)"
		<< std::endl;
#endif // _DEBUG
	pugi::xml_document group_playlist_document;
	group_playlist_document.load(group_playlist_filename);
	pugi::xml_node root_node = group_playlist_document.child("Tacktech");
	for(pugi::xml_node group_playlist_node = root_node.child("Group_Playlist_Item");
		group_playlist_node;
		group_playlist_node = group_playlist_node.next_sibling("Group_Playlist_Item"))
	{
		std::string organization_name =
			group_playlist_node.attribute("Organization_Name").as_string();
		for (pugi::xml_node group_playlist = group_playlist_node.child("Group_Playlist");
			group_playlist;
			group_playlist = group_playlist.next_sibling("Group_Playlist"))
		{
			for (pugi::xml_node group_playlist_item = root_node.child(
				"Group_Playlist_Item");
				group_playlist_item;
				group_playlist_item =
				group_playlist_item.next_sibling("Group_Playlist_Item"))
			{
				organization_map[organization_name].connect_group_to_playlist(
					group_playlist_item.child_value("Group_Name"),
					group_playlist_item.child_value("Playlist_Name"));
			}
		}
	}
}
