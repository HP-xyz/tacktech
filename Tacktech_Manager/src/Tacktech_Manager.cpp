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
	upload_data_dialog = new Upload_Data();

	groups_and_computers.reset(new Group_Container());
	playlist.reset(new Playlist_Container());
	group_playlist.reset(new Group_Playlist_Container());
	upload_data = new Upload_Data_Container();

    ui.setupUi(this);
	read_config();
	node_menu = new QMenu();
	ui.centralwidget->setContextMenuPolicy(Qt::ActionsContextMenu);

	/* Creating context menu actions */
	schedule_upload = new QAction(tr("Schedule Upload"), node_menu);

	/* Adding actions to context_menu */
	ui.centralwidget->addAction(schedule_upload);

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
	connect(schedule_upload, SIGNAL(triggered()),
		this, SLOT(show_schedule_upload_dialog()));

    /* Connecting secondary signals */
    connect(edit_group_class,
            SIGNAL(groups_changed()),
            this, SLOT(group_editing_complete()));
    connect(select_playlist_dialog,
            SIGNAL(group_playlist_changed()),
            this, SLOT(repopulate_widget()));
	connect(upload_data_dialog, SIGNAL(scheduled_item_added(QDate)),
		this, SLOT(scheduled_item_added(QDate)));
	connect(upload_data, SIGNAL(xml_creation_complete(std::string)),
		this, SLOT(start_upload(std::string)));

    read_variables_from_xml();
    repopulate_widget();
}

