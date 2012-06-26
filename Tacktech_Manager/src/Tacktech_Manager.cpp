#include "Tacktech_Manager.h"

Tacktech_Manager::Tacktech_Manager(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
#ifdef _DEBUG
	std::cout << "= Setting up Tacktech Manager" << std::endl;
#endif // _DEBUG

	/* Setting up global variables */
	add_group_window = new Add_Group();

	ui.setupUi(this);
	QStringList manager_headers;
	manager_headers << "Group" << "Playlist";
	ui.management_tree_widget->setHeaderLabels(manager_headers);

#ifdef _DEBUG
	std::cout << " - Connecting action signals" << std::endl;
#endif // _DEBUG
	/* Connect the main menu GUI signals */
	connect(ui.actionAdd_Group, SIGNAL(triggered()),
		this, SLOT(add_group()));
	connect(ui.actionEdit_Group, SIGNAL(triggered()),
		this, SLOT(edit_group()));
	connect(ui.actionCreate_Playlist, SIGNAL(triggered()),
		this, SLOT(create_playlist()));
	connect(ui.actionEdit_Playlist, SIGNAL(triggered()),
		this, SLOT(edit_playlist()));
	connect(ui.actionEdit_Preferences, SIGNAL(triggered()),
		this, SLOT(edit_preferences()));

	/* Connecting secondany signals */
	connect(add_group_window, SIGNAL(add_group_input_complete(QString)),
		this, SLOT(add_group_input_complete(QString)));
}

Tacktech_Manager::~Tacktech_Manager()
{
	delete add_group_window;
}

/** Function will refresh all group statuses
 ** */
void Tacktech_Manager::refresh_all_groups()
{

}

/** Function will open the add group GUI. Connects the Add_Group class's
 ** add_group_input_complete(QString) signal to this class's
 ** add_group_input_complete(QString) slot.
 ** */
void Tacktech_Manager::add_group()
{
	/* I actually suppose this disconnecting/reconnecting and 
	 * deleting/creating of pointers and signals are a bit of a hack, but
	 * it works at least */
#ifdef _DEBUG
	std::cout << " - Creating and showing Add_Group GUI" << std::endl;
#endif // _DEBUG
	/* Deleting the old pointer to add_group_window and disconnecting the
	 * signal from the old pointer */
	disconnect(add_group_window, SIGNAL(add_group_input_complete(QString)),
		this, SLOT(add_group_input_complete(QString)));
	delete add_group_window;

	/* Creating new pointer to add_group_window and reconnecting signal
	 * to the new pointer */
	add_group_window = new Add_Group();
	connect(add_group_window, SIGNAL(add_group_input_complete(QString)),
		this, SLOT(add_group_input_complete(QString)));
	add_group_window->show();
}

/** Function will open the edit group GUI
 ** */
void Tacktech_Manager::edit_group()
{
#ifdef _DEBUG
	std::cout << " - Creating and showing Edit_Group GUI" << std::endl;
#endif // _DEBUG
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
		add_group();
	}
}
