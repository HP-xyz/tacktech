#ifndef GROUP_PLAYLIST_CONTAINER
#define GROUP_PLAYLIST_CONTAINER
#include <list>
#include <utility>
#include <string>
#include <vector>

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG
typedef 
	std::shared_ptr<std::vector<std::pair<std::string, std::string> > > Group_Playlist_Ptr;
typedef
	std::vector<std::pair<std::string, std::string> > Group_Playlist;
class Group_Playlist_Container
{
public:
    Group_Playlist_Container();
    ~Group_Playlist_Container();
    Group_Playlist_Ptr get_group_playlist();
    bool contains_group_name(std::string);
    void connect_group_to_playlist(std::string, std::string);
private:
    Group_Playlist_Ptr group_playlist;
};
#endif //GROUP_PLAYLIST_CONTAINER
