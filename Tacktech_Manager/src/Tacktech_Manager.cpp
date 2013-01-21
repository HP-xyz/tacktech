#include "Tacktech_Manager.h"

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

Tacktech_Manager::Tacktech_Manager(QWidget *parent, Qt::WFlags flags) :
		QMainWindow(parent, flags)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Setting up Tacktech Manager" << std::endl;
#endif // _DEBUG

	edit_group_class = new Edit_Group();
	edit_playlist_class = new Edit_Playlist();
	select_playlist_dialog = new Select_Playlist_Dialog();
	upload_data_dialog = new Upload_Data();

	groups_and_computers.reset(new Group_Container());
	playlist.reset(new Playlist_Container());
	group_playlist.reset(new Group_Playlist_Container());

	ui.setupUi(this);
	read_config();

	io_service.reset(new boost::asio::io_service);
	network_manager.reset(
		new Tacktech_Network_Manager(*io_service, parameters));

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
	connect(ui.actionEdit_Group, SIGNAL(triggered()), this, SLOT(edit_group()));
	connect(ui.actionCreate_Playlist, SIGNAL(triggered()), this,
			SLOT(create_playlist()));
	connect(ui.actionEdit_Playlist, SIGNAL(triggered()), this,
			SLOT(edit_playlist()));
	connect(ui.actionEdit_Preferences, SIGNAL(triggered()), this,
			SLOT(edit_preferences()));
	connect(ui.management_tree_widget,
			SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this,
			SLOT(show_playlist_selection(QTreeWidgetItem*, int)));
	connect(ui.actionRefresh_All, SIGNAL(triggered()), this,
			SLOT(refresh_all_request()));
	connect(schedule_upload, SIGNAL(triggered()), this,
			SLOT(show_schedule_upload_dialog()));
	connect(ui.actionSend_Data_to_Server, SIGNAL(triggered()), this,
			SLOT(upload_new_variables_slot()));

	/* Connecting secondary signals */
	connect(edit_group_class, SIGNAL(groups_changed()), this,
			SLOT(group_editing_complete()));
	connect(select_playlist_dialog, SIGNAL(group_playlist_changed()), this,
			SLOT(repopulate_widget()));
	connect(upload_data_dialog, SIGNAL(scheduled_item_added(QDate)), this,
			SLOT(scheduled_item_added(QDate)));

	repopulate_widget();
}

void Tacktech_Manager::read_config()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Tacktech_Manager::read_config()" << std::endl;
#endif // _DEBUG
	std::ifstream config("config.ini");
#ifdef _SHOW_DEBUG_OUTPUT
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

#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Creating options.insert" << std::endl;
#endif // _DEBUG
	options.insert("*");

	try
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << "Config file read: " << std::endl << "==================="
				<< std::endl;
#endif
		for (boost::program_options::detail::config_file_iterator i(config,
				options), e; i != e; ++i)
		{
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << i->string_key << " " << i->value[0] << std::endl;
#endif
			parameters[i->string_key] = i->value[0];
		}
	} catch (std::exception& e)
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
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= ~Tacktech_Manager" << std::endl;
#endif // _DEBUG
	delete edit_group_class;
	delete edit_playlist_class;
	delete select_playlist_dialog;
	delete upload_data_dialog;

	delete schedule_upload;
	delete node_menu;
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
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Creating and showing Edit_Group GUI" << std::endl;
#endif // _DEBUG
	edit_group_class->set_groups_and_computer_names(groups_and_computers);
	edit_group_class->show();
}

/** Function will open the create playlist GUI
 ** */
void Tacktech_Manager::create_playlist()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Creating and showing Create_Playlist GUI" << std::endl;
#endif // _DEBUG
}

/** Function will open the edit playlist GUI
 ** */
void Tacktech_Manager::edit_playlist()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Creating and showing Edit_Playlist GUI" << std::endl;
#endif // _DEBUG
	edit_playlist_class->set_playlist(playlist);
	edit_playlist_class->show();
}

/** Function will open the edit preferences GUI
 ** */
void Tacktech_Manager::edit_preferences()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Creating and showing Edit_Preferences GUI" << std::endl;
#endif // _DEBUG
}

/** Slot to repopulate the tree widget.
 ** NOTE: Takes linear time to complete */
void Tacktech_Manager::repopulate_widget()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Tacktech_Manager::repopulate_widget()" << std::endl;
	playlist->print_contents();
