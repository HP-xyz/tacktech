#include "Group_Container.h"

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

Group_Container::Group_Container()
{
	groups_and_computers.reset(new Group_Multimap());
}
Group_Container::~Group_Container()
{

}

bool Group_Container::add_group_name(std::string new_group_name)
{
#ifdef _SHOW_DEBUG_OUTPUT
	print_contents();
#endif // _DEBUG
	if (Group_Container::contains_group_name(new_group_name))
		return false;
	else
	{
		Computer_Item item("TEMP_ITEM",
			boost::posix_time::second_clock::universal_time());
		groups_and_computers->insert(
				std::pair<std::string, Computer_Item>(new_group_name,
						item));
		return true;
	}
}

bool Group_Container::add_computer_name(std::string group_name,
		std::string new_computer_name)
{
	if (Group_Container::contains_computer_in_all_groups(new_computer_name)
			!= groups_and_computers->end())
	{
		return false;
	}

	/** CHECKING IF TEMP GROUP IS STILL IN THE LIST */
	Group_Multimap::iterator it = Group_Container::contains_computer_in_group(
			group_name, "TEMP_ITEM");
	if (it != groups_and_computers->end())
	{ //DELETING IF NECCESSARY
		groups_and_computers->erase(it);
	}

	Computer_Item item(new_computer_name,
		boost::posix_time::second_clock::universal_time());
	groups_and_computers->insert(
			std::pair<std::string, Computer_Item>(group_name, item));
#ifdef _SHOW_DEBUG_OUTPUT
	print_contents();
#endif // _DEBUG
	return true;
}

bool Group_Container::add_computer_name( std::string computer_name )
{
	Group_Multimap::iterator it =
		contains_computer_in_all_groups(computer_name);
	if (it == groups_and_computers->end())
	{
		/* There exists no computer identified by computer_name in the
		 * entire container */
		Computer_Item item(computer_name,
			boost::posix_time::second_clock::universal_time());
		groups_and_computers->insert(Group_Item("NONE", item));
		return true;
	}
	else
		return false;
}


#ifdef _SHOW_DEBUG_OUTPUT
void Group_Container::print_contents()
{
	std::cout << "= Group_Container::print_contents()" << std::endl;
	Group_Multimap::iterator it = groups_and_computers->end();
	for (it = groups_and_computers->begin();
		it != groups_and_computers->end();
		it++)
	{
		std::string out_string;
		out_string += " - ";
		out_string += it->first;
		out_string += ": ";
		Group_Computer_Range range = groups_and_computers->equal_range(
				it->first);
		Group_Multimap::iterator it2 = range.first;
		for (it2; it2 != range.second; ++it2)
		{
			out_string += it2->second.first;
			out_string += ", ";
		}
		out_string = out_string.substr(0, out_string.length() - 2);
		std::cout << out_string << std::endl;
	}
}
#endif // _DEBUG
Groups_And_Computers_Ptr Group_Container::get_groups_and_computers()
{
	return groups_and_computers;
}

bool Group_Container::contains_group_name(std::string p_group_name)
{
	Group_Multimap::iterator it = groups_and_computers->end();
	for (it = groups_and_computers->begin();
		it != groups_and_computers->end();
		it++)
	{
		if (it->first == p_group_name)
			return true;
	}
	return false;
}

void Group_Container::remove_group_name(std::string group_name_to_remove)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Group_Container::remove_group_name()" << std::endl;
	std::cout << " - Removing: " << std::endl;
#endif // _DEBUG
	int remove_count = 0;
	for (Group_Multimap::iterator it = groups_and_computers->begin();
			it != groups_and_computers->end(); it++)
	{
		Group_Computer_Range range = groups_and_computers->equal_range(
				it->first);
		Group_Multimap::iterator it2 = range.first;
		for (it2; it2 != range.second; ++it2)
		{
			if (it2->first == group_name_to_remove)
			{
#ifdef _SHOW_DEBUG_OUTPUT
				std::cout << "  - " << it2->first << std::endl;
#endif // _DEBUG

				groups_and_computers->erase(it2);
				remove_count += 1;
			}
		}
	}
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Removed " << remove_count << " items" << std::endl;
#endif // _DEBUG
}

void Group_Container::remove_computer_name(std::string group_name,
		std::string computer_name)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Group_Container::remove_computer_name()" << std::endl;
	int remove_count = 0;
