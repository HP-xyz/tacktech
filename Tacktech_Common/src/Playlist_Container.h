#ifndef PLAYLIST_CONTAINER_H
#define PLAYLIST_CONTAINER_H
#ifdef _SHOW_DEBUG_OUTPUT
#define _IMPORTANT_OUTPUT _SHOW_DEBUG_OUTPUT
#endif // _SHOW_DEBUG_OUTPUT
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
 ** contain the organizations that are able to use the aforementioned Playlist object. */
typedef std::pair<Playlist_Ptr, std::vector<std::string> > Playlist_Item;
typedef std::set<Playlist_Item> Container;
class Playlist_Container
{
public:
	Playlist_Container();
	/** Takes a string representation of a Playlist_Container
	 ** object, and constructs the Playlist_Container object. */
	Playlist_Container(std::string, std::set<std::string>);
	~Playlist_Container();
	/** Returns a Container that will contain only the Playlist's that
	 ** are allowed to be used by the std::string supplied in the parameter.
	 ** This string should be a ORGNIZATION NAME. */
	Container get_playlist_container(std::string);

	/** Returns a Playlist_Container that will contain only the Playlist's that
	 ** are allowed to be used by the std::string supplied in the parameter.
	 ** This string should be a GROUP NAME. */
	Playlist_Container get_playlist_container(std::string, std::string);

	boost::shared_ptr<Container> get_playlist_container();

	boost::shared_ptr<std::set<Playlist_Ptr> > get_playlists_of_group(std::string);

	/** Adds a Playlist to the Container, including a std::vector of strings
	 ** to identify which ORGANIZAITONS have access to it. */
	void add_playlist(Playlist_Ptr, std::vector<std::string>, std::set<std::string>);

	/** Adds a Playlist to the Container, including a std::vector of strings
	 ** to identify which ORGANIZAITONS have access to it. */
	void add_playlist(Playlist_Ptr, std::vector<std::string>);

	/** Returns a std::string representation of a playlist_container,
	 ** containing only the playlists that can be used by the group,
	 ** identified by the parameter string. */
	std::string get_playlist_container_xml(std::string);

	/** Returns a std::string representation of a playlist_container */
	std::string get_playlist_container_xml();

	void set_playlist_container_name(std::string);
	std::string get_playlist_container_name();
	bool update_playlist(Playlist_Container, std::string);

	std::string get_current_playing_item();

	std::pair<std::string,int> get_next_item();

	std::vector<std::string> get_needed_items(std::vector<std::string>);

	/** Removes a playlist identified by the std::string in the parameter,
	 ** which should be a playlist_name */
	void remove_playlist(std::string);

	/** Removes a playlist_item identified by the first std::string parameter
	 ** as a playlist_name, and the second std::string parameter as a
	 ** item_name*/
	void remove_playlist_item(std::string, std::string);

	void remove_playlist_of_group(std::string);

#ifdef _SHOW_DEBUG_OUTPUT
	void print_contents();
#endif // _SHOW_DEBUG_OUTPUT
private:
	boost::shared_ptr<Container> m_playlist_container;
	std::string m_playlist_container_name;

	/** Takes a comma_separated_list and creates a
	 ** std::vector of strings. */
	std::vector<std::string> make_vector(std::string);
	Playlist_Ptr get_playlist_ptr(std::string, std::string);
};
typedef boost::shared_ptr<Playlist_Container> Playlist_Container_Ptr;
#endif //PLAYLIST_CONTAINER_H
