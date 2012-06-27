#include "Tacktech_Manager.h"

Tacktech_Manager::Tacktech_Manager(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
#ifdef _DEBUG
	std::cout << "= Setting up Tacktech Manager" << std::endl;
	std::cout << " - Creating new edit_group_class" << std::endl;
#endif // _DEBUG

	/* Create new pointer to Edit_Group */
	edit_group_class = new Edit_Group();

#ifdef _DEBUG
	std::cout << " - Setting up UI" << std::endl;
#endif // _DEBUG
	ui.setupUi(this);

#ifdef _DEBUG
	std::cout << " - Setting management_tree_widget headers" << std::endl;
#endif // _DEBUG
	QStringList manager_headers;
	manager_headers << "Group" << "Playlist";
	ui.management_tree_widget->setHeaderLabels(manager_headers);
	
#ifdef _DEBUG
	std::cout << " - Connecting action signals" << std::endl;
#endif // _DEBUG
	/* Connect the main menu GUI signals */
	connect(ui.actionEdit_Group, SIGNAL(triggered()),
		this, SLOT(edit_group()));
	connect(ui.actionCreate_Playlist, SIGNAL(triggered()),
		this, SLOT(create_playlist()));
	connect(ui.actionEdit_Playlist, SIGNAL(triggered()),
		this, SLOT(edit_playlist()));
	connect(ui.actionEdit_Preferences, SIGNAL(triggered()),
		this, SLOT(edit_preferences()));

	/* Connecting secondany signals */
}

Tacktech_Manager::~Tacktech_Manager()
{
	delete edit_group_class;
}

/** Function will refresh all group statuses
 ** */
void Tacktech_Manager::refresh_all_groups()
{

}

/** Function will open the edit group GUI
 ** */
void Tacktech_Manager::edit_group()
{
#ifdef _DEBUG
	std::cout << " - Creating and showing Edit_Group GUI" << std::endl;
#endif // _DEBUG
	edit_group_class->set_group_names(group_names);
	edit_group_class->show();
}

/** Function will open the create playlist GUI
 ** */
void Tacktech_Manager::create_playlist()
{
#ifdef _DEBUG
	std::cout << " - Creating and showing Create_Playlist GUI" << std::endl;
#endif // _DEBUG
}

/** Function will open the edit playlist GUI
 ** */
void Tacktech_Manager::edit_playlist()
{
#ifdef _DEBUG
	std::cout << " - Creating and showing Edit_Playlist GUI" << std::endl;
#endif // _DEBUG
}

/** Function will open the edit preferences GUI
 ** */
void Tacktech_Manager::edit_preferences()
{
#ifdef _DEBUG
	std::cout << " - Creating and showing Edit_Preferences GUI" << std::endl;
#endif // _DEBUG
}

/** Slot to add group name to the global group_names variable. Validation
 ** happens here, and calls the add_group() slot again if the validation
 ** fails */
void Tacktech_Manager::add_group_input_complete( QString group_name )
{
#ifdef _DEBUG
	std::cout << " - Received from Add_Group: " 
		<< qPrintable(group_name) << std::endl;
	std::cout << " - Printing existing group names:" << std::endl;
	for(int i = 0; i < group_names.size(); i++)
		std::cout << "   - " << qPrintable(group_names[i]) << std::endl;
#endif // _DEBUG

	if (!group_names.contains(group_name))
	{
#ifdef _DEBUG
		std::cout << "  - Adding new group name:" 
			<< qPrintable(group_name) << std::endl;
#endif // _DEBUG
		group_names.append(group_name);
	}
	else
	{
#ifdef _DEBUG
		std::cout << "  - Group name already in list, recalling add_group()" 
			<< std::endl;
#endif // _DEBUG
	}
}