#endif // _DEBUG
	for (Group_Multimap::iterator it = groups_and_computers->begin();
			it != groups_and_computers->end(); it++)
	{
		Group_Computer_Range range = groups_and_computers->equal_range(
				it->first);
		Group_Multimap::iterator it2 = range.first;
		for (it2; it2 != range.second; ++it2)
		{
			if (it2->first == group_name 
				&& it2->second.first == computer_name)
			{
#ifdef _SHOW_DEBUG_OUTPUT
					std::cout << "  - " << it2->first 
						<< " : " << it2->second.first
						<< std::endl;
#endif // _DEBUG
				groups_and_computers->erase(it2);
#ifdef _SHOW_DEBUG_OUTPUT
				remove_count += 1;
#endif // _DEBUG

			}
		}
	}
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Removed " << remove_count << " items" << std::endl;
#endif // _DEBUG
}

Group_Multimap::iterator Group_Container::contains_computer_in_group(
		std::string group_name, std::string computer_name)
{
	for (Group_Multimap::iterator it = groups_and_computers->begin();
			it != groups_and_computers->end(); it++)
	{
		Group_Computer_Range range = groups_and_computers->equal_range(
				it->first);
		Group_Multimap::iterator it2 = range.first;
		for (it2; it2 != range.second; ++it2)
		{
			if (it2->first == group_name 
				&& it2->second.first == computer_name)
			{
				return it2;
			}
		}
	}
	return groups_and_computers->end();;
}

Group_Multimap::iterator Group_Container::contains_computer_in_all_groups(
		std::string computer_name)
{
	for (Group_Multimap::iterator it = groups_and_computers->begin();
			it != groups_and_computers->end(); it++)
	{
		if (it->second.first == computer_name)
		{
			return it;
		}
	}
	return groups_and_computers->end();
}

Group_Multimap Group_Container::get_unique_groups()
{
	Group_Multimap out_map;
	for (Group_Multimap::iterator it1 = groups_and_computers->begin();
			it1 != groups_and_computers->end(); it1++)
	{
		bool playlist_duplicate = false;
		for (Group_Multimap::iterator it2 = out_map.begin();
				it2 != out_map.end(); it2++)
		{
			if (it2->first == it1->first)
			{
				playlist_duplicate = true;
			}
		}
		if (!playlist_duplicate)
		{
			out_map.insert(
					std::pair<std::string, Computer_Item>(it1->first,
							it1->second));
		}
	}
	return out_map;
}

std::string Group_Container::get_groups_and_computers_xml()
{
	pugi::xml_document groups_and_computers_document;
	pugi::xml_node root_node_groups_and_computers =
			groups_and_computers_document.append_child("Groups_And_Computers");

	Group_Multimap group_map = get_unique_groups();
	for (Group_Multimap::iterator it = group_map.begin(); it != group_map.end();
			it++)
	{
		pugi::xml_node group_node = root_node_groups_and_computers.append_child(
				"Group_Item");
		group_node.append_attribute("Group_Name") = it->first.c_str();
		Group_Computer_Range group_range =
				get_groups_and_computers()->equal_range(it->first);
		for (Group_Multimap::iterator it2 = group_range.first;
				it2 != group_range.second; ++it2)
		{
			pugi::xml_node computer_item_node = group_node.append_child(
					"Computer_Item");
			pugi::xml_node computer_node = computer_item_node.append_child(
					"Computer");
			pugi::xml_node computer_pcdata = computer_node.append_child(
					pugi::node_pcdata);
			computer_pcdata.set_value(it2->second.first.c_str());
		}
	}
	xml_string_writer writer;
	groups_and_computers_document.save(writer);
	return writer.result;
}

void Group_Container::construct_groups_and_computers(
		std::string groups_and_computers)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Group_Container::construct_groups_and_computers()"
			<< std::endl;
	std::cout << groups_and_computers << std::endl;
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
			add_computer_name(group_item.attribute("Group_Name").as_string(),
					computer_item.child_value("Computer"));
		}
	}
}

void Group_Container::construct_groups_and_computers(
		const char* groups_and_computers_filename)
{
	/* Read groups_and_computers from file */
	pugi::xml_document groups_and_computers_document;
	groups_and_computers_document.load_file(groups_and_computers_filename);
	pugi::xml_node root_node = groups_and_computers_document.child(
			"Groups_And_Computers");
	for (pugi::xml_node group_item = root_node.child("Group_Item"); group_item;
			group_item = group_item.next_sibling("Group_Item"))
	{
		for (pugi::xml_node computer_item = group_item.child("Computer_Item");
				computer_item;
				computer_item = computer_item.next_sibling("Computer_Item"))
		{
			add_computer_name(group_item.attribute("Group_Name").as_string(),
					computer_item.child_value("Computer"));
		}
	}
}

void Group_Container::reset_container()
{
	groups_and_computers.reset(new Group_Multimap());
}

