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
	ui.setupUi(this);
	read_config();

	io_service.reset(new boost::asio::io_service);
	network_manager.reset(
		new Tacktech_Network_Manager(*io_service, parameters));

	QStringList manager_headers;
	manager_headers << "Computer Name" << "Groups" << "Last Ping";
	ui.main_tree_widget->setHeaderLabels(manager_headers);
	refresh_timer = new QTimer(this);

	connect(refresh_timer, SIGNAL(timeout()), this, SLOT(refresh_all_request()));
	connect(ui.actionUpload_Files_To_Server, SIGNAL(triggered()), this, SLOT(show_upload_gui()));
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

	Typed_QTreeWidgetItem *computer_item;
	for (unsigned int i = 0;
		i < display_client_container->get_display_client_container().size();
		++i)
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << "   - Adding name: " 
			<< display_client_container->get_display_client_container()
			[i]->get_identification() 
			<< std::endl;
#endif // _DEBUG
		computer_item = new Typed_QTreeWidgetItem();
		computer_item->set_computer_name(
			QString::fromStdString(display_client_container->
			get_display_client_container()
			[i]->get_identification()));

		computer_item->set_group_name(QString::fromStdString(
			display_client_container->get_display_client_container()
			[i]->get_groups_string()));
		computer_item->set_type("COMPUTER");
		computer_item->setText(0, 
			QString::fromStdString(display_client_container->
			get_display_client_container()
			[i]->get_identification()));
		computer_item->setText(1, QString::fromStdString(
			display_client_container->get_display_client_container()
			[i]->get_groups_string()));

		/** Here we get the elapsed time between now, and the last ping the
		  * remote screen has answered */
		boost::posix_time::time_duration duration =
			boost::posix_time::second_clock::universal_time()
			- display_client_container->get_display_client_container()[i]->get_last_ping();
		std::string str_duration = boost::posix_time::to_simple_string(duration);

		computer_item->setText(2, QString::fromStdString(str_duration));

		ui.main_tree_widget->addTopLevelItem(computer_item);
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

		/* Now substring the recieved data, and assign only the needed data
		 * to the appropraite string */
		playlist_str = playlist_str.substr(
			playlist_str.find("<Playlist_Container>"),
			playlist_str.rfind("</Playlist_Container>") + 21);
		display_client_str = display_client_str.substr(
			display_client_str.find("<Display_Client_Container"),
			display_client_str.rfind("</Display_Client_Container>") + 27);

		pugi::xml_document playlist_container_doc;
		playlist_container_doc.load(playlist_str.c_str());
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Playlist_Container RECIEVED Print: " << std::endl;
		std::cout << "==================" << std::endl;
		playlist_container_doc.print(std::cout);
#endif

		pugi::xml_document display_client_doc;
		display_client_doc.load(display_client_str.c_str());
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Display_Client_Container RECIEVED Print: " << std::endl;
		std::cout << "==================" << std::endl;
		display_client_doc.print(std::cout);
#endif

		xml_string_writer playlist_container_writer;
		playlist_container_doc.child("Playlist_Container")
			.print(playlist_container_writer);
		playlist_container.reset(
			new Playlist_Container(playlist_container_writer.result));
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Playlist_Container Print: " << std::endl;
		std::cout << "==================" << std::endl;
		playlist_container->print_contents();
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
//
//		xml_string_writer group_playlist_writer;
//		group_playlist_doc.child("GROUPS_PLAYLIST_NODE")
//			.child("Group_Playlist").print(	group_playlist_writer);
//		group_playlist->reset_container();
//		group_playlist->construct_group_playlist(
//			group_playlist_writer.result);
//#ifdef _SHOW_DEBUG_OUTPUT
//		std::cout << " - group_playlist Print: " << std::endl;
//		std::cout << "=========================" << std::endl;
//		groups_and_computers->print_contents();
//#endif
//		status_msg += "Received variables successfully";
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
	if(items.size() > 0)
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
