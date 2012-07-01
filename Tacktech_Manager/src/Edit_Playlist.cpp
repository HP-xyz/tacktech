#include "Edit_Playlist.h"

Edit_Playlist::Edit_Playlist(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
#ifdef _DEBUG
	std::cout << "= Setting up Edit_Playlist GUI" << std::endl;
#endif // _DEBUG
	ui.setupUi(this);
	QStringList headers;
	headers << "Playlist";
	ui.playlist_tree_widget->setHeaderLabels(headers);

#ifdef _DEBUG
	std::cout << " - Creating add_playlist_dialog pointer" << std::endl;
#endif // _DEBUG
	add_playlist_dialog = new Add_Playlist_Dialog();
#ifdef _DEBUG
	std::cout << " - Creating add_file_dialog pointer" << std::endl;
#endif // _DEBUG
	add_file_dialog = new Add_File_Dialog();

#ifdef _DEBUG
	std::cout << " - Creating node_menu pointer" << std::endl;
#endif // _DEBUG
	node_menu = new QMenu();
	ui.playlist_tree_widget->setContextMenuPolicy(Qt::ActionsContextMenu);

	/* Creating context menu actions */
	add_playlist = new QAction(tr("Add Playlist"), node_menu);
	add_file = new QAction(tr("Add file to playlist"), node_menu);
	remove_playlist = new QAction(tr("Remove Playlist"), node_menu);
	remove_file = new QAction(tr("Remove file from playlist"), node_menu);

	/* Adding actions to context_menu */
	ui.playlist_tree_widget->addAction(add_playlist);
	ui.playlist_tree_widget->addAction(add_file);
	ui.playlist_tree_widget->addAction(remove_playlist);
	ui.playlist_tree_widget->addAction(remove_file);

	/* Connecting the main GUI signals for this class */
	connect(add_playlist, SIGNAL(triggered()),
		this, SLOT(add_playlist_slot()));
	connect(add_file, SIGNAL(triggered()),
		this, SLOT(add_file_slot()));
	connect(ui.buttonBox, SIGNAL(accepted()),
		this, SLOT(ok_clicked()));
	connect(ui.buttonBox, SIGNAL(rejected()),
		this, SLOT(cancel_clicked()));

	/* Connecting secondary signals */
	connect(add_playlist_dialog, SIGNAL(playlist_name_added()),
		this, SLOT(repopulate_widget()));
}

Edit_Playlist::~Edit_Playlist()
{
	delete add_playlist;
	delete add_file;
	delete remove_playlist;
	delete remove_file;
	delete node_menu;
	delete add_playlist_dialog;
	delete add_file_dialog;
}
 /** Function to set the playlist variable to that the calling class 
  ** provides */
void Edit_Playlist::set_playlist( Playlist_Container *p_playlist)
{
	playlist = p_playlist;
}

/** Slot to start the Add_Playlist GUI and show the GUI */
void Edit_Playlist::add_playlist_slot()
{
	add_playlist_dialog->set_playlist(playlist);
	add_playlist_dialog->show();
}

void Edit_Playlist::remove_file_slot()
{

}

void Edit_Playlist::remove_playlist_slot()
{

}

void Edit_Playlist::add_file_slot()
{
	if (ui.playlist_tree_widget->selectedItems().count() == 1)
	{
		add_file_dialog->set_playlist_name(
			ui.playlist_tree_widget->selectedItems().at(0)->text(0));
		add_file_dialog->set_playlist(playlist);
		add_file_dialog->show();
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText("Selet only ONE playlist to add files to");
		msgBox.exec();
	}
}

void Edit_Playlist::ok_clicked()
{

}

void Edit_Playlist::cancel_clicked()
{

}

void Edit_Playlist::repopulate_widget()
{
	foreach(QString playlist_name, playlist->get_playlist().keys())
	{
		Typed_QTreeWidgetItem *playlist_item = new Typed_QTreeWidgetItem();
		playlist_item->setText(0, playlist_name);
		playlist_item->set_type("PLAYLIST");
		playlist_item->set_playlist_name(playlist_name);
		ui.playlist_tree_widget->addTopLevelItem(playlist_item);
		for (int i = 0;
			i < playlist->get_playlist().values(playlist_name).count();
			i++)
		{
			Typed_QTreeWidgetItem *filename_item =
				new Typed_QTreeWidgetItem();
			filename_item->setText(0,
				playlist->get_playlist().values(playlist_name)
				.at(i).first);
			filename_item->setText(1, QString::number(
				playlist->get_playlist().values(playlist_name)
				.at(i).second));
			filename_item->set_filename(
				playlist->get_playlist().values(playlist_name)
				.at(i).first);
			filename_item->set_pause(
				playlist->get_playlist().values(playlist_name)
				.at(i).second);
			filename_item->set_playlist_name(playlist_name);
			playlist_item->addChild(filename_item);
		}
	}
}
