#include "Edit_Playlist.h"

Edit_Playlist::Edit_Playlist(QWidget *parent, Qt::WFlags flags) :
		QWidget(parent, flags)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Setting up Edit_Playlist GUI" << std::endl;
#endif // _DEBUG
	ui.setupUi(this);
	QStringList headers;
	headers << "Filename" << "Pause" << "Start Time" << "End Time";
	ui.playlist_tree_widget->setHeaderLabels(headers);

	display_client_list.reset(new std::vector<Display_Client_Ptr>);

#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Creating node_menu pointer" << std::endl;
#endif // _DEBUG
	node_menu = new QMenu();
	ui.playlist_tree_widget->setContextMenuPolicy(Qt::ActionsContextMenu);

	/* Creating context menu actions */
	add_file = new QAction(tr("Add file to playlist"), node_menu);
	remove_playlist = new QAction(tr("Remove Playlist"), node_menu);
	remove_file = new QAction(tr("Remove file from playlist"), node_menu);

	/* Adding actions to context_menu */
	ui.playlist_tree_widget->addAction(add_file);
	ui.playlist_tree_widget->addAction(remove_playlist);
	ui.playlist_tree_widget->addAction(remove_file);

	/* Connecting the main GUI signals for this class */
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(ok_clicked()));
	connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(cancel_clicked()));
	connect(remove_file, SIGNAL(triggered()), this, SLOT(remove_file_slot()));
	connect(remove_playlist, SIGNAL(triggered()), this,
			SLOT(remove_playlist_slot()));
	connect(add_file, SIGNAL(triggered()), this, SLOT(add_file_slot()));
	connect(ui.add_playlist_pushbutton, SIGNAL(clicked()),
		this, SLOT(create_playlist_slot()));
}

Edit_Playlist::~Edit_Playlist()
{
    delete add_file;
	delete remove_playlist;
	delete remove_file;
	delete node_menu;
}

/** Slot gets fired when the context menu item connected to it is
 ** activated. Removes a filename from the widget and global playlist
 ** variable. If both a filenames and playlist names are selected, removes
 ** both.
 ** Note: This function will take n^2 time to complete. */
void Edit_Playlist::remove_file_slot()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Edit_Playlist::remove_file_slot()" << std::endl;
#endif // _DEBUG
	if (!ui.playlist_tree_widget->selectedItems().count() == 0)
	{
		for (int i = 0; i < ui.playlist_tree_widget->selectedItems().size();
				i++)
		{
			Typed_QTreeWidgetItem *selected_item =
					static_cast<Typed_QTreeWidgetItem*>(ui.playlist_tree_widget->selectedItems().at(
							i));
			if (selected_item->get_type() == "PLAYLIST_ITEM")
			{
#ifdef _SHOW_DEBUG_OUTPUT
				std::cout << " - Removing filename: "
					<< selected_item->get_filename().toStdString()
					<< std::endl;
#endif // _DEBUG
				Typed_QTreeWidgetItem *selected_item_parent =
						static_cast<Typed_QTreeWidgetItem*>(selected_item->parent());

				for (std::vector<Display_Client_Ptr>::iterator it = display_client_list->begin();
					it != display_client_list->end(); ++it)
				{
					it->get()->get_playlist_container()->remove_playlist_item(
						selected_item_parent->get_playlist_name().toStdString(),
						selected_item->get_filename().toStdString());
				}
			}
//			else if (selected_item->get_type() == "PLAYLIST")
//			{
//#ifdef _SHOW_DEBUG_OUTPUT
//				std::cout << " - Removing playlist" << std::endl;
//#endif // _DEBUG
//				remove_playlist_slot();
//			}
		}
		/* We now repopulate the widget to reflect the changes */
		repopulate_widget();
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText("Select a file to remove from playlist");
		msgBox.exec();
	}
}

/** Slot removes a playlist and all its items from the widget and the
 ** playlist global variable */
void Edit_Playlist::remove_playlist_slot()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Edit_Playlist::remove_playlist_slot()" << std::endl;
#endif // _DEBUG
	if (ui.playlist_tree_widget->selectedItems().count() > 0
			&& ui.playlist_tree_widget->selectedItems().count() < 1)
	{
		Typed_QTreeWidgetItem *selected_item =
				static_cast<Typed_QTreeWidgetItem*>(ui.playlist_tree_widget->selectedItems().at(
						0));
		if (selected_item->get_type() == "PLAYLIST")
		{
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << " - Removing Playlist" << std::endl;
#endif // _DEBUG
			for (std::vector<Display_Client_Ptr>::iterator it = display_client_list->begin();
				it != display_client_list->end(); ++it)
			{
				it->get()->get_playlist_container()->remove_playlist(
					selected_item->get_playlist_name().toStdString());
			}
			/* Repopulate widget to reflect changes */
			repopulate_widget();
		}
		else
		{
			QMessageBox msgBox;
			msgBox.setText("Select a PLAYLIST to remove from playlist");
			msgBox.exec();
		}
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText("Select ONE playlist to remove from playlist");
		msgBox.exec();
	}
}

void Edit_Playlist::ok_clicked()
{
	//update_display_client_container();
	emit playlist_changed(display_client_container);
	this->close();
}

void Edit_Playlist::keyPressEvent(QKeyEvent *event)
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
    break;
    }
}

void Edit_Playlist::cancel_clicked()
{
	this->close();
}

