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

	node_menu = new QMenu();
	ui.playlist_tree_widget->setContextMenuPolicy(Qt::ActionsContextMenu);

	/* Creating context menu actions */
	add_playlist = new QAction(tr("Add Group"), node_menu);
	add_file = new QAction(tr("Add file to playlist"), node_menu);
	remove_playlist = new QAction(tr("Remove Group"), node_menu);
	remove_file = new QAction(tr("Remove file from playlist"), node_menu);

	/* Adding actions to context_menu */
	ui.playlist_tree_widget->addAction(add_playlist);
	ui.playlist_tree_widget->addAction(add_file);
	ui.playlist_tree_widget->addAction(remove_playlist);
	ui.playlist_tree_widget->addAction(remove_file);

	/* Connecting the main GUI signals for this class */
	connect(add_playlist, SIGNAL(triggered()),
		this, SLOT(add_playlist_slot()));
}

Edit_Playlist::~Edit_Playlist()
{

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

}
