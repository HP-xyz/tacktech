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

	/* Create new pointer for global variable to groups_and_computers*/
	groups_and_computers = new Group_Container();

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

	/* Connecting secondary signals */
	connect(edit_group_class,
		SIGNAL(editing_complete()),
		this, SLOT(group_editing_complete()));
}

Tacktech_Manager::~Tacktech_Manager()
{
	delete edit_group_class;
	delete groups_and_computers;
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
	edit_group_class->set_groups_and_computer_names(groups_and_computers);
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

/** Slot called from the called class to set the group_and_computers to the
 ** current values. Gets called if the edit_group class object sends the accepted
 ** signal from its buttonbox */
void Tacktech_Manager::group_editing_complete()
{
	//TODO
	//REDRAW GUI
}

