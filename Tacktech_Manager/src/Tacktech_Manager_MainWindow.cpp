#include "Tacktech_Manager_MainWindow.h"

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

Tacktech_Manager_MainWindow::Tacktech_Manager_MainWindow( QWidget *parent /*= 0*/, Qt::WFlags flags /*= 0*/ )
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Tacktech_Manager_MainWindow" << std::endl;
#endif // _DEBUG
	display_client_container.reset(new Display_Client_Container());
	file_upload_dialog.reset(new Upload_Files_To_Server_Dialog());
	pending_uploads.reset(new std::vector<std::pair<std::vector<std::string>, QTime> >);
	edit_playlist_dialog.reset(new Edit_Playlist());

	ui.setupUi(this);
	read_config();

	node_menu.reset(new QMenu());
	ui.main_tree_widget->setContextMenuPolicy(Qt::ActionsContextMenu);
	add_to_group.reset(new QAction("Add to group...", node_menu.get()));
	edit_playlist.reset(new QAction("Edit Playlist...", node_menu.get()));

	ui.main_tree_widget->addAction(add_to_group.get());
	ui.main_tree_widget->addAction(edit_playlist.get());

	io_service.reset(new boost::asio::io_service);
	network_manager.reset(
		new Tacktech_Network_Manager(*io_service, parameters));
	connect(network_manager.get(), SIGNAL(data_recieved(QString)), this,
		SLOT(data_recieved_slot(QString)));
	/** Start network_manager thread */

	QStringList manager_headers;
	manager_headers << "Computer Name" << "Group" << "Last Ping" << "Current Playing Item";
	ui.main_tree_widget->setHeaderLabels(manager_headers);
	refresh_timer = new QTimer(this);

	connect(refresh_timer, SIGNAL(timeout()), this, SLOT(refresh_all_request()));
	connect(ui.actionUpload_Files_To_Server, SIGNAL(triggered()), this, SLOT(show_upload_gui()));
	connect(file_upload_dialog.get(), SIGNAL(selection_complete(std::vector<std::string>, QTime)),
		this, SLOT(upload_files_to_server( std::vector<std::string>, QTime)));
	connect(add_to_group.get(), SIGNAL(triggered()), this, SLOT(assign_group()));
	connect(edit_playlist.get(), SIGNAL(triggered()), this, SLOT(edit_playlist_slot()));
	refresh_timer->start(30000);
}

void Tacktech_Manager_MainWindow::read_config()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " =Tacktech_Manager_MainWindow::read_config()" << std::endl;
#endif // _DEBUG
	std::ifstream config("config.ini");
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "  - Created config ifstream" << std::endl;
#endif // _DEBUG
	if (!config)
	{
		std::cerr << "  == Error -> No config found" << std::endl;
		ui.statusbar->showMessage("No config file 'config.ini' found");
	}
	else
	{
		ui.statusbar->showMessage("Config file 'config.ini' found");
	}

#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "  - Creating options.insert" << std::endl;
#endif // _DEBUG
	options.insert("*");

	try
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << "  - Config file read: " << std::endl << "   ==================="
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
		std::cerr << "  - Exception: " << e.what() << std::endl;
		std::string error_msg;
		error_msg = "Config ERROR: ";
		error_msg += e.what();
		ui.statusbar->showMessage(error_msg.c_str());
	}
}


Tacktech_Manager_MainWindow::~Tacktech_Manager_MainWindow(void)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=~Tacktech_Manager_MainWindow" << std::endl;
#endif // _DEBUG
	delete refresh_timer;
}

void Tacktech_Manager_MainWindow::repopulate_ui()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " =Tacktech_Manager_MainWindow::repopulate_ui" << std::endl;
#endif // _DEBUG
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "  - Clearing the main_tree_widget"
		<< std::endl;
