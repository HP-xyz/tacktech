#ifndef DISPLAY_CLIENT_H
#define DISLPAY_CLIENT_H
#include <string>
#include <set>
#include <boost/date_time.hpp>
#include <boost/shared_ptr.hpp>
#include <pugixml.hpp>
#include "Playlist_Container.h"
class Display_Client
{
public:
	Display_Client(void);
	Display_Client(std::string);
	~Display_Client(void);

	/** Return std::string reference to the identification
	 ** string. */
	std::string get_identification();
	/** Return a boost::posix_time::ptime reference to the
	 ** last recorded ping that was received. */
	boost::posix_time::ptime get_last_ping();
	/** Returns a std::set<std::string> reference to the
	 ** groups that the Display_Client is part of. */
	boost::shared_ptr<std::set<std::string> > get_groups();
	/** Returns a std::set<std::string> containing the organizations
	 ** that the Display_Client belongs to. */
	boost::shared_ptr<std::set<std::string>> get_organizations();
	/** Returns a Playlist_Container_Ptr to the Dislpay_
	 ** Client object. */
	Playlist_Container_Ptr get_playlist_container();

	/** Receives a std::string that will identify the
	 ** Display_Client. */
	void set_identification(std::string);
	/** Receives a boost::posix_time::ptime that will
	 ** indicate the last successful ping the Display_
	 ** Client sent. */
	void set_last_ping(boost::posix_time::ptime);
	/** Receives a std::set<std::string> that will
	 ** replace the current groups the Dislay_Client
	 ** is part of. */
	void set_groups(std::set<std::string>);
	/** Receives a std::set<std::string that will replace
	 ** the current organizations for the Display_Client. */
	void set_organizations(std::set<std::string>);
	/** Receives a Playlist_Container_Ptr that will replace
	 ** the current playlist container of the Display_Client. */
	void set_playlist_container(Playlist_Container_Ptr);

	/** Receives a std::string that will be added to the groups
	 ** the Display_Client is part of. */
	bool add_group(std::string);
	/** Receives a std:string the will be added to the organizations
	 ** the Display_Client belongs to. */
	bool add_organization(std::string);

	/** Return a std::string representation of the groups std::set.
	 ** The set will be returned as a comma separated list. */
	std::string get_groups_string();

	void set_playlist_container_name(std::string);
	std::string get_playlist_container_name();

	/** Returns a std::string representation of the Display_Client. */
	std::string get_display_client_xml();
private:
	std::string m_identification;
	boost::posix_time::ptime m_last_ping;
	boost::shared_ptr<std::set<std::string> > m_organizations;
	boost::shared_ptr<std::set<std::string> > m_groups;
	Playlist_Container_Ptr m_playlist_container;
	std::string m_playlist_container_name;
	/** Creates a comma separated list from a std::set<std::string> */
	std::string make_list(std::set<std::string>);

	/** Takes a comma_separated_list and creates a
	 ** std::vector of strings. */
	std::set<std::string> make_set(std::string);
};
typedef boost::shared_ptr<Display_Client> Display_Client_Ptr;
#endif //DISPLAY_CLIENT_H
