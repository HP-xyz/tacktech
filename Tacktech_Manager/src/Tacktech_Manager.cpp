#include "Tacktech_Manager.h"

Tacktech_Manager::Tacktech_Manager(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
#ifdef _DEBUG
	std::cout << "= Setting up Tacktech Manager" << std::endl;
#endif // _DEBUG

	edit_group_class = new Edit_Group();
	edit_playlist_class = new Edit_Playlist();
	select_playlist_dialog = new Select_Playlist_Dialog();

	groups_and_computers = new Group_Container();
	playlist = new Playlist_Container();
	group_playlist = new Group_Playlist_Container();

	ui.setupUi(this);

	QStringList manager_headers;
	manager_headers << "Group" << "Playlist";
	ui.management_tree_widget->setHeaderLabels(manager_headers);
	
	/* Connect the main menu GUI signals */
	connect(ui.actionEdit_Group, SIGNAL(triggered()),
		this, SLOT(edit_group()));
	connect(ui.actionCreate_Playlist, SIGNAL(triggered()),
		this, SLOT(create_playlist()));
	connect(ui.actionEdit_Playlist, SIGNAL(triggered()),
		this, SLOT(edit_playlist()));
	connect(ui.actionEdit_Preferences, SIGNAL(triggered()),
		this, SLOT(edit_preferences()));
	connect(ui.management_tree_widget,
		SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
		this, SLOT(show_playlist_selection(QTreeWidgetItem*, int)));

	/* Connecting secondary signals */
	connect(edit_group_class,
		SIGNAL(groups_changed()),
		this, SLOT(group_editing_complete()));
	connect(select_playlist_dialog,
		SIGNAL(group_playlist_changed()),
		this, SLOT(repopulate_widget()));
}

Tacktech_Manager::~Tacktech_Manager()
{
	save_variables_to_xml();
#ifdef _DEBUG
	std::cout << "= ~Tacktech_Manager" << std::endl;
#endif // _DEBUG
	delete edit_group_class;
	delete edit_playlist_class;
	delete groups_and_computers;
	delete playlist;
	delete group_playlist;
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
	edit_playlist_class->set_playlist(playlist);
	edit_playlist_class->show();
}

/** Function will open the edit preferences GUI
 ** */
void Tacktech_Manager::edit_preferences()
{
#ifdef _DEBUG
	std::cout << " - Creating and showing Edit_Preferences GUI" 
		<< std::endl;
#endif // _DEBUG
}

/** Slot to repopulate the tree widget. 
 ** NOTE: Takes linear time to complete */
void Tacktech_Manager::repopulate_widget()
{
#ifdef _DEBUG
	std::cout << "= Tacktech_Manager::repopulate_widget()" << std::endl;
#endif // _DEBUG
	ui.management_tree_widget->clear();
	for(int i = 0; i < group_playlist->get_group_playlist()->size(); i++)
	{
		QTreeWidgetItem *tree_item = new QTreeWidgetItem();
		tree_item->setText(0, group_playlist->
			get_group_playlist()->at(i).first);
		tree_item->setText(1, group_playlist->
			get_group_playlist()->at(i).second);
		ui.management_tree_widget->addTopLevelItem(tree_item);
	}
}
 /** Slot fired when the add_group_dialog sends its groups_changed()
  ** signal. Adds the groups that have been added to the group_playlist
  ** list and removes the ones that have been removed.
  ** NOTE: This function could takes at least linear time to complete, 
  **		and may take up to n^2 time to complete */