#endif // _DEBUG
	ui.main_tree_widget->clear();

	if(display_client_container.get() != 0)
	{
		for (std::vector<Display_Client_Ptr>::iterator it = display_client_container->get_display_client_container()->begin();
			it != display_client_container->get_display_client_container()->end(); ++it)
		{
			/* Check if Display_Client's groups are empty, saving 2 nested loops
			 * if it is indeed empty :) */
			if (it->get()->get_groups()->size() == 0)
			{
				int i = 0;
				bool group_none_found = false;
				while (i < ui.main_tree_widget->topLevelItemCount() && !group_none_found)
				{
					if (static_cast<Typed_QTreeWidgetItem*>(ui.main_tree_widget->topLevelItem(i))->get_group_name() == "NONE")
					{
						group_none_found = true;
					}
					++i;
				}
				/* If the NONE group already exists, create a pointer to it, else
				 * create a pointer to a new Typed_QTreeWidgetItem */
				Typed_QTreeWidgetItem *group_item;
				if (group_none_found)
					group_item = static_cast<Typed_QTreeWidgetItem*>(ui.main_tree_widget->topLevelItem(i));
				else
					group_item = new Typed_QTreeWidgetItem();
				group_item->set_group_name("NONE");
				group_item->setText(1, "NONE");
				ui.main_tree_widget->addTopLevelItem(group_item);

				Typed_QTreeWidgetItem *computer_item = new Typed_QTreeWidgetItem();
				computer_item->set_computer_name(QString::fromStdString(it->get()->get_identification()));
				computer_item->set_group_name("NONE");

				/** Here we get the elapsed time between now, and the last ping the
				* remote screen has answered */
				boost::posix_time::time_duration duration =
					boost::posix_time::second_clock::universal_time()
					- it->get()->get_last_ping();
				std::string str_duration = boost::posix_time::to_simple_string(duration);

				/* Set actual data to display */
				computer_item->setText(0, computer_item->get_computer_name());
				computer_item->setText(2, QString::fromStdString(str_duration));
				computer_item->setText(3, QString::fromStdString(
					it->get()->get_playlist_container()->get_current_playing_item()));

				/* Last thing to do to the group item, we get the Playlist_Container
					* name from the item we just added. */
				group_item->set_playlist_name(
					QString::fromStdString(
					it->get()->get_playlist_container()->get_playlist_container_name()));
				group_item->setText(3, group_item->get_playlist_name());

				group_item->addChild(computer_item);
			}
			else
			{
				for (std::set<std::string>::iterator it2 = it->get()->get_groups()->begin();
					it2 != it->get()->get_groups()->end(); ++it2)
				{
					bool tree_contains_group = false;
					int i = 0;
					while(!tree_contains_group && i < ui.main_tree_widget->topLevelItemCount())
					{
						if (static_cast<Typed_QTreeWidgetItem*>(ui.main_tree_widget->topLevelItem(i))->get_group_name().toStdString()
							== *it2)
						{//There already exists a Group TopLevelItem, so add to its child
							Typed_QTreeWidgetItem *computer_item = new Typed_QTreeWidgetItem();
							computer_item->set_computer_name(QString::fromStdString(it->get()->get_identification()));
							computer_item->set_group_name(QString::fromStdString(*it2));
							/** Here we get the elapsed time between now, and the last ping the
							* remote screen has answered */
							boost::posix_time::time_duration duration =
								boost::posix_time::second_clock::universal_time()
								- it->get()->get_last_ping();
							std::string str_duration = boost::posix_time::to_simple_string(duration);

							/* Set actual data to display */
							computer_item->setText(0, computer_item->get_computer_name());
							computer_item->setText(2, QString::fromStdString(str_duration));
							computer_item->setText(3, QString::fromStdString(
								it->get()->get_playlist_container()->get_current_playing_item()));
							ui.main_tree_widget->topLevelItem(i)->addChild(computer_item);
							tree_contains_group = true;
						}
						++i;
					}
					/* Check if there was already a Group TopLevelItem */
					if (tree_contains_group == false)
					{//We need to create a new TopLevelItem for the Group now
						Typed_QTreeWidgetItem *group_item = new Typed_QTreeWidgetItem();
						group_item->set_group_name(QString::fromStdString(*it2));
						group_item->setText(1, QString::fromStdString(*it2));
						ui.main_tree_widget->addTopLevelItem(group_item);

						Typed_QTreeWidgetItem *computer_item = new Typed_QTreeWidgetItem();
						computer_item->set_computer_name(QString::fromStdString(it->get()->get_identification()));
						computer_item->set_group_name(QString::fromStdString(*it2));
						computer_item->setText(3, QString::fromStdString(
							it->get()->get_playlist_container()->get_current_playing_item()));
						/** Here we get the elapsed time between now, and the last ping the
						* remote screen has answered */
						boost::posix_time::time_duration duration =
							boost::posix_time::second_clock::universal_time()
							- it->get()->get_last_ping();
						std::string str_duration = boost::posix_time::to_simple_string(duration);

						/* Set actual data to display */
						computer_item->setText(0, computer_item->get_computer_name());
						computer_item->setText(2, QString::fromStdString(str_duration));

						/* Last thing to do to the group item, we get the Playlist_Container
						 * name from the item we just added. */
						group_item->set_playlist_name(
							QString::fromStdString(
							it->get()->get_playlist_container()->get_playlist_container_name()));
						group_item->setText(3, group_item->get_playlist_name());

						group_item->addChild(computer_item);
					}
				}
			}
		}
	}
}

