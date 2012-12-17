#ifndef UPLOAD_DATA_CONTAINER_H
#define UPLOAD_DATA_CONTAINER_H

#ifdef _SHOW_DEBUG_OUTPUT
#include <iostream>
#endif // _DEBUG
#include <QString>
#include <QObject>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <exception>
#include <cctype>
#include <pugixml.hpp>
#include <boost/lexical_cast.hpp>
#include <b64/encode.h>
#include <Playlist_Container.h>
#include <Group_Container.h>
#include <Group_Playlist_Container.h>
#include <Display_Client_Container.h>
class Upload_Data_Container: public QObject
{
Q_OBJECT
public:
	/** Default constructor */
	Upload_Data_Container(std::map<std::string, std::string>& parms);
	/** Default destructor */
	virtual
	~Upload_Data_Container();
	void set_playlist(Playlist_Container_Ptr);
	void set_display_client_container(Display_Client_Container_Ptr);
	void set_groups(Group_Container_Ptr);
	void set_group_playlist(Group_Playlist_Container_Ptr);
	void set_upload_items(std::vector<std::string>);
	void set_playlist_name(QString);
	void set_group_name(QString);
	void set_upload_time(QString);
	void set_command(QString);
	void set_organization_name(QString);
	void get_xml_upload();
protected:
private:
	std::string	get_binary_file(QString);
	Playlist_Container_Ptr playlist_container;
	Display_Client_Container_Ptr display_client_container;
	Group_Container_Ptr groups;
	Group_Playlist_Container_Ptr group_playlist;
	std::vector<std::string> upload_items;
	QString group_name;
	QString playlist_name;
	QString upload_time;
	QString command;
	QString organization_name;
	std::ifstream::pos_type m_size;
	std::map<std::string, std::string> parameters;

	std::string get_variables_request_xml();
	std::string	get_update_display_container_xml();
	std::string	upload_file();
signals:
	void xml_creation_complete(std::string);
};

#endif // UPLOAD_DATA_CONTAINER_H
