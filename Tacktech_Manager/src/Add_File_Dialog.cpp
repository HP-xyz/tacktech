#include "Add_File_Dialog.h"

Add_File_Dialog::Add_File_Dialog(Filelist_Ptr p_filelist, QString p_playlist_name, QString p_playlist_organizaiton, QString p_playlist_group, QWidget *parent, Qt::WFlags flags) :
		QMainWindow(parent, flags)
{
	set_filelist(p_filelist);
	set_playlist_name(p_playlist_name);
	set_playlist_organization(p_playlist_organizaiton);
	set_playlist_group(p_playlist_group);
	playlist.reset(new Playlist());
	playlist->add_group(playlist_group.toStdString());
	playlist->set_playlist_name(playlist_name.toStdString());
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Add_File_Dialog()" << std::endl;
	std::cout << " - Setting up ui" << std::endl;
#endif // _DEBUG
	ui.setupUi(this);
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Setting header labels" << std::endl;
#endif // _DEBUG
	QStringList headers;
	headers << "Filename" << "Pause (s)";
	ui.playlist_filenames->setColumnCount(2);
	ui.playlist_filenames->setHeaderLabels(headers);
	ui.server_filenames->setHeaderLabel("Files on Server");

#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Creating node menu" << std::endl;
#endif // _DEBUG
	node_menu = new QMenu();
	ui.playlist_filenames->setContextMenuPolicy(Qt::ActionsContextMenu);

#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Creating add_pause_dialog pointer" << std::endl;
#endif // _DEBUG
	add_pause_dialog = new Add_Pause_Dialog();
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Creating remove_filename action" << std::endl;
#endif // _DEBUG
	/* Creating context menu actions */
	add_pause = new QAction(tr("Add pause"), node_menu);

#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Adding actions to tree widget" << std::endl;
#endif // _DEBUG
	/* Adding action to context menu */
	ui.playlist_filenames->addAction(add_pause);

#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Connecting primary signals" << std::endl;
#endif // _DEBUG
	/* Connecting the main GUI signals for this class */
	connect(add_pause, SIGNAL(triggered()), this,
			SLOT(add_pause_slot()));
	connect(ui.playlist_filenames,
			SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this,
			SLOT(add_pause_slot(QTreeWidgetItem*, int)));
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(ok_clicked()));
	connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(cancel_clicked()));
	connect(ui.add_button, SIGNAL(clicked()), this, SLOT(add_file_slot()));
	connect(ui.remove_button, SIGNAL(clicked()), this, SLOT(remove_file_slot()));

#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Connecting secondary signals" << std::endl;
#endif // _DEBUG
	/* Connecting secondary signals */
	connect(add_pause_dialog, SIGNAL(pause_changed(int)), this,
			SLOT(pause_changed_slot(int)));
	connect(add_pause_dialog, SIGNAL(pause_unchanged()), this,
			SLOT(pause_unchanged_slot()));
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - GUI setup complete" << std::endl;
#endif // _DEBUG
	repopulate_widget();
}

Add_File_Dialog::~Add_File_Dialog()
{
	delete add_pause;
	delete node_menu;
	delete add_pause_dialog;
}

/** Sets the class playlist_name variable to the one gained from checking
 ** the selected item in the calling class's widget.
 ** NB: HAS TO BE CALLED BEFORE set_playlist*/
void Add_File_Dialog::set_playlist_name(QString p_playlist_name)
{
	playlist_name = p_playlist_name;
	this->setWindowTitle(playlist_name);
}

/** Repopulates the tree widget */
void Add_File_Dialog::repopulate_widget()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Add_File_Dialog::repopulate_widget()" << std::endl;
#endif // _DEBUG
	ui.playlist_filenames->clear();
	for (std::vector<std::pair<std::string,int> >::iterator it = playlist->get_playlist_items()->begin();
		it != playlist->get_playlist_items()->end(); ++it)
	{
		Typed_QTreeWidgetItem *item = new Typed_QTreeWidgetItem();
		item->set_filename(QString::fromStdString(it->first));
		item->setText(0, QString::fromStdString(it->first));
		item->setText(1, QString::fromStdString(boost::lexical_cast<std::string, int>(it->second)));
		ui.playlist_filenames->addTopLevelItem(item);
	}
#ifdef _SHOW_DEBUG_OUTPUT
	filelist->print_contents();
#endif // _DEBUG
	std::vector<std::string> temp_vector = filelist->get_filelist(playlist_organization.toStdString());
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "Server contains '" << temp_vector.size() << "' files" << std::endl;
#endif // _DEBUG
	for (std::vector<std::string>::iterator it = temp_vector.begin();
		it != temp_vector.end(); ++it)
	{
		Typed_QTreeWidgetItem *item = new Typed_QTreeWidgetItem();
		item->set_filename(QString::fromStdString(*it));
		item->setText(0, QString::fromStdString(*it));
		ui.server_filenames->addTopLevelItem(item);
	}
}