void Tacktech_Manager_MainWindow::refresh_all_request()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Tacktech_Manager::refresh_all_request()" << std::endl;
#endif // _DEBUG
	upload_data.reset(new Upload_Data_Container(parameters));
	connect(upload_data.get(), SIGNAL(xml_creation_complete(std::string)), this,
		SLOT(start_upload(std::string)));
	//upload_data->set_playlist(playlist);
	//upload_data->set_groups(groups_and_computers);
	//upload_data->set_group_playlist(group_playlist);
	upload_data->set_command("GET_VARIABLES");
	upload_data->get_xml_upload();
}

void Tacktech_Manager_MainWindow::start_upload( std::string xml_string)
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
	network_manager->start_write(string_to_send);
	boost::thread t(
		boost::bind(&boost::asio::io_service::run, boost::ref(io_service)));
	t.join();
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Upload thread execution started" << std::endl;
#endif // _DEBUG
}

void Tacktech_Manager_MainWindow::data_recieved_slot( QString data_recieved )
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
		/* We declare 2 strings that contain the variables */
		std::string display_client_str = data_recieved.toStdString();
		std::string playlist_str = data_recieved.toStdString();
		std::string filelist_str = data_recieved.toStdString();

		/* Now substring the recieved data, and assign only the needed data
		 * to the appropraite string */
