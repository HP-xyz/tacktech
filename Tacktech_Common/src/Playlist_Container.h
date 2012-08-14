#ifndef PLAYLIST_CONTAINER_H
#define PLAYLIST_CONTAINER_H
#include <map>
#include <utility>
#include <algorithm>
#include <typeinfo>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <pugixml.hpp>

#ifdef _SHOW_DEBUG_OUTPUT
#include <iostream>
#endif // _DEBUG
typedef boost::shared_ptr<
		std::multimap<std::string, std::pair<std::string, int> > > Playlist_Ptr;
typedef std::multimap<std::string, std::pair<std::string, int> > Playlist_Multimap;
typedef std::map<std::string, std::pair<std::string, int> > Playlist_Map;
typedef std::pair<std::string, std::pair<std::string, int> > Playlist_Item;
typedef std::pair<
		std::multimap<std::string, std::pair<std::string, int> >::iterator,
		std::multimap<std::string, std::pair<std::string, int> >::iterator> Playlist_Range;
class Playlist_Container
{
public:
	Playlist_Container();
	~Playlist_Container();

	/** Function to add a playlist name to the Playlist_Container. Returns
	 ** false if the playlist name already exists. */
	bool add_playlist_name(std::string);
	bool add_filename(std::string, std::string, int);
	Playlist_Ptr get_playlist();

	void remove_playlist(std::string playlist_name);
	void remove_filename_from_playlist(std::string playlist_name,
			std::string filename);

	std::string get_playlists_xml();
	void construct_playlist(std::string playlist);
	void construct_playlist(const char *playlist_filename);

	/** Function to return a Playlist_Map of only the unique playlists, 
	 ** uniqueness being based on playlist_name.
	 ** NOTE
	 ** FUNCTION TAKES N^2 TIME TO COMPLETE
	 ** TODO OPTIMIZE FUNCTION */
	Playlist_Multimap get_unique_playlists();

	Playlist_Range get_files_in_playlist(std::string playlist_name);

	void reset_container();

#ifdef _SHOW_DEBUG_OUTPUT
	void print_contents();
#endif // _DEBUG
private:
	Playlist_Ptr playlist;
	std::pair<std::string, int> temp_pair;

	/** Checks if the filename exists in any of the playlists. This function
	 ** will return an iterator to playlist->end() if the playlist DOES NOT
	 ** contain the filename, else it returns an iterator to the item searched
	 ** for. 
	 ** NOTE: This function takes n^2 time to complete*/
	Playlist_Multimap::iterator
	contains_filename_in_all_playlists(std::string filename);
};
typedef boost::shared_ptr<Playlist_Container> Playlist_Container_Ptr;
#endif //PLAYLIST_CONTAINER_H
