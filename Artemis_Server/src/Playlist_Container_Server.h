#ifndef PLAYLIST_CONTAINER_SERVER_H
#define PLAYLIST_CONTAINER_SERVER_H
#include <utility>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <boost/shared_ptr.hpp>
#include <pugixml.hpp>
#include <Playlist_Container.h>
#ifdef _SHOW_DEBUG_OUTPUT
#include <iostream>
#endif // _DEBUG
typedef std::map<std::string, Playlist_Container> Playlist_Container_Server_Map;
class Playlist_Container_Server
{
public:
	Playlist_Container_Server(void);
	~Playlist_Container_Server(void);

	/** Add an organization to the container. The organization is added to
	 ** the container map, and is identified by the key organization_name.
	 ** Return true if teh organization could be added, otherwise returns
	 ** false. False will be returned when the container already contains
	 ** the aforementioned organization_name */
	bool add_organization(std::string organization_name,
		Playlist_Container playlist);

	/** Returns a XML representation of the organization_map container*/
	std::string get_playlists_xml();

	/** Constructs a playlist from std::string */
	void construct_playlist(std::string organization_name,
		std::string playlist);
	/** Constructs a playlist from file */
	void construct_playlist(
		const char* playlist_filename);

	/** Returns a reference to the organization map */
	Playlist_Container_Server_Map& get_organization_map();

private:
	Playlist_Container_Server_Map organization_map;
};
typedef boost::shared_ptr<Playlist_Container_Server> Playlist_Container_Server_Ptr;
#endif //PLAYLIST_CONTAINER_SERVER_H