void Tacktech_Manager::group_editing_complete()
{
#ifdef _DEBUG
	std::cout << "= Tacktech_Manager::group_editing_complete()" 
		<< std::endl;
#endif // _DEBUG
	for(int i = 0; i < group_playlist->get_group_playlist()->size(); i++)
	{
		if(!groups_and_computers->contains_group_name(
			group_playlist->get_group_playlist()->at(i).first))
		{//Removing group name if needed
			group_playlist->get_group_playlist()->removeAt(i);
		}
	}
	foreach(QString group_name,
		groups_and_computers->get_groups_and_computers().uniqueKeys())
	{
		if(!group_playlist->contains_group_name(group_name))
		{
#ifdef _DEBUG
			std::cout << " - Adding: " << qPrintable(group_name) 
				<< std::endl;
#endif // _DEBUG
			QPair<QString, QString> pair_to_add;
			pair_to_add.first = group_name;
			pair_to_add.second = "ADD PLAYLIST HERE";
			group_playlist->get_group_playlist()->append(pair_to_add);
		}
	}
	repopulate_widget();
}

/** Slot to show the playlist selection GUI. */
void Tacktech_Manager::show_playlist_selection(
	QTreeWidgetItem* selected_item, int )
{
	select_playlist_dialog->set_group_playlist(group_playlist);
	select_playlist_dialog->set_selected_group(selected_item->text(0));
	select_playlist_dialog->set_playlist(playlist);
	select_playlist_dialog->show();
}

/** Saves the playlist, groups_and_computers and group_playlist variables
 ** to file, XML encoded */
void Tacktech_Manager::save_variables_to_xml()
{
	/* Save playlist to file */
	pugi::xml_document playlist_document;
	pugi::xml_node root_node_playlist =
		playlist_document.append_child("Playlist");
	for (int i = 0; i < playlist->get_playlist().uniqueKeys().size(); i++)
	{
		pugi::xml_node playlist_node = root_node_playlist.append_child(
			playlist->get_playlist().uniqueKeys().at(i).toStdString()
			.c_str());
		for (int j = 0; j < playlist->get_playlist().values(
			playlist->get_playlist().uniqueKeys().at(i)).size(); j++)
		{
			pugi::xml_node filename_node = playlist_node.append_child(
				"Filename");
			filename_node.append_attribute("Filename") = 
				playlist->get_playlist().values(
				playlist->get_playlist().uniqueKeys().at(i))
				.at(j).first.toStdString().c_str();
			filename_node.append_attribute("Pause") = 
				playlist->get_playlist().values(
				playlist->get_playlist().uniqueKeys().at(i))
				.at(j).second;
		}
	}
	playlist_document.save_file("./playlist.xml");

	/* Save groups_and_computers to file */
	pugi::xml_document groups_and_computers_document;
	pugi::xml_node root_node_groups_and_computers =
		groups_and_computers_document.append_child("Groups And Computers");

	for (int i = 0;
		i < groups_and_computers->
		get_groups_and_computers().uniqueKeys().size(); i++)
	{
		pugi::xml_node group_node = root_node_groups_and_computers
			.append_child(
			groups_and_computers->get_groups_and_computers().uniqueKeys()
			.at(i).toStdString()
			.c_str());

		for (int j = 0;
			j < groups_and_computers->
			get_groups_and_computers().values(
			groups_and_computers->get_groups_and_computers()
			.uniqueKeys().at(i)).size(); j++)
		{
			pugi::xml_node computer_node = group_node.append_child(
				"Computer");
			computer_node.append_attribute("Hostname") =
				groups_and_computers->get_groups_and_computers().values(
				groups_and_computers->get_groups_and_computers()
				.uniqueKeys().at(i)).at(j).toStdString().c_str();
		}
	}
	groups_and_computers_document.save_file("./groups_and_computers.xml");

	/* Save group_playlist to file */
	pugi::xml_document group_playlist_document;
	pugi::xml_node root_node_group_playlist =
		group_playlist_document.append_child("Group Playlist");

	for (int i = 0; i < group_playlist->get_group_playlist()->size(); i++)
	{
		pugi::xml_node group_playlist_node =
			root_node_group_playlist.append_child("Group Playlist Node");
		group_playlist_node.append_attribute("Group Name") =
			group_playlist->get_group_playlist()->
			at(i).first.toStdString().c_str();
		group_playlist_node.append_attribute("Playlist Name") =
			group_playlist->get_group_playlist()->
			at(i).second.toStdString().c_str();
	}
	group_playlist_document.save_file("./group_playlist.xml");
}

