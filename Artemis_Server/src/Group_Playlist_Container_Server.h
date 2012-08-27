#ifndef GROUP_PLAYLIST_CONTAINER_SERVER_H
#define GROUP_PLAYLIST_CONTAINER_SERVER_H
#include <utility>
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include <pugixml.hpp>
#include <Group_Playlist_Container.h>
#ifdef _SHOW_DEBUG_OUTPUT
#include <iostream>
#endif // _DEBUG
typedef std::map<std::string, Group_Playlist_Container> Group_Playlist_Container_Server_Map;
class Group_Playlist_Container_Server
{
public:
	Group_Playlist_Container_Server(void);
	~Group_Playlist_Container_Server(void);

	/** Add an organization to the container. The organization is added to
	 ** the container map, and is identified by the key organization_name.
	 ** Return true if teh organization could be added, otherwise returns
	 ** false. False will be returned when the container already contains
	 ** the aforementioned organization_name */
	bool add_organization(std::string organization_name, Group_Playlist_Container group_playlist);

	/** Returns a XML representation of the organization_map container*/
	std::string get_group_playlist_xml();

	void construct_group_playlist(std::string organization_name,
		std::string group_playlist);
	void construct_group_playlist(
		const char* group_playlist_filename);

	/** Returns a reference to the organization map */
	Group_Playlist_Container_Server_Map& get_organization_map();
private:
	Group_Playlist_Container_Server_Map organization_map;
};
typedef boost::shared_ptr<Group_Playlist_Container_Server> Group_Playlist_Container_Server_Ptr;
#endif //GROUP_PLAYLIST_CONTAINER_SERVER_H