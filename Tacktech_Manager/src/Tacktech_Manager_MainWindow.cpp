#include "Tacktech_Manager_MainWindow.h"

Tacktech_Manager_MainWindow::Tacktech_Manager_MainWindow( QWidget *parent /*= 0*/, Qt::WFlags flags /*= 0*/ )
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Tacktech_Manager_MainWindow" << std::endl;
#endif // _DEBUG
	ui.setupUi(this);
	read_config();

	QStringList manager_headers;
	manager_headers << "Computer Name" << "Groups" << "Last Ping";
	ui.main_tree_widget->setHeaderLabels(manager_headers);
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
	for (Group_Multimap::iterator it = groups_and_computers->get_groups_and_computers()->begin();
		it != groups_and_computers->get_groups_and_computers()->end();
		++it)
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << "   - Adding name: " << it->second.first 
			<< std::endl;
#endif // _DEBUG
		computer_item = new Typed_QTreeWidgetItem();
		computer_item->set_computer_name(
			QString::fromStdString(it->second.first));
		computer_item->set_group_name(QString::fromStdString(it->first));
		computer_item->set_type("COMPUTER");
		computer_item->setText(0, QString::fromStdString(it->second.first));
		computer_item->setText(1, QString::fromStdString(it->first));

		/** Here we get the elapsed time between now, and the last ping the
		  * remote screen has answered */
		boost::posix_time::time_duration duration =
			boost::posix_time::second_clock::universal_time()
			- it->second.second;
		std::string str_duration = boost::posix_time::to_simple_string(duration);

		computer_item->setText(2, QString::fromStdString(str_duration));

		ui.main_tree_widget->addTopLevelItem(computer_item);
	}
}
