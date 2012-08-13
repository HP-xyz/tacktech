#include "Add_File_Dialog.h"

Add_File_Dialog::Add_File_Dialog(QWidget *parent, Qt::WFlags flags) :
		QMainWindow(parent, flags)
{
#ifdef _DEBUG
	std::cout << "= Add_File_Dialog()" << std::endl;
	std::cout << " - Setting up ui" << std::endl;
#endif // _DEBUG
	ui.setupUi(this);
#ifdef _DEBUG
	std::cout << " - Setting header labels" << std::endl;
#endif // _DEBUG
	QStringList headers;
	headers << "Filename" << "Pause (s)";
	ui.filename_tree_widget->setColumnCount(2);
	ui.filename_tree_widget->setHeaderLabels(headers);

#ifdef _DEBUG
	std::cout << " - Creating node menu" << std::endl;
#endif // _DEBUG
	node_menu = new QMenu();
	ui.filename_tree_widget->setContextMenuPolicy(Qt::ActionsContextMenu);

#ifdef _DEBUG
	std::cout << " - Creating add_pause_dialog pointer" << std::endl;
#endif // _DEBUG
	add_pause_dialog = new Add_Pause_Dialog();
#ifdef _DEBUG
	std::cout << " - Creating remove_filename action" << std::endl;
#endif // _DEBUG
	/* Creating context menu actions */
	remove_filename = new QAction(tr("Remove File"), node_menu);

#ifdef _DEBUG
	std::cout << " - Adding actions to tree widget" << std::endl;
#endif // _DEBUG
	/* Adding action to context menu */
	ui.filename_tree_widget->addAction(remove_filename);

#ifdef _DEBUG
	std::cout << " - Connecting primary signals" << std::endl;
#endif // _DEBUG
	/* Connecting the main GUI signals for this class */
	connect(remove_filename, SIGNAL(triggered()), this,
			SLOT(remove_filename_slot()));
	connect(ui.filename_tree_widget,
			SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this,
			SLOT(add_pause_slot(QTreeWidgetItem*, int)));
	connect(ui.browse_button, SIGNAL(clicked()), this,
			SLOT(add_filename_slot()));
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(ok_clicked()));
	connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(cancel_clicked()));

#ifdef _DEBUG
	std::cout << " - Connecting secondary signals" << std::endl;
#endif // _DEBUG
	/* Connecting secondary signals */
	connect(add_pause_dialog, SIGNAL(pause_changed(int)), this,
			SLOT(pause_changed_slot(int)));
	connect(add_pause_dialog, SIGNAL(pause_unchanged()), this,
			SLOT(pause_unchanged_slot()));
#ifdef _DEBUG
	std::cout << " - GUI setup complete" << std::endl;
#endif // _DEBUG
}

Add_File_Dialog::~Add_File_Dialog()
{
	delete remove_filename;
	delete node_menu;
	delete add_pause_dialog;
}

/** Sets the class playlist variable to that of the calling class.
 ** */
void Add_File_Dialog::set_playlist(Playlist_Container_Ptr p_playlist)
{
#ifdef _DEBUG
	std::cout << "= Add_File_Dialog::set_playlist()" << std::endl;
#endif // _DEBUG
	playlist = p_playlist;
	original_playlist = new Playlist_Container(*p_playlist);
	repopulate_widget();
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
#ifdef _DEBUG
	std::cout << "= Add_File_Dialog::repopulate_widget()" << std::endl;
	playlist->print_contents();
#endif // _DEBUG
	ui.filename_tree_widget->clear();

	Playlist_Range range = playlist->get_files_in_playlist(
			playlist_name.toStdString());
	Playlist_Multimap::iterator it = range.first;
	for (it; it != range.second; ++it)
	{
#ifdef _DEBUG
		std::cout << " - Adding: " << it->second.first << std::endl;
#endif // _DEBUG
		Typed_QTreeWidgetItem *new_item = new Typed_QTreeWidgetItem();
		new_item->set_filename(QString::fromStdString(it->second.first));
		new_item->set_pause(it->second.second);
		new_item->setText(0, QString::fromStdString(it->second.first));
		new_item->setText(1, QString::number(it->second.second));
		ui.filename_tree_widget->addTopLevelItem(new_item);
	}
}

/** Removes selected filename from the playlist */
void Add_File_Dialog::remove_filename_slot()
{
	if (ui.filename_tree_widget->selectedItems().count() != 0)
	{
		foreach(QTreeWidgetItem* item,
				ui.filename_tree_widget->selectedItems()){
		Typed_QTreeWidgetItem typed_item =
		static_cast<Typed_QTreeWidgetItem>(item);
		playlist->remove_filename_from_playlist(
				typed_item.get_playlist_name().toStdString(),
				typed_item.get_filename().toStdString());
	}
}
else
{
	QMessageBox msgBox;
	msgBox.setText("Select a file first");
	msgBox.exec();
}
}

/** Displays a file browser where the user can select a file to add to the
 ** playlist variable and the tree widget */
void Add_File_Dialog::add_filename_slot()
{
#ifdef _DEBUG
	std::cout << "= Add_File_Dialog::add_filename_slot()" << std::endl;
#endif // _DEBUG
	QStringList filenames = QFileDialog::getOpenFileNames(this,
			tr("Select file(s) to add to playlist"));
	foreach(QString filename, filenames){
#ifdef _DEBUG
	std::cout << " - Adding " << qPrintable(playlist_name)
	<< qPrintable(filename) << 0 << std::endl;
#endif // _DEBUG
	if(!playlist->add_filename(playlist_name.toStdString(),
					filename.toStdString(), 0))
	{
		QMessageBox msgBox;
		msgBox.setText(filename + " is already in the playlist");
		msgBox.exec();
	}
}
	repopulate_widget();
}

/** Slot to open the add_pause_dialog GUI. The parameter values are
 ** ignored as they are not needed. The slots makes use of the
 ** selectedItems() function of the tree widget to get the QTreeWidgetItem
 ** */
void Add_File_Dialog::add_pause_slot(QTreeWidgetItem *dud1, int dud2)
{
	if (ui.filename_tree_widget->selectedItems().count() == 1)
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

/** Slot is fired when the pause value of an item changes. Replaces the
 ** values in both the tree widget and the global playlist variable */
void Add_File_Dialog::pause_changed_slot(int new_pause)
{
#ifdef _DEBUG
	std::cout << "=Add_File_Dialog::pause_changed_slot()" << std::endl;
#endif // _DEBUG
	ui.filename_tree_widget->selectedItems().at(0)->setText(1,
			QString::number(new_pause));

	/* Removing the old item from the playlist */
	playlist->remove_filename_from_playlist(playlist_name.toStdString(),
			ui.filename_tree_widget->selectedItems().at(0)->text(0).toStdString());

	/* Inserting the new item into the playlist */
	playlist->add_filename(playlist_name.toStdString(),
			ui.filename_tree_widget->selectedItems().at(0)->text(0).toStdString(),
			new_pause);
	repopulate_widget();
}

/** Slot fired when the pause value is unchanged.
 ** Note: DOES NOTHING*/
void Add_File_Dialog::pause_unchanged_slot()
{

}

void Add_File_Dialog::ok_clicked()
{
#ifdef _DEBUG
	std::cout << "= Add_File_Dialog::ok_clicked()" << std::endl;
#endif // _DEBUG
	emit filelist_changed();
	delete original_playlist;
	this->close();
}

void Add_File_Dialog::cancel_clicked()
{
	playlist.reset(original_playlist);
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
