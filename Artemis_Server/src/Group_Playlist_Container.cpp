#include "Group_Playlist_Container.h"
Group_Playlist_Container::Group_Playlist_Container()
{
    group_playlist.reset(
		new std::vector< std::pair< std::string, std::string > > );
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
bool Group_Playlist_Container::contains_group_name(
	std::string group_to_find)
{
    for(int i = 0; i < group_playlist->size(); i++)
    {
        if(group_playlist->at(i).first == group_to_find)
            return true;
    }
    return false;
}

/** Connects a group_name to a playlist name in the QList*/
void Group_Playlist_Container::connect_group_to_playlist(
    std::string group_name, std::string playlist_name)
{
    for (int i = 0; i < group_playlist->size(); i++)
    {
        if (group_playlist->at(i).first == group_name)
        {
            std::pair<std::string, std::string> pair_item;
            pair_item.first = group_name;
            pair_item.second = playlist_name;
			Group_Playlist::iterator it = group_playlist->begin() + i;
			group_playlist->erase(it);
            group_playlist->push_back(pair_item);
        }
    }
}