void Tacktech_Manager::read_config()
{
#ifdef _DEBUG
	std::cout << "= Tacktech_Manager::read_config()" << std::endl;
#endif // _DEBUG
	std::ifstream config("config.ini");
#ifdef _DEBUG
	std::cout << " - Created config ifstream" << std::endl;
#endif // _DEBUG
	if (!config)
	{
		std::cerr << " == Error -> No config found" << std::endl;
		ui.statusbar->showMessage("No config file 'config.ini' found");
	}
	else
	{
		ui.statusbar->showMessage("Config file 'config.ini' found");
	}

#ifdef _DEBUG
	std::cout << " - Creating options.insert" << std::endl;
#endif // _DEBUG
	options.insert("*");

	try
	{
#ifdef _DEBUG
		std::cout << "Config file read: " << std::endl <<
			"===================" << std::endl;
#endif
		for (boost::program_options::detail::config_file_iterator
			i(config, options), e; i != e; ++i)
		{
#ifdef _DEBUG
			std::cout << i->string_key << " " << i->value[0] << std::endl;
#endif
			parameters[i->string_key] = i->value[0];
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
		std::string error_msg;
		error_msg = "Config ERROR: ";
		error_msg += e.what();
		ui.statusbar->showMessage(error_msg.c_str());
	}
}

Tacktech_Manager::~Tacktech_Manager()
{
    save_variables_to_xml();
#ifdef _DEBUG
    std::cout << "= ~Tacktech_Manager" << std::endl;
#endif // _DEBUG
    delete edit_group_class;
    delete edit_playlist_class;
	delete select_playlist_dialog;
	delete upload_data_dialog;

	delete schedule_upload;
	delete node_menu;

	delete upload_data;
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
    playlist->print_contents();
#endif // _DEBUG
    ui.management_tree_widget->clear();
    for(int i = 0; i < group_playlist->get_group_playlist()->size(); i++)
    {
        Typed_QTreeWidgetItem *tree_item = new Typed_QTreeWidgetItem();
        tree_item->set_group_name(QString::fromStdString(group_playlist->
                           get_group_playlist()->at(i).first));
        tree_item->set_playlist_name(QString::fromStdString(group_playlist->
                           get_group_playlist()->at(i).second));
        tree_item->setText(0, QString::fromStdString(group_playlist->
                           get_group_playlist()->at(i).first));
        tree_item->setText(1, QString::fromStdString(group_playlist->
                           get_group_playlist()->at(i).second));
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
        {
            //Removing group name if needed
			Group_Playlist_Vector::iterator it =
				group_playlist->get_group_playlist()->begin() + i;
			group_playlist->get_group_playlist()->erase(it);
        }
    }
	Group_Multimap map = groups_and_computers->get_unique_groups();
	for(Group_Multimap::iterator it 
		= map.begin();
		it != map.end(); it++)
    {
        if(!group_playlist->contains_group_name(it->first))
        {
#ifdef _DEBUG
            std::cout << " - Adding: " << it->first
                      << std::endl;
#endif // _DEBUG
            std::pair<std::string, std::string> pair_to_add;
            pair_to_add.first = it->first;
            pair_to_add.second = "ADD PLAYLIST HERE";
            group_playlist->get_group_playlist()->push_back(pair_to_add);
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
 ** to file, XML encoded.
 ** NOTE: This function takes n^2 time to complete */
void Tacktech_Manager::save_variables_to_xml()
{
//#ifdef _DEBUG
//    std::cout << "= Tacktech_Manager::save_variables_to_xml()"
//              << std::endl;
//#endif // _DEBUG
//    /* Save playlist to file */
//    pugi::xml_document playlist_document;
//    pugi::xml_node root_node_playlist =
//        playlist_document.append_child("Playlist");
//    for (int i = 0; i < playlist->get_playlist().uniqueKeys().size(); i++)
//    {
//        pugi::xml_node playlist_node =
//            root_node_playlist.append_child("Playlist_Item");
//        playlist_node.append_attribute("Playlist_Name") =
//            playlist->get_playlist().uniqueKeys().at(i).toStdString()
//            .c_str();
//        for (int j = 0; j < playlist->get_playlist().values(
//                    playlist->get_playlist().uniqueKeys().at(i)).size(); j++)
//        {
//            pugi::xml_node item_node = playlist_node.append_child("Item");
//            pugi::xml_node filename_node = item_node.append_child(
//                                               "Filename");
//            pugi::xml_node pause_node = item_node.append_child(
//                                            "Pause");
//            pugi::xml_node filename_pcdata =
//                filename_node.append_child(pugi::node_pcdata);
//            pugi::xml_node pause_pcdata =
//                pause_node.append_child(pugi::node_pcdata);
//
//            filename_pcdata.set_value(playlist->get_playlist().values(
//                                          playlist->get_playlist().uniqueKeys().at(i))
//                                      .at(j).first.toStdString().c_str());
//            pause_pcdata.set_value(boost::lexical_cast<std::string>(
//                                       playlist->get_playlist().values(
//                                           playlist->get_playlist().uniqueKeys().at(i))
//                                       .at(j).second).c_str());
//        }
//    }
//    playlist_document.save_file("./playlist.xml");
//
//    /* Save groups_and_computers to file */
//    pugi::xml_document groups_and_computers_document;
//    pugi::xml_node root_node_groups_and_computers =
//        groups_and_computers_document.append_child("Groups_And_Computers");
//
//    for (int i = 0;
//            i < groups_and_computers->
//            get_groups_and_computers().uniqueKeys().size(); i++)
//    {
//        pugi::xml_node group_node = root_node_groups_and_computers
//                                    .append_child("Group_Item");
//        group_node.append_attribute("Group_Name") =
//            groups_and_computers->get_groups_and_computers().uniqueKeys()
//            .at(i).toStdString().c_str();
//        for (int j = 0;
//                j < groups_and_computers->
//                get_groups_and_computers().values(
//                    groups_and_computers->get_groups_and_computers()
//                    .uniqueKeys().at(i)).size(); j++)
//        {
//            pugi::xml_node computer_item_node = group_node.append_child("Computer_Item");
//            pugi::xml_node computer_node = computer_item_node.append_child(
//                                               "Computer");
//            pugi::xml_node computer_pcdata =
//                computer_node.append_child(pugi::node_pcdata);
//            computer_pcdata.set_value(
//                groups_and_computers->get_groups_and_computers().values(
//                    groups_and_computers->get_groups_and_computers()
//                    .uniqueKeys().at(i)).at(j).toStdString().c_str());
//        }
//    }
//    groups_and_computers_document.save_file("./groups_and_computers.xml");
//
//    /* Save group_playlist to file */
//    pugi::xml_document group_playlist_document;
//    pugi::xml_node root_node_group_playlist =
//        group_playlist_document.append_child("Group_Playlist");
//
//    for (int i = 0; i < group_playlist->get_group_playlist()->size(); i++)
//    {
//        pugi::xml_node group_playlist_node =
//            root_node_group_playlist.append_child("Group_Playlist_Item");
//        pugi::xml_node group_name_node =
//            group_playlist_node.append_child("Group_Name");
//        pugi::xml_node playlist_name_node =
//            group_playlist_node.append_child("Playlist_Name");
//        pugi::xml_node group_name_pcdata =
//            group_name_node.append_child(pugi::node_pcdata);
//        pugi::xml_node playlist_name_pcdata =
//            playlist_name_node.append_child(pugi::node_pcdata);
//        group_name_pcdata.set_value(
//            group_playlist->get_group_playlist()->
//            at(i).first.toStdString().c_str());
//        playlist_name_pcdata.set_value(
//            group_playlist->get_group_playlist()->
//            at(i).second.toStdString().c_str());
//    }
//    group_playlist_document.save_file("./group_playlist.xml");
}

/** Read the values for variables groups_and_computers, playlists and
 ** group playlist from file, XML encoded */
void Tacktech_Manager::read_variables_from_xml()
{
//#ifdef _DEBUG
//    std::cout << "= Tacktech_Manager::read_variables_from_xml()"
//              << std::endl;
//#endif // _DEBUG
//    /* Read playlist from file */
//    pugi::xml_document playlist_document;
//    playlist_document.load_file("./playlist.xml");
//    pugi::xml_node root_node = playlist_document.child("Playlist");
//#ifdef _DEBUG
//    std::cout << " - Root Item:" << std::endl;
//    root_node.print(std::cout);
//#endif // _DEBUG
//    for (pugi::xml_node playlist_item = root_node.child("Playlist_Item");
//            playlist_item;
//            playlist_item = playlist_item.next_sibling("Playlist_Item"))
//    {
//#ifdef _DEBUG
//        std::cout << " - Playlist Item:" << std::endl;
//        playlist_item.print(std::cout);
//#endif // _DEBUG
//        for(pugi::xml_node item_node = playlist_item.child("Item");
//                item_node;
//                item_node = item_node.next_sibling("Item"))
//        {
//            playlist->add_filename(
//                playlist_item.attribute("Playlist_Name").as_string(),
//                item_node.child_value("Filename"),
//                boost::lexical_cast<int>(item_node.child_value("Pause")));
//        }
//    }
//
//    /* Read groups_and_computers from file */
//    pugi::xml_document groups_and_computers_document;
//    groups_and_computers_document.load_file("./groups_and_computers.xml");
//    root_node = groups_and_computers_document.child("Groups_And_Computers");
//#ifdef _DEBUG
//    std::cout << " - Root Item:" << std::endl;
//    root_node.print(std::cout);
//#endif // _DEBUG
//    for (pugi::xml_node group_item = root_node.child("Group_Item");
//            group_item;
//            group_item = group_item.next_sibling("Group_Item"))
//    {
//#ifdef _DEBUG
//// TODO (HP#1#):
//        std::cout << " - Group Item:" << std::endl;
//        group_item.print(std::cout);
//#endif // _DEBUG
//        for (pugi::xml_node computer_item = group_item.child("Computer_Item");
//                computer_item;
//                computer_item = computer_item.next_sibling("Computer_Item"))
//        {
//            groups_and_computers->add_computer_name(
//                group_item.attribute("Group_Name").as_string(),
//                computer_item.child_value("Computer"));
//        }
//    }
//
//    /* Now we have to tell the class to repopulate the group_playlist.
//     * This is done by calling the group_editing_complete slot. */
//    group_editing_complete();
//
//    /* Read group_playlist from file */
//    pugi::xml_document group_playlist_document;
//    group_playlist_document.load_file("./group_playlist.xml");
//    root_node = group_playlist_document.child("Group_Playlist");
//#ifdef _DEBUG
//    std::cout << " - Root Item:" << std::endl;
//    root_node.print(std::cout);
//#endif // _DEBUG
//    for (pugi::xml_node group_playlist_item =
//                root_node.child("Group_Playlist_Item");
//            group_playlist_item;
//            group_playlist_item =
//                group_playlist_item.next_sibling("Group_Playlist_Item"))
//    {
//#ifdef _DEBUG
//        std::cout << " - Group_Playlist Item:" << std::endl;
//        group_playlist_item.print(std::cout);
//#endif // _DEBUG
//        group_playlist->connect_group_to_playlist(
//            group_playlist_item.child_value("Group_Name"),
//            group_playlist_item.child_value("Playlist_Name"));
//    }
}

void Tacktech_Manager::scheduled_item_added( QDate date)
{
#ifdef _DEBUG
	std::cout << "= Tacktech_Manager::scheduled_item_added()" << std::endl;
#endif // _DEBUG

	Typed_QTreeWidgetItem *selected_item =
		static_cast<Typed_QTreeWidgetItem*>
		(ui.management_tree_widget->selectedItems().at(0));
	upload_data->set_group_name(selected_item->get_group_name());
	upload_data->set_groups(groups_and_computers);
	upload_data->set_playlist(playlist);
	upload_data->set_playlist_name(selected_item->get_playlist_name());
	upload_data->set_upload_time(date.toString(Qt::ISODate));
	upload_data->get_xml_upload();
}

void Tacktech_Manager::show_schedule_upload_dialog()
{
	upload_data_dialog->show();
}

//TODO
//CHECK PARAMETER COPY/POINTER etc
void Tacktech_Manager::start_upload( std::string xml_string)
{
#ifdef _DEBUG
	std::cout << "= Tacktech_Manager::start_upload()" << std::endl;
#endif // _DEBUG
	std::string status_msg;
	status_msg += "Sending to: ";
	status_msg += parameters["general.server_ip"].c_str();
	status_msg += ":";
	status_msg += parameters["general.server_port"];
	ui.statusbar->showMessage(status_msg.c_str());

	Send_Data *send_data = new Send_Data(parameters["general.server_ip"].c_str(),
		boost::lexical_cast<int>(parameters["general.server_port"]),
		xml_string);
#ifdef _DEBUG
	std::cout << " - Upload thread execution started" << std::endl;
#endif // _DEBUG

}

