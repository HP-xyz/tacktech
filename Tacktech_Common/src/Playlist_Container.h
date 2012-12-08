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
	/** Takes a string representation of a Playlist_Container
	 ** object, and constructs the Playlist_Container object. */
	Playlist_Container(std::string);
	~Playlist_Container();
	/** Returns a Container that will contain only the Playlist's that
	 ** are allowed to be used by the std::string supplied in the parameter.
	 ** This string should be a group_name. */
	Container get_playlist_container(std::string);
	/** Adds a Playlist to the Container, including a std::vector of strings
	 ** to identify which groups have access to it. */
	void add_playlist(Playlist_Ptr, std::vector<std::string>);
	/** Returns a std::string representation of a playlist_container,
	 ** containing only the playlists that can be used by the group,
	 ** identified by the parameter string. */
	std::string get_playlist_container_xml(std::string);

	void set_playlist_container_name(std::string);
	std::string get_playlist_container_name();

#ifdef _SHOW_DEBUG_OUTPUT
	void print_contents();
#endif // _SHOW_DEBUG_OUTPUT
private:
	Container m_playlist_container;
	std::string m_playlist_container_name;

	/** Takes a comma_separated_list and creates a
	 ** std::vector of strings. */
	std::vector<std::string> make_vector(std::string);

	/** Creates a comma separated list from a std::set<std::string> */
	std::string make_list(std::vector<std::string>);
};
typedef boost::shared_ptr<Playlist_Container> Playlist_Container_Ptr;
#endif //PLAYLIST_CONTAINER_H
