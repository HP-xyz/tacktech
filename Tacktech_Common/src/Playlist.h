#ifndef PLAYLIST_H
#define PLAYLIST_H
#include <string>
#include <vector>
#include <boost/date_time.hpp>
#include <boost/shared_ptr.hpp>
class Playlist
{
public:
	Playlist(void);
	~Playlist(void);
	/** Returns a std::string identifying the playlist */
	std::string get_playlist_name();
	/** Returns a std::vector<std::pair<std::string,int>>
	 ** to the items contained in the playlist.
	 ** NOTE: Pair is -> std::string to identify filename
	 **               -> int to identify pause
	 */
	std::vector<std::pair<std::string,int>> get_playlist_items();
	/** Returns an int indicating the index of the file currently
	 ** being played. */
	int get_current_item_index();
	/** Returns an boost::posix_time::time_duration to indicate the
	 ** time the playlist should start playing. */
	boost::posix_time::time_duration get_start_time();
	/** Returns an boost::posix_time::time_duration to indicate the
	 ** time the playlist should start playing. */
	boost::posix_time::time_duration get_end_time();

	/** Receives a std::string that will be used to indentify
	 ** the playlist. */
	void set_playlist_name(std::string);
	/** Receives a std::vector<std::pair<std::string,int>> that
	 ** will replace the current playlist_items vector. */
	void set_playlist_items(std::vector<std::pair<std::string,int>>);
	/** Receives a std::string and int that will be made a pair to
	 ** add to the playlist_items of the Playlist. */
	void add_playlist_item(std::string, int);
	/** Removes a playlist_item at the index indicated by the 
	 ** received int. */
	void remove_playlist_item(int);
	/** Receives a int to indicate the currently being played
	 ** item index. */
	void set_current_item_index(int);
	/** Receives a boost::posix_time::time_duration that
	 ** will be used to identify the start time when the
	 ** playlist should start playing.
	 ** NOTE: Time zones must be taken into account by
	          the calling function. */
	void set_start_time(boost::posix_time::time_duration);
	/** Receives a boost::posix_time::time_duration that
	 ** will be used to identify the end time when the
	 ** playlist should stop playing.
	 ** NOTE: Time zones must be taken into account by
	          the calling function. */
	void set_end_time(boost::posix_time::time_duration);
private:
	std::string m_playlist_name;
	std::vector<std::pair<std::string,int>> m_playlist_items;
	int m_current_item_index;
	boost::posix_time::ptime m_start_time;
	boost::posix_time::ptime m_end_time;
	//ADD START AND END TIME
};
typedef boost::shared_ptr<Playlist> Playlist_Ptr;
#endif //PLAYLIST_H