#endif // _DEBUG
	ui.management_tree_widget->clear();
	for (unsigned int i = 0; i < group_playlist->get_group_playlist()->size(); i++)
	{
		Typed_QTreeWidgetItem *tree_item = new Typed_QTreeWidgetItem();
		tree_item->set_group_name(
				QString::fromStdString(
						group_playlist->get_group_playlist()->at(i).first));
		tree_item->set_playlist_name(
				QString::fromStdString(
						group_playlist->get_group_playlist()->at(i).second));
		tree_item->setText(0,
				QString::fromStdString(
						group_playlist->get_group_playlist()->at(i).first));
		tree_item->setText(1,
				QString::fromStdString(
						group_playlist->get_group_playlist()->at(i).second));
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
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Tacktech_Manager::group_editing_complete()" << std::endl;
#endif // _DEBUG
	for (unsigned int i = 0; i < group_playlist->get_group_playlist()->size(); i++)
	{
		if (!groups_and_computers->contains_group_name(
				group_playlist->get_group_playlist()->at(i).first))
		{
			//Removing group name if needed
			Group_Playlist_Vector::iterator it =
					group_playlist->get_group_playlist()->begin() + i;
			group_playlist->get_group_playlist()->erase(it);
		}
	}
	Group_Multimap map = groups_and_computers->get_unique_groups();
	for (Group_Multimap::iterator it = map.begin(); it != map.end(); it++)
	{
		if (!group_playlist->contains_group_name(it->first))
		{
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << " - Adding: " << it->first << std::endl;
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
void Tacktech_Manager::show_playlist_selection(QTreeWidgetItem* selected_item,
		int)
{
	select_playlist_dialog->set_group_playlist(group_playlist);
	select_playlist_dialog->set_selected_group(selected_item->text(0));
	select_playlist_dialog->set_playlist(playlist);
	select_playlist_dialog->show();
}

void Tacktech_Manager::scheduled_item_added(QDate date)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Tacktech_Manager::scheduled_item_added()" << std::endl;
#endif // _DEBUG
	upload_data.reset(new Upload_Data_Container(parameters));
	connect(upload_data.get(), SIGNAL(xml_creation_complete(std::string)), this,
			SLOT(start_upload(std::string)));
	Typed_QTreeWidgetItem *selected_item =
			static_cast<Typed_QTreeWidgetItem*>(ui.management_tree_widget->selectedItems().at(
					0));
	upload_data->set_command("UPLOAD");
	upload_data->set_group_name(selected_item->get_group_name());
	upload_data->set_playlist(playlist);
	upload_data->set_groups(groups_and_computers);
	upload_data->set_group_playlist(group_playlist);
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
void Tacktech_Manager::start_upload(std::string xml_string)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Tacktech_Manager::start_upload()" << std::endl;
#endif // _DEBUG
	std::string status_msg;
	status_msg += "Sending to: ";
	status_msg += parameters["general.server_ip"].c_str();
	status_msg += ":";
	status_msg += parameters["general.server_port"];
	ui.statusbar->showMessage(status_msg.c_str());

	boost::shared_ptr<std::string> string_to_send;
	string_to_send.reset(new std::string(xml_string));

	io_service->reset();
	network_manager->connect(parameters["general.server_ip"],
			parameters["general.server_port"]);
	connect(network_manager.get(), SIGNAL(data_recieved(QString)), this,
				SLOT(data_recieved_slot(QString)));
	network_manager->start_write(string_to_send);
	boost::thread t(
			boost::bind(&boost::asio::io_service::run, boost::ref(io_service)));
	t.join();
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Upload thread execution started" << std::endl;
#endif // _DEBUG
}

void Tacktech_Manager::refresh_all_request()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Tacktech_Manager::refresh_all_request()" << std::endl;
#endif // _DEBUG
	upload_data.reset(new Upload_Data_Container(parameters));
	connect(upload_data.get(), SIGNAL(xml_creation_complete(std::string)), this,
			SLOT(start_upload(std::string)));
	upload_data->set_playlist(playlist);
	upload_data->set_groups(groups_and_computers);
	upload_data->set_group_playlist(group_playlist);
	upload_data->set_command("GET_VARIABLES");
	upload_data->get_xml_upload();
}

void Tacktech_Manager::data_recieved_slot(QString data_recieved)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Tacktech_Manager::data_recieved_slot()" << std::endl;
	std::cout << " - Data Received Size: " <<
		data_recieved.toStdString().length() << std::endl;
	std::cout << data_recieved.toStdString() << std::endl;
#endif // _DEBUG
	std::string status_msg;
	pugi::xml_document document;
	document.load(data_recieved.toStdString().c_str());
	std::string type_string =
			document.child("Tacktech").child("Type").attribute("TYPE").as_string();
	if (type_string == "SET_VARIABLES")
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Received SET_VARIABLES command" << std::endl;
#endif // _DEBUG
		/* We declare 3 strings that contain the variables */
		std::string groups_and_computers_str = data_recieved.toStdString();
		std::string playlist_str = data_recieved.toStdString();
		std::string group_playlist_str = data_recieved.toStdString();

		/* Now substring the recieved data, and assign only the needed data
		 * to the appropraite string */
		playlist_str = playlist_str.substr(
			playlist_str.find("<PLAYLIST_NODE>"),
			playlist_str.rfind("</PLAYLIST_NODE>") + 16);
		groups_and_computers_str = group_playlist_str.substr(
			groups_and_computers_str.find("<GROUPS_AND_COMPUTERS_NODE"),
			groups_and_computers_str.rfind("</GROUPS_AND_COMPUTERS_NODE>") + 28);
		group_playlist_str = group_playlist_str.substr(
			group_playlist_str.find("<GROUPS_PLAYLIST_NODE>"),
			group_playlist_str.rfind("</GROUPS_PLAYLIST_NODE>") + 23);

		pugi::xml_document playlist_doc;
		playlist_doc.load(playlist_str.c_str());
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Playlist RECIEVED Print: " << std::endl;
		std::cout << "==================" << std::endl;
		playlist_doc.print(std::cout);
#endif

		pugi::xml_document groups_and_computers_doc;
		groups_and_computers_doc.load(groups_and_computers_str.c_str());
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Groups_And_Computers RECIEVED Print: " << std::endl;
		std::cout << "==================" << std::endl;
		groups_and_computers_doc.print(std::cout);
#endif

		pugi::xml_document group_playlist_doc;
		group_playlist_doc.load(group_playlist_str.c_str());
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Group_Playlist RECIEVED Print: " << std::endl;
		std::cout << "==================" << std::endl;
		group_playlist_doc.print(std::cout);
#endif

		xml_string_writer playlist_writer;
		playlist_doc.child("PLAYLIST_NODE").
			child("Playlist").print(playlist_writer);
		playlist->reset_container();
		playlist->construct_playlist(playlist_writer.result);
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Playlist Print: " << std::endl;
		std::cout << "==================" << std::endl;
		playlist->print_contents();
#endif
		xml_string_writer groups_and_computers_writer;
		groups_and_computers_doc.child("GROUPS_AND_COMPUTERS_NODE")
			.child("Groups_And_Computers")
			.print(groups_and_computers_writer);
		groups_and_computers->reset_container();
		groups_and_computers->construct_groups_and_computers(
				groups_and_computers_writer.result);
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Groups_And_Computers Print: " << std::endl;
		std::cout << "===============================" << std::endl;
		groups_and_computers->print_contents();
#endif

		xml_string_writer group_playlist_writer;
		group_playlist_doc.child("GROUPS_PLAYLIST_NODE")
			.child("Group_Playlist").print(	group_playlist_writer);
		group_playlist->reset_container();
		group_playlist->construct_group_playlist(
			group_playlist_writer.result);
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - group_playlist Print: " << std::endl;
		std::cout << "=========================" << std::endl;
		groups_and_computers->print_contents();
#endif
		status_msg += "Received variables successfully";
	}
	else if(type_string == "UPLOAD_RESULT")
	{
		std::string succes_string =
			document.child("Success").attribute("SUCCESS").as_string();
		if (succes_string == "TRUE")
		{
			status_msg += "Uploaded successfully to server";
		}
		else
		{
			status_msg += "Uploaded failed to server";
		}
	}
	ui.statusbar->showMessage(status_msg.c_str());
	repopulate_widget();
}

void Tacktech_Manager::upload_new_variables_slot()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Tacktech_Manager::upload_new_variables_slot()" << std::endl;
#endif // _DEBUG
	upload_data.reset(new Upload_Data_Container(parameters));
	connect(upload_data.get(), SIGNAL(xml_creation_complete(std::string)),
		this, SLOT(start_upload(std::string)));
	upload_data->set_playlist(playlist);
	upload_data->set_groups(groups_and_computers);
	upload_data->set_group_playlist(group_playlist);
	upload_data->set_command("SET_VARIABLES");
	upload_data->get_xml_upload();
}