//		if(playlist_str.find("<Playlist_Container>") != playlist_str.npos)
//		{
//			playlist_str = playlist_str.substr(
//				playlist_str.find("<Playlist_Container>"),
//				playlist_str.rfind("</Playlist_Container>") + 21);
//
//			pugi::xml_document playlist_container_doc;
//			playlist_container_doc.load(playlist_str.c_str());
//#ifdef _SHOW_DEBUG_OUTPUT
//			std::cout << " - Playlist_Container RECIEVED Print: " << std::endl;
//			std::cout << "==================" << std::endl;
//			playlist_container_doc.print(std::cout);
//#endif
//
//			xml_string_writer playlist_container_writer;
//			playlist_container_doc.child("Playlist_Container")
//				.print(playlist_container_writer);
//			playlist_container.reset(
//				new Playlist_Container(playlist_container_writer.result));
//#ifdef _SHOW_DEBUG_OUTPUT
//			std::cout << " - Playlist_Container Print: " << std::endl;
//			std::cout << "==================" << std::endl;
//			playlist_container->print_contents();
//#endif
//		}
//		else
//		{
//			playlist_container.reset(
//				new Playlist_Container());
//		}

		if (display_client_str.find("<Display_Client_Container>") != display_client_str.npos)
		{
			display_client_str = display_client_str.substr(
				display_client_str.find("<Display_Client_Container>"),
				display_client_str.rfind("</Display_Client_Container>") + 27);

			pugi::xml_document display_client_doc;
			display_client_doc.load(display_client_str.c_str());
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << " - Display_Client_Container RECIEVED Print: " << std::endl;
			std::cout << "==================" << std::endl;
			display_client_doc.print(std::cout);
#endif

			xml_string_writer display_client_container_writer;
			display_client_doc.child("Display_Client_Container")
				.print(display_client_container_writer);
			display_client_container.reset(new Display_Client_Container(display_client_container_writer.result));
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << " - Dislpay_Client_Container Print: " << std::endl;
			std::cout << " ===============================" << std::endl;
			display_client_container->print_contents();
#endif
		}
		else
		{
			display_client_container.reset(new Display_Client_Container());
		}
		if (filelist_str.find("<Filelist>") != filelist_str.npos)
		{
			filelist_str = filelist_str.substr(
				filelist_str.find("<Filelist>"),
				filelist_str.rfind("</Filelist>") + 11);

			pugi::xml_document filelist_doc;
			filelist_doc.load(filelist_str.c_str());
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << " - Filelist RECIEVED Print: " << std::endl;
			std::cout << "==================" << std::endl;
			filelist_doc.print(std::cout);
#endif

			xml_string_writer filelist_writer;
			filelist_doc.child("Filelist")
				.print(filelist_writer);
			filelist.reset(new Filelist(filelist_writer.result));
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << " - Filelist Print: " << std::endl;
			std::cout << " ===============================" << std::endl;
			filelist->print_contents();
#endif
		}
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
	repopulate_ui();
}

void Tacktech_Manager_MainWindow::upload_files_to_server( std::vector<std::string> items, QTime time)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Tacktech_Manager_MainWindow::upload_files_to_server" << std::endl;
#endif // _DEBUG
	if(!items.empty())
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - There ARE items to upload" << std::endl;
#endif // _DEBUG
		if (time.hour() == 22 && time.second() == 22 && time.minute() == 22)
		{//Time is equal to the specially defined time that means to upload instantly
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << " ## Upload time is: NOW" << std::endl;
#endif // _DEBUG
			upload_data.reset(new Upload_Data_Container(parameters));
			connect(upload_data.get(), SIGNAL(xml_creation_complete(std::string)), this,
				SLOT(start_upload(std::string)));
			upload_data->set_upload_items(items);
			upload_data->set_command("FILE_UPLOAD");
			upload_data->get_xml_upload();
		}
		else
		{
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << " ## Upload time is: " << time.toString().toStdString() << std::endl;
#endif // _DEBUG
			pending_uploads->push_back(std::pair<std::vector<std::string>, QTime>(items, time));
		}
	}
}

void Tacktech_Manager_MainWindow::check_uploads_pending()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Tacktech_Manager_MainWindow::check_uploads_pending()" << std::endl;
#endif // _DEBUG
	for (std::vector<std::pair<std::vector<std::string>, QTime> >::iterator it = pending_uploads->begin();
		it != pending_uploads->end(); ++it)
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " ## Upload time is: " << it->second.toString().toStdString() << std::endl;
#endif // _DEBUG
		QTime current_time(QTime::currentTime());
		if (it->second == current_time.addSecs(180)
			|| it->second == current_time.addSecs(-180))
		{//Checks if the time when the upload should start is
		 //close to the actual time, with -+2 mins of discrepancy
		 //allowed
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << " ++ Uploading NOW" << std::endl;
#endif // _DEBUG
			upload_data.reset(new Upload_Data_Container(parameters));
			connect(upload_data.get(), SIGNAL(xml_creation_complete(std::string)), this,
				SLOT(start_upload(std::string)));
			upload_data->set_upload_items(it->first);
			upload_data->set_command("FILE_UPLOAD");
			upload_data->get_xml_upload();

			/* Erasing the pending upload from the pending_uploads */
			pending_uploads->erase(it);
		}
	}
}

void Tacktech_Manager_MainWindow::show_upload_gui()
{
	file_upload_dialog->show_gui();
}

