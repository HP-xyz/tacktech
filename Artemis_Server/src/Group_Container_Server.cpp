#include "Group_Container_Server.h"

/** Struct to define a xml_writer to string.
 ** Copied directly from the pugixml quickstart */
struct xml_string_writer: pugi::xml_writer
{
	std::string result;

	virtual void write(const void* data, size_t size)
	{
		result += std::string(static_cast<const char*>(data), size);
	}
};

Group_Container_Server::Group_Container_Server(void)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout
		<< "=Group_Container_Server::Group_Container_Server()"
		<< std::endl;
#endif // _SHOW_DEBUG_OUTPUT
}


Group_Container_Server::~Group_Container_Server(void)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout
		<< "=Group_Container_Server::~Group_Container_Server()"
		<< std::endl;
#endif // _SHOW_DEBUG_OUTPUT
}
bool Group_Container_Server::add_organization(
	std::string organization_name, Group_Container group_container)
{
	if (organization_map.find(organization_name) == organization_map.end())
	{
		organization_map.insert(std::pair<std::string, Group_Container>
			(organization_name, group_container));
		return true;
	}
	else
		return false;
}

Group_Container_Server_Map& Group_Container_Server::get_organization_map()
{
	return organization_map;
}

std::string Group_Container_Server::get_groups_and_computers_xml()
{
	std::string return_str;
	return_str += "<Tacktech>";
	for (Group_Container_Server_Map::iterator it = organization_map.begin();
		it !=  organization_map.end();
		++it)
	{
		return_str += "<Group_Container_Item Organization_Name=\"";
		return_str += it->first;
		return_str += "\"/>";
		return_str += it->second.get_groups_and_computers_xml();
		return_str += "</Group_Container_Item>";
	}
	return_str += "</Tacktech>";
	return return_str;
}

void Group_Container_Server::construct_groups_and_computers(
	std::string organization_name,
	std::string groups_and_computers )
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Group_Container_Server::construct_groups_and_computers(STRING)"
		<< std::endl;
	int items_added = 0;
#endif // _DEBUG
	pugi::xml_document groups_and_computers_document;
	groups_and_computers_document.load(groups_and_computers.c_str());
	pugi::xml_node root_node = groups_and_computers_document.child(
		"Groups_And_Computers");
	for (pugi::xml_node group_item = root_node.child("Group_Item"); group_item;
		group_item = group_item.next_sibling("Group_Item"))
	{
		for (pugi::xml_node computer_item = group_item.child("Computer_Item");
			computer_item;
			computer_item = computer_item.next_sibling("Computer_Item"))
		{
			if(organization_map[organization_name].add_computer_name(
				group_item.attribute("Group_Name").as_string(),
				computer_item.child_value("Computer")))
			{
#ifdef _SHOW_DEBUG_OUTPUT
				items_added += 1;
#endif // _DEBUG
			}
		}
	}
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Added " << items_added << " new items" << std::endl;
#endif // _DEBUG
}

void Group_Container_Server::construct_groups_and_computers(
	const char* groups_and_computers_filename )
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Group_Container_Server::construct_groups_and_computers(FILE)"
		<< std::endl;
#endif // _DEBUG
	/* Read groups_and_computers from file */
	pugi::xml_document groups_and_computers_document;
	groups_and_computers_document.load_file(groups_and_computers_filename);
	pugi::xml_node root_node = groups_and_computers_document.child("Tacktech");
	std::string organization_name =
		root_node.child("Group_Container_Item")
		.attribute("Organization_Name").as_string();
	pugi::xml_node groups_and_computers_node =
		groups_and_computers_document.child("Groups_And_Computers");
	for (pugi::xml_node group_item =
		groups_and_computers_node.child("Group_Item");
		group_item;
		group_item = group_item.next_sibling("Group_Item"))
	{
		for (pugi::xml_node computer_item = group_item.child("Computer_Item");
			computer_item;
			computer_item = computer_item.next_sibling("Computer_Item"))
		{
			organization_map[organization_name].add_computer_name(
				group_item.attribute("Group_Name").as_string(),
				computer_item.child_value("Computer"));
		}
	}
}
