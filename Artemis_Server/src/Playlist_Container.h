#ifndef PLAYLIST_CONTAINER_H
#define PLAYLIST_CONTAINER_H
#include <map>
#include <utility>
#include <boost/lexical_cast.hpp>

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG
typedef std::shared_ptr<std::multimap<std::string, std::pair<std::string, int> > > Playlist_Ptr;
typedef std::multimap<std::string, std::pair<std::string, int> > Playlist_Multimap;
typedef std::pair<std::string, std::pair<std::string, int> > Playlist_Item;
typedef std::pair<std::multimap<std::string, std::pair<std::string, int> >::iterator,
	std::multimap<std::string, std::pair<std::string, int> >::iterator> 
	Playlist_Range;
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
#ifdef _DEBUG
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

#endif //PLAYLIST_CONTAINER_H
