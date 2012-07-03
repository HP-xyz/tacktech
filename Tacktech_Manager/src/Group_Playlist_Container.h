#ifndef GROUP_PLAYLIST_CONTAINER
#define GROUP_PLAYLIST_CONTAINER
#include <QList>
#include <QPair>

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

class Group_Playlist_Container
{
public:
	Group_Playlist_Container();
	~Group_Playlist_Container();
	QList<QPair<QString, QString> >* get_group_playlist();
	bool contains_group_name(QString);
	void connect_group_to_playlist(QString, QString);
private:
	QList<QPair<QString, QString> > *group_playlist;
};
#endif //GROUP_PLAYLIST_CONTAINER