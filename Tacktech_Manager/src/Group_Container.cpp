#include "Group_Container.h"
Group_Container::Group_Container()
{

}
Group_Container::~Group_Container()
{

}
 /** Function to add a group name to the Group_Container. Returns false
  ** if the group name already exists */
bool Group_Container::add_group_name( QString new_group_name)
{
	if (groups_and_computers.contains(new_group_name))
		return false;
	else
	{
		groups_and_computers.insert(new_group_name, "");
		return true;
	}
}

/** Function to add a computer name to the Group_Container. Returns false
 ** if the computer name is already in another group. */
bool Group_Container::add_computer_name(QString group_name,
	QString new_computer_name)
{
	bool computer_in_group = false;
	foreach(QString group, groups_and_computers.keys())
	{
		foreach(QString computer, groups_and_computers.values(group))
		{
			if (computer == new_computer_name)
				return false;
		}
	}
	groups_and_computers.remove(group_name, "");
	groups_and_computers.insert(group_name, new_computer_name);
	return true;
}

#ifdef _DEBUG
/** Function to print the contents of the Group_Container. Only compiled
 ** in debug mode */
void Group_Container::print_contents()
{
	std::cout << "= Group_Container::print_contents()" << std::endl;
	foreach(QString group, groups_and_computers.keys())
	{
		std::cout << " - " << qPrintable(group) << std::endl;
		foreach(QString computer, groups_and_computers.values(group))
		{
			std::cout << "  - " << qPrintable(computer) << std::endl;
		}
	}
}
#endif // _DEBUG

/** Returns the groups_and_computers map to the calling class */
QMultiMap<QString, QString> Group_Container::get_groups_and_computers()
{
	return groups_and_computers;
}


