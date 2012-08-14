#ifndef GROUP_PLAYLIST_CONTAINER
#define GROUP_PLAYLIST_CONTAINER
#include <list>
#include <utility>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <pugixml.hpp>

#ifdef _SHOW_DEBUG_OUTPUT
#include <iostream>
#endif // _DEBUG
typedef boost::shared_ptr<std::vector<std::pair<std::string, std::string> > > Group_Playlist_Ptr;
typedef std::vector<std::pair<std::string, std::string> > Group_Playlist_Vector;
class Group_Playlist_Container
{
public:
	Group_Playlist_Container();
	~Group_Playlist_Container();
	Group_Playlist_Ptr get_group_playlist();
	bool contains_group_name(std::string);
	void connect_group_to_playlist(std::string, std::string);
	std::string get_group_playlist_xml();

	void construct_group_playlist(std::string group_playlist);
	void construct_group_playlist(const char* group_playlist_filename);

	void reset_container();
private:
	Group_Playlist_Ptr group_playlist;
	Group_Playlist_Vector::iterator contains_group(std::string group_name);
};
typedef boost::shared_ptr<Group_Playlist_Container> Group_Playlist_Container_Ptr;
#endif //GROUP_PLAYLIST_CONTAINER
