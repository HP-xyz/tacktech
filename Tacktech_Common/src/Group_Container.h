#ifndef GROUP_CONTAINER_H
#define GROUP_CONTAINER_H
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <boost/shared_ptr.hpp>
#include <pugixml.hpp>
#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG
typedef boost::shared_ptr<std::multimap<std::string, std::string> > Groups_And_Computers_Ptr;
typedef std::multimap<std::string, std::string> Group_Multimap;
typedef std::map<std::string, std::string> Group_Map;
typedef std::pair<std::multimap<std::string, std::string>::iterator,
		std::multimap<std::string, std::string>::iterator> Group_Computer_Range;
class Group_Container
{
public:
	Group_Container();
	~Group_Container();

	/** Function to add a group name to the Group_Container. Returns false
	 ** if the group name already exists */
	bool add_group_name(std::string);

	/** Function to add a computer name to the Group_Container. Returns false
	 ** if the computer name is already in another group.
	 ** NOTE: This function takes n^2 time */
	bool add_computer_name(std::string, std::string);

	/** Removes the group name provided in the parameter from the
	 ** groups_and_computers variable */
	void remove_group_name(std::string);

	/** Removes the computer name from the group name provided in the
	 ** parameters from the groups_and_computers variable */
	void remove_computer_name(std::string, std::string);

	/** Returns the groups_and_computers shared_ptr to the map,
	 ** to the calling class */
	Groups_And_Computers_Ptr get_groups_and_computers();

	/** Returns true if the groups_and_computers variable has the parameter
	 ** as a group name */
	bool contains_group_name(std::string);

	/** Returns a std::map<std::string, std::string> containing only
	 ** unique group names 
	 ** NOTE
	 ** FUNCTION TAKES N^2 TIME TO COMPLETE
	 ** TODO OPTIMIZE FUNCTION */
	Group_Multimap get_unique_groups();

	/** Checks to see if a item with group_name and computer_name already 
	 ** exists in the Group_Container. Returns an iterator to 
	 ** groups_and_computers.end() if the Group_Container DOES NOT contain
	 ** the provided value item, else returns an iterator to the item found */
	Group_Multimap::iterator contains_computer_in_group(std::string group_name,
			std::string computer_name);

	/** Checks if in item with computer_name already already exists in
	 ** the Group_Container. Returns an iterator to groups_and_computers.end()
	 ** if the Group_Container DOES NOT contain the computer_name, else returns
	 ** an iterator to the item found */
	Group_Multimap::iterator contains_computer_in_all_groups(
			std::string computer_name);

	void construct_groups_and_computers(std::string groups_and_computers);
	void construct_groups_and_computers(
			const char* groups_and_computers_filename);

	std::string get_groups_and_computers_xml();
	void reset_container();
#ifdef _DEBUG
	/** Function to print the contents of the Group_Container. Only compiled
	 ** in debug mode */
	void print_contents();
#endif // _DEBUG
private:
	Groups_And_Computers_Ptr groups_and_computers;
};
typedef boost::shared_ptr<Group_Container> Group_Container_Ptr;
#endif //GROUP_CONTAINER_H