void Edit_Playlist::repopulate_widget()
{
	ui.playlist_tree_widget->clear();
	Container temp_container = *display_client_list->at(0)->get_playlist_container()->get_playlist_container();
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Received '" << display_client_list->size() << "' display_clients" << std::endl;
	display_client_list->at(0)->print_contents();
#endif // _DEBUG
	for (std::vector<Playlist_Ptr>::iterator it = playlist_list.begin();
		it != playlist_list.end(); ++it)
	{
		Typed_QTreeWidgetItem *item = new Typed_QTreeWidgetItem();
		item->set_type("PLAYLIST");
		item->set_playlist_name(QString::fromStdString(it->get()->get_playlist_name()));
		item->setText(0, QString::fromStdString(it->get()->get_playlist_name()));
		item->setText(2, QString::fromStdString(boost::posix_time::to_iso_string(it->get()->get_start_time())));
		item->setText(3, QString::fromStdString(boost::posix_time::to_iso_string(it->get()->get_end_time())));
		ui.playlist_tree_widget->addTopLevelItem(item);
		for (std::vector<std::pair<std::string, int> >::iterator it2 = it->get()->get_playlist_items()->begin();
			it2 != it->get()->get_playlist_items()->end(); ++it2)
		{
			Typed_QTreeWidgetItem *file_item = new Typed_QTreeWidgetItem();
			file_item->set_type("PLAYLIST_ITEM");
			file_item->set_filename(QString::fromStdString(it2->first));
			file_item->setText(0, QString::fromStdString(it2->first));
			file_item->setText(1, QString::fromStdString(
				boost::lexical_cast<std::string, int>(it2->second)));
			item->addChild(file_item);
		}
	}
}

void Edit_Playlist::set_organization_name( std::string p_oranization_name)
{
	m_organization_name = p_oranization_name;
}

void Edit_Playlist::create_playlist_slot()
{
	if (ui.lineEdit->text() != "")
	{
		if (add_file_dialog.get() != 0)
			disconnect(add_file_dialog.get(), SIGNAL(playlist_added(Playlist_Ptr)), this, SLOT(playlist_added_slot(Playlist_Ptr)));
		add_file_dialog.reset(new Add_File_Dialog(filelist, ui.lineEdit->text(), QString::fromStdString(m_organization_name), QString::fromStdString(m_group_name)));
		connect(add_file_dialog.get(), SIGNAL(playlist_added(Playlist_Ptr)), this, SLOT(playlist_added_slot(Playlist_Ptr)));
		add_file_dialog->show();
	}
	else
	{
		QMessageBox box;
		box.setText("Please enter a playlist name");
		box.show();
	}
}

void Edit_Playlist::set_filelist( Filelist_Ptr p_filelist)
{
	filelist = p_filelist;
}

void Edit_Playlist::set_group_name( std::string p_group_name)
{
	m_group_name = p_group_name;
}

void Edit_Playlist::set_display_client_container( Display_Client_Container_Ptr p_display_client_container)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "Edit_Playlist::set_display_client_container" << std::endl;
#endif //_SHOW_DEBUG_OUTPUT
	display_client_container.reset(new Display_Client_Container(*p_display_client_container));
	display_client_list = display_client_container->get_display_clients(m_organization_name, m_group_name);
	playlist_list = display_client_list->at(0)->get_playlists_of_group(m_group_name);
	repopulate_widget();
}

void Edit_Playlist::playlist_added_slot( Playlist_Ptr p_playlist_ptr)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "Edit_Playlist::playlist_added_slot" << std::endl;
	p_playlist_ptr->print_contents();
#endif //_SHOW_DEBUG_OUTPUT
	for (std::vector<Display_Client_Ptr>::iterator it = display_client_list->begin();
		it != display_client_list->end(); ++it)
	{
		std::vector<std::string> organization_vector;
		organization_vector.push_back(m_organization_name);

		/** First we remove the old playlist if it exists, then we add the new one*/
		it->get()->get_playlist_container()->remove_playlist(p_playlist_ptr->get_playlist_name());
		it->get()->get_playlist_container()->add_playlist(p_playlist_ptr, organization_vector);
	}
	playlist_list = display_client_list->at(0)->get_playlists_of_group(m_group_name);
	repopulate_widget();
}

void Edit_Playlist::add_file_slot()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "Edit_Playlist::add_file_slot()" << std::endl;
#endif //_SHOW_DEBUG_OUTPUT
	if (ui.playlist_tree_widget->selectedItems().count() > 0
		&& ui.playlist_tree_widget->selectedItems().count() < 2)
	{
		Typed_QTreeWidgetItem *selected_item =
			static_cast<Typed_QTreeWidgetItem*>(ui.playlist_tree_widget->selectedItems().at(
			0));
		if (selected_item->get_type() == "PLAYLIST")
		{
			if (add_file_dialog.get() != 0)
				disconnect(add_file_dialog.get(), SIGNAL(playlist_added(Playlist_Ptr)), this, SLOT(playlist_added_slot(Playlist_Ptr)));
			add_file_dialog.reset(new Add_File_Dialog(filelist, selected_item->get_playlist_name(), QString::fromStdString(m_organization_name), QString::fromStdString(m_group_name)));
			connect(add_file_dialog.get(), SIGNAL(playlist_added(Playlist_Ptr)), this, SLOT(playlist_added_slot(Playlist_Ptr)));
			add_file_dialog->show();
			/* Repopulate widget to reflect changes */
			repopulate_widget();
		}
		else
		{
			QMessageBox msgBox;
			msgBox.setText("Select a PLAYLIST to add file to");
			msgBox.exec();
		}
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText("Select ONE playlist to add file to");
		msgBox.exec();
	}
}
