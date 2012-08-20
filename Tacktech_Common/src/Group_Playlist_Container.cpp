#include "Group_Playlist_Container.h"

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

Group_Playlist_Container::Group_Playlist_Container()
{
	group_playlist.reset(new Group_Playlist_Vector());
}
Group_Playlist_Container::~Group_Playlist_Container()
{
}/** Returns the group playlist variable */
Group_Playlist_Ptr Group_Playlist_Container::get_group_playlist()
{
	return group_playlist;
}

/** Returns true if the group_playlist variable contains the group name
 ** provided in the parameter.
 ** NOTE: This function may take up to linear time to complete */
bool Group_Playlist_Container::contains_group_name(std::string group_to_find)
{
	for (unsigned int i = 0; i < group_playlist->size(); i++)
	{
		if (group_playlist->at(i).first == group_to_find)
			return true;
	}
	return false;
}

/** Connects a group_name to a playlist name in the QList*/
void Group_Playlist_Container::connect_group_to_playlist(std::string group_name,
		std::string playlist_name)
{
	Group_Playlist_Vector::iterator it = contains_group(group_name);
	if (it != group_playlist->end())
		group_playlist->erase(it);

	std::pair<std::string, std::string> pair_item;
	pair_item.first = group_name;
	pair_item.second = playlist_name;
	group_playlist->push_back(pair_item);

}

std::string Group_Playlist_Container::get_group_playlist_xml()
{
	pugi::xml_document group_playlist_document;
	pugi::xml_node root_node_group_playlist =
			group_playlist_document.append_child("Group_Playlist");

	for (unsigned int i = 0;
			i < Group_Playlist_Container::get_group_playlist()->size(); i++)
	{
		pugi::xml_node group_playlist_node =
				root_node_group_playlist.append_child("Group_Playlist_Item");
		pugi::xml_node group_name_node = group_playlist_node.append_child(
				"Group_Name");
		pugi::xml_node playlist_name_node = group_playlist_node.append_child(
				"Playlist_Name");
		pugi::xml_node group_name_pcdata = group_name_node.append_child(
				pugi::node_pcdata);
		pugi::xml_node playlist_name_pcdata = playlist_name_node.append_child(
				pugi::node_pcdata);
		group_name_pcdata.set_value(get_group_playlist()->at(i).first.c_str());
		playlist_name_pcdata.set_value(
				get_group_playlist()->at(i).second.c_str());
	}
	xml_string_writer writer;
	group_playlist_document.save(writer);
	return writer.result;
}

void Group_Playlist_Container::construct_group_playlist(
		std::string group_playlist)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Group_Playlist_Container::construct_group_playlist()"
			<< std::endl;
	std::cout << group_playlist << std::endl;
#endif // _DEBUG
	pugi::xml_document group_playlist_document;
	group_playlist_document.load(group_playlist.c_str());
	pugi::xml_node root_node = group_playlist_document.child("Group_Playlist");
	for (pugi::xml_node group_playlist_item = root_node.child(
			"Group_Playlist_Item"); group_playlist_item; group_playlist_item =
			group_playlist_item.next_sibling("Group_Playlist_Item"))
	{
		connect_group_to_playlist(group_playlist_item.child_value("Group_Name"),
				group_playlist_item.child_value("Playlist_Name"));
	}
}

void Group_Playlist_Container::construct_group_playlist(
		const char* group_playlist_filename)
{
	pugi::xml_document group_playlist_document;
	group_playlist_document.load_file(group_playlist_filename);
	pugi::xml_node root_node = group_playlist_document.child("Group_Playlist");
	for (pugi::xml_node group_playlist_item = root_node.child(
			"Group_Playlist_Item"); group_playlist_item; group_playlist_item =
			group_playlist_item.next_sibling("Group_Playlist_Item"))
	{
		connect_group_to_playlist(group_playlist_item.child_value("Group_Name"),
				group_playlist_item.child_value("Playlist_Name"));
	}
}

void Group_Playlist_Container::reset_container()
{
	group_playlist.reset(new Group_Playlist_Vector());
}

Group_Playlist_Vector::iterator Group_Playlist_Container::contains_group(
		std::string group_name)
{
	Group_Playlist_Vector::iterator it = group_playlist->end();
	for (unsigned int i = 0; i < group_playlist->size(); i++)
	{
		if (group_playlist->at(i).first == group_name)
		{
			it = group_playlist->begin() + i;
		}
	}
	return it;
}
