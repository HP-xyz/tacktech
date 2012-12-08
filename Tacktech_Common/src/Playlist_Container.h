#ifndef PLAYLIST_CONTAINER_H
#define PLAYLIST_CONTAINER_H
#include <set>
#include <vector>
#include <algorithm>
#include <utility>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <pugixml.hpp>

#include "Playlist.h"

#ifdef _SHOW_DEBUG_OUTPUT
#include <iostream>
#endif // _DEBUG
typedef std::multimap<std::string, std::pair<std::string, int> > Playlist_Multimap;
typedef std::map<std::string, std::pair<std::string, int> > Playlist_Map;
typedef std::pair<
		std::multimap<std::string, std::pair<std::string, int> >::iterator,
		std::multimap<std::string, std::pair<std::string, int> >::iterator> Playlist_Range;

/** Container is the type that will hold the playlists. It consists of a 
 ** set, containing a pair where the first member of the pair is the 
 ** Playlist object, and the second member is a vector of strings, that will
 ** contain the groups that are able to use the aforementioned vector. */
typedef std::pair<Playlist_Ptr, std::vector<std::string> > Playlist_Item;
typedef std::set<Playlist_Item> Container;
class Playlist_Container
{
public:
	Playlist_Container();
	~Playlist_Container();
	/** Returns a Container that will contain only the Playlist's that
	 ** are allowed to be used by the std::string supplied in the parameter.
	 ** This string should be a group_name. */
	Container get_playlist_container(std::string);
	/** Adds a Playlist to the Container, including a std::vector of strings
	 ** to identify which groups have access to it. */
	void add_playlist(Playlist_Ptr, std::vector<std::string>);
private:
	Container m_playlist_container;
};
typedef boost::shared_ptr<Playlist_Container> Playlist_Container_Ptr;
#endif //PLAYLIST_CONTAINER_H