/** Slot to open the add_pause_dialog GUI. The parameter values are
 ** ignored as they are not needed. The slots makes use of the
 ** selectedItems() function of the tree widget to get the QTreeWidgetItem
 ** */
void Add_File_Dialog::add_pause_slot(QTreeWidgetItem *dud1, int dud2)
{
	if (ui.playlist_filenames->selectedItems().count() == 1)
	{
		add_pause_dialog->show();
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText("Select only ONE file");
		msgBox.exec();
	}
}

void Add_File_Dialog::add_pause_slot()
{
	if (ui.playlist_filenames->selectedItems().count() > 0)
	{
		add_pause_slot(ui.playlist_filenames->selectedItems().at(0), 0);
	}
}

/** Slot is fired when the pause value of an item changes. Replaces the
 ** values in both the tree widget and the global playlist variable */
void Add_File_Dialog::pause_changed_slot(int new_pause)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Add_File_Dialog::pause_changed_slot()" << std::endl;
#endif // _DEBUG
	ui.playlist_filenames->selectedItems().at(0)->setText(1,
			QString::number(new_pause));
}

/** Slot fired when the pause value is unchanged.
 ** Note: DOES NOTHING*/
void Add_File_Dialog::pause_unchanged_slot()
{

}

void Add_File_Dialog::ok_clicked()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Add_File_Dialog::ok_clicked()" << std::endl;
#endif // _DEBUG
	add_new_playlist();
	emit playlist_added(playlist);
	this->close();
}

void Add_File_Dialog::cancel_clicked()
{
	this->close();
}

void Add_File_Dialog::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Enter:
	{
		ok_clicked();
		break;
	}
	case Qt::Key_Return:
	{
		ok_clicked();
		break;
	}
	default:
		QWidget::keyPressEvent(event);
	}
}

void Add_File_Dialog::set_playlist_organization( QString p_playlist_organization)
{
	playlist_organization = p_playlist_organization;
}

void Add_File_Dialog::set_filelist( Filelist_Ptr p_filelist)
{
	filelist = p_filelist;
}

void Add_File_Dialog::add_file_slot()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Add_File_Dialog::add_file_slot()" << std::endl;
	std::cout << " - Selected item count: " << ui.server_filenames->selectedItems().size() << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	if (ui.server_filenames->selectedItems().size() > 0)
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << "  -- Some items are selected" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
		foreach(QTreeWidgetItem *item, ui.server_filenames->selectedItems())
		{
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << "  --- Adding:" << item->text(0).toStdString() << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
			Typed_QTreeWidgetItem *item_to_insert = new Typed_QTreeWidgetItem();
			item_to_insert->set_filename(item->text(0));
			item_to_insert->set_pause(0);
			item_to_insert->setText(0, item->text(0));
			item_to_insert->setText(1, "0");
			ui.playlist_filenames->addTopLevelItem(item_to_insert);
			ui.server_filenames->takeTopLevelItem(ui.server_filenames->indexOfTopLevelItem(item));
			delete item;
		}
	}
}

void Add_File_Dialog::remove_file_slot()
{
	if (ui.playlist_filenames->selectedItems().size() > 0)
	{
		foreach(QTreeWidgetItem *item, ui.playlist_filenames->selectedItems())
		{
			Typed_QTreeWidgetItem *item_to_insert = new Typed_QTreeWidgetItem();
			item_to_insert->set_filename(item->text(0));
			item_to_insert->setText(0, item->text(0));
			ui.server_filenames->addTopLevelItem(item_to_insert);
			ui.playlist_filenames->takeTopLevelItem(ui.playlist_filenames->indexOfTopLevelItem(item));
			delete item;
		}
	}
}

void Add_File_Dialog::add_new_playlist()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Add_File_Dialog::add_new_playlist()" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	for (int i = 0; i < ui.playlist_filenames->topLevelItemCount(); ++i)
	{
		playlist->add_playlist_item(
			ui.playlist_filenames->topLevelItem(i)->text(0).toStdString(),
			ui.playlist_filenames->topLevelItem(i)->text(1).toInt());
	}
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Created playlist '" << playlist_name.toStdString() 
		<< "' containing '" << playlist->get_playlist_items()->size() << "' items" << std::endl;
	std::cout << " - Playlist belongs to group: " << make_list(*playlist->get_groups()) << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
}
std::string Add_File_Dialog::make_list( std::set<std::string> p_vector)
{
	std::string comma_separated_list;
	for (std::set<std::string>::iterator it = p_vector.begin();
		it != p_vector.end(); ++it)
	{
		comma_separated_list += *it;
		comma_separated_list += ", ";
	}
	comma_separated_list =
		comma_separated_list.substr(0, comma_separated_list.length() - 2);
	return comma_separated_list;
}

void Add_File_Dialog::set_playlist_group( QString p_playlist_group)
{
	playlist_group = p_playlist_group;
}
