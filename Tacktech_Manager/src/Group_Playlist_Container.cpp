#include "Group_Playlist_Container.h"
Group_Playlist_Container::Group_Playlist_Container()
{
    group_playlist = new QList<QPair<QString, QString> >;
}
Group_Playlist_Container::~Group_Playlist_Container()
{
    delete group_playlist;
}
/** Returns the group playlist variable */
QList<QPair<QString, QString> >*
Group_Playlist_Container::get_group_playlist()
{
    return group_playlist;
}

/** Returns true if the group_playlist variable contains the group name
 ** provided in the parameter.
 ** NOTE: This function may take up to linear time to complete */
bool Group_Playlist_Container::contains_group_name( QString group_to_find)
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
    QString group_name, QString playlist_name)
{
    for (int i = 0; i < group_playlist->size(); i++)
    {
        if (group_playlist->at(i).first == group_name)
        {
            QPair<QString, QString> pair_item;
            pair_item.first = group_name;
            pair_item.second = playlist_name;
            group_playlist->replace(i, pair_item);
        }
    }
}
