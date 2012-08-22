#ifndef GROUP_CONTAINER_SERVER_H
#define GROUP_CONTAINER_SERVER_H
#include <utility>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <boost/shared_ptr.hpp>
#include <pugixml.hpp>
#include <Group_Container.h>
#ifdef _SHOW_DEBUG_OUTPUT
#include <iostream>
#endif // _DEBUG
typedef std::map<std::string, Group_Container> Group_Container_Server_Map;
class Group_Container_Server
{
public:
	Group_Container_Server(void);
	~Group_Container_Server(void);

	/** Add an organization to the container. The organization is added to
	 ** the container map, and is identified by the key organization_name.
	 ** Return true if teh organization could be added, otherwise returns
	 ** false. False will be returned when the container already contains
	 ** the aforementioned organization_name */
	bool add_organization(std::string organization_name, Group_Container);

	/** Returns a XML representation of the organization_map container*/
	std::string get_groups_and_computers_xml();

	void construct_groups_and_computers(std::string organization_name,
		std::string groups_and_computers);
	void construct_groups_and_computers(
		const char* groups_and_computers_filename);

	/** Returns a reference to the organization map */
	Group_Container_Server_Map& get_organization_map();

#ifdef _SHOW_DEBUG_OUTPUT
	/** Function to print the contents of the Group_Container. Only compiled
	 ** in debug mode */
	void print_contents();
#endif // _DEBUG
private:
	Group_Container_Server_Map organization_map;
};
typedef boost::shared_ptr<Group_Container_Server> Group_Container_Server_Ptr;
#endif //GROUP_CONTAINER_SERVER_H