void Tacktech_Manager_MainWindow::assign_group()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Tacktech_Manager_MainWindow::assign_group()" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	if (ui.main_tree_widget->selectedItems().count() > 0)
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Selected items count > 0" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
		std::vector<std::string> selected_names;
		foreach(QTreeWidgetItem *item, ui.main_tree_widget->selectedItems())
		{
			Typed_QTreeWidgetItem *t_item = static_cast<Typed_QTreeWidgetItem*>(item);
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << " -- " << t_item->get_computer_name().toStdString() << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
			selected_names.push_back(t_item->get_computer_name().toStdString());
		}
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " --- Resetting assign_group_dialog "  << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
		if (assign_group_dialog.get() != 0)
		{
			disconnect(assign_group_dialog.get(), SIGNAL(group_added()), this, SLOT(repopulate_ui()));
			disconnect(assign_group_dialog.get(), SIGNAL(group_added(Display_Client_Container_Ptr)), this, SLOT(display_container_changed(Display_Client_Container_Ptr)));
		}
		assign_group_dialog.reset(new Assign_Group(display_client_container, selected_names));
		connect(assign_group_dialog.get(), SIGNAL(group_added()), this, SLOT(repopulate_ui()));
		connect(assign_group_dialog.get(), SIGNAL(group_added(Display_Client_Container_Ptr)), this, SLOT(display_container_changed(Display_Client_Container_Ptr)));
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " ---- Showing assign_group_dialog "  << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
		assign_group_dialog->show();
	}
}

void Tacktech_Manager_MainWindow::display_container_changed(Display_Client_Container_Ptr p_display_client_container)
{
#ifdef _SHOW_DEBUG_OUTPUT
    std::cout << "=Tacktech_Manager_MainWindow::display_container_changed(Display_Client_Container_Ptr)" << std::endl;
	p_display_client_container->print_contents();
#endif // _SHOW_DEBUG_OUTPUT
    temp_display_client_container.reset(new Display_Client_Container(*p_display_client_container));
    upload_data.reset(new Upload_Data_Container(parameters));
    connect(upload_data.get(), SIGNAL(xml_creation_complete(std::string)), this,
        SLOT(start_upload(std::string)));
    upload_data->set_display_client_container(temp_display_client_container);
    upload_data->set_command("SET_DISPLAY_CONTAINER");
    upload_data->get_xml_upload();
}

void Tacktech_Manager_MainWindow::display_container_changed()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Tacktech_Manager_MainWindow::display_container_changed()" << std::endl;

#endif // _SHOW_DEBUG_OUTPUT
	temp_display_client_container.reset(new Display_Client_Container(*display_client_container));
	upload_data.reset(new Upload_Data_Container(parameters));
	connect(upload_data.get(), SIGNAL(xml_creation_complete(std::string)), this,
		SLOT(start_upload(std::string)));
	upload_data->set_display_client_container(temp_display_client_container);
	upload_data->set_command("SET_DISPLAY_CONTAINER");
	upload_data->get_xml_upload();
}

void Tacktech_Manager_MainWindow::edit_playlist_slot()
{
#ifdef _SHOW_DEBUG_OUTPUT
    std::cout << "=Tacktech_Manager_MainWindow::edit_playlist_slot()" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	if (edit_playlist_dialog.get() != 0)
		disconnect(edit_playlist_dialog.get(), SIGNAL(playlist_changed(Display_Client_Container_Ptr)), this, SLOT(display_container_changed(Display_Client_Container_Ptr)));
    edit_playlist_dialog.reset(new Edit_Playlist());
	edit_playlist_dialog->set_group_name(ui.main_tree_widget->selectedItems().at(0)->text(1).toStdString());
	edit_playlist_dialog->set_filelist(filelist);
	edit_playlist_dialog->set_organization_name(parameters["general.organization_name"]);
	edit_playlist_dialog->set_display_client_container(display_client_container);
	

	connect(edit_playlist_dialog.get(), SIGNAL(playlist_changed(Display_Client_Container_Ptr)), this, SLOT(display_container_changed(Display_Client_Container_Ptr)));

	edit_playlist_dialog->show();
}
