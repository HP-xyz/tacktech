#include "Group_Container.h"
Group_Container::Group_Container()
{

}
Group_Container::~Group_Container()
{

}

bool Group_Container::add_group_name( std::string new_group_name)
{
#ifdef _DEBUG
    print_contents();
#endif // _DEBUG
    if (Group_Container::contains_group_name(new_group_name))
        return false;
    else
    {
        groups_and_computers->insert(
			std::pair<std::string, std::string>(new_group_name, "TEMP_ITEM"));
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
	Group_Playlist_Multimap::const_iterator it =
		Group_Container::contains_computer_in_group(group_name, "TEMP_ITEM");
	if(it != groups_and_computers->end())
	{//DELETING IF NECCESSARY
		groups_and_computers->erase(it);
	}

    groups_and_computers->insert(std::pair<std::string, std::string>
		(group_name, new_computer_name));
#ifdef _DEBUG
    print_contents();
#endif // _DEBUG
    return true;
}

#ifdef _DEBUG
void Group_Container::print_contents()
{
    std::cout << "= Group_Container::print_contents()" << std::endl;
	Group_Playlist_Multimap::iterator it = groups_and_computers->end();
	for(it = groups_and_computers->begin();
		it != groups_and_computers->end(); it++)
	{
		std::string out_string;
		out_string += " - ";
		out_string += it->first;
		out_string += ": ";
		Group_Playlist_Range range =
			groups_and_computers->equal_range(it->first);
		Group_Playlist_Multimap::iterator it2 = range.first;
		for (it2; it2 != range.second; ++it2)
		{
			out_string += it2->second;
			out_string += ", ";
		}
		out_string = out_string.substr(0, out_string.length() - 2);
		std::cout << out_string << std::endl;
	}
}
#endif // _DEBUG

Group_Container_Ptr Group_Container::get_groups_and_computers()
{
    return groups_and_computers;
}

bool Group_Container::contains_group_name( std::string p_group_name)
{
	Group_Playlist_Multimap::iterator it = groups_and_computers->end();
	for(it = groups_and_computers->begin();
		it != groups_and_computers->end(); it++)
	{
		if (it->first == p_group_name)
			return true;
	}
	return false;
}

void Group_Container::remove_group_name( std::string group_name_to_remove)
{
#ifdef _DEBUG
    std::cout << "= Group_Container::remove_group_name()" << std::endl;
	std::cout << " - Removing: " << std::endl;
#endif // _DEBUG
	int remove_count = 0;
	for(Group_Playlist_Multimap::iterator it = groups_and_computers->begin();
		it != groups_and_computers->end(); it++)
	{
		Group_Playlist_Range range =
			groups_and_computers->equal_range(it->first);
		Group_Playlist_Multimap::iterator it2 = range.first;
		for (it2; it2 != range.second; ++it2)
		{
			if(it2->first == group_name_to_remove)
			{
				std::cout << "  - " << it2->first << std::endl;
				groups_and_computers->erase(it2);
				remove_count += 1;
			}
		}
	}
#ifdef _DEBUG
    std::cout << " - Removed " << remove_count << " items" << std::endl;
#endif // _DEBUG
}

void Group_Container::remove_computer_name( std::string group_name,
        std::string computer_name )
{
#ifdef _DEBUG
    std::cout << "= Group_Container::remove_computer_name()" << std::endl;
	int remove_count = 0;
#endif // _DEBUG
	for(Group_Playlist_Multimap::iterator it = groups_and_computers->begin();
		it != groups_and_computers->end(); it++)
	{
		Group_Playlist_Range range =
			groups_and_computers->equal_range(it->first);
		Group_Playlist_Multimap::iterator it2 = range.first;
		for (it2; it2 != range.second; ++it2)
		{
			if(it2->first == group_name && it2->second == computer_name)
			{
				std::cout << "  - " << it2->first << " : " 
					<< it2->second << std::endl;
				groups_and_computers->erase(it2);
				remove_count += 1;
			}
		}
	}
#ifdef _DEBUG
    std::cout << " - Removed " << remove_count << " items" << std::endl;
#endif // _DEBUG
}

Group_Playlist_Multimap::iterator
	Group_Container::contains_computer_in_group( 
	std::string group_name, std::string computer_name)
{
	for(Group_Playlist_Multimap::iterator it 
		= groups_and_computers->begin();
		it != groups_and_computers->end(); it++)
	{
		Group_Playlist_Range range =
			groups_and_computers->equal_range(it->first);
		Group_Playlist_Multimap::iterator it2 = range.first;
		for (it2; it2 != range.second; ++it2)
		{
			if(it2->first == group_name && it2->second == computer_name)
			{
				return it2;
			}
		}
	}
	return groups_and_computers->end();;
}

Group_Playlist_Multimap::iterator Group_Container::contains_computer_in_all_groups(
	std::string computer_name )
{
	for (Group_Playlist_Multimap::iterator it 
		= groups_and_computers->begin();
		it != groups_and_computers->end(); it++)
	{
		if (it->second == computer_name)
		{
			return it;
		}
	}
	return groups_and_computers->end();
}




