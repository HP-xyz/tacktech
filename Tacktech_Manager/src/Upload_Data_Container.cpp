#include "Upload_Data_Container.h"

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

Upload_Data_Container::Upload_Data_Container()
{
    //ctor
}

Upload_Data_Container::~Upload_Data_Container()
{
    //dtor
}
/** @brief set_group_name
  *
  * Sets the group_name of the selected item
  */
void Upload_Data_Container::set_group_name(QString p_group_name)
{
    group_name = p_group_name;
}

/** @brief set_playlist_name
  *
  * Sets the playlist_name of the selected item
  */
void Upload_Data_Container::set_playlist_name(QString p_playlist_name)
{
    playlist_name = p_playlist_name;
}

/** @brief set_groups
  *
  * Sets the group variable to that provided in the
  * parameter.
  */
void Upload_Data_Container::set_groups(Group_Container *p_group_container)
{
    groups = p_group_container;
}

/** @brief set_playlist
  *
  * Sets the playlist variable to that provided in the
  * parameter.
  */
void Upload_Data_Container::set_playlist(Playlist_Container *p_playlist_container)
{
    playlist = p_playlist_container;
}

/** @brief get_binary_file
  *
  * Reads a file into memory and returns a char* to
  * the read file
  */
char* Upload_Data_Container::get_binary_file(QString filename)
{
    char *binary_file;
    std::ifstream file(filename.toStdString().c_str());
    if (file.is_open())
    {
        m_size = file.tellg();
        binary_file = new char[m_size];
        file.seekg(0, std::ios::beg);
        file.read(binary_file, m_size);
        file.close();
    }
    return binary_file;
}

/** @brief set_upload_time
  *
  * Sets the upload time for this payload
  */
void Upload_Data_Container::set_upload_time(QString p_upload_time)
{
    upload_time = p_upload_time;
}

/** @brief get_xml_upload
  *
  * This function creates a std::string variable that contains
  * the xml generated by this function.
  * The XML is generated in this format:
  *
  * Tacktech
  *     Type
  *         Command = COMMAND_HERE
  *     Type
  *     Upload <Upload_Time=TIME_HERE>
  *         Computer = COMPUTER_IP
  *             Item
  *                 <Filename>FILENAME_HERE</Filename>
  *                 <File_Data>FILE_DATA_HERE</File_Data>
  *                 <Pause>PAUSE_HERE</Pause>
  *             Item
  *         Computer
  *     Data
  * Tacktech
  *
  */
void Upload_Data_Container::get_xml_upload()
{
    pugi::xml_document transmit_document;
    pugi::xml_node root_node = transmit_document.append_child("Tacktech");
    pugi::xml_node type_node = root_node.append_child("Type");
    pugi::xml_node upload_node = type_node.append_child("Upload");
    upload_node.append_attribute("Upload_Time") = upload_time.toStdString().c_str();
    for (int i = 0; i < groups->get_groups_and_computers().values(group_name).size(); i++)
    {
        pugi::xml_node computer_node = upload_node.append_child("Computer");
        computer_node.append_attribute("Computer_IP") =
            groups->get_groups_and_computers().values(group_name).at(i).toStdString().c_str();

        for (int j = 0; j < playlist->get_playlist().values(playlist_name).size(); j++)
        {
            pugi::xml_node item_node = computer_node.append_child("Item");

            /* Creating item children */
            pugi::xml_node filename_node = item_node.append_child("Filename");
            pugi::xml_node file_data_node = item_node.append_child("File_Data");
            pugi::xml_node pause_node = item_node.append_child("Pause");

            /* Creating item children's pcdata */
            pugi::xml_node filename_pcdata = filename_node.append_child(pugi::node_pcdata);
            pugi::xml_node file_data_pcdata = file_data_node.append_child(pugi::node_pcdata);
            pugi::xml_node pause_pcdata = pause_node.append_child(pugi::node_pcdata);

            /* Giving pcdata a value */
			std::string temp_filename = playlist->get_playlist().
				values(playlist_name).at(j).first.toStdString();
			if (temp_filename.find("\\") != std::string::npos)
				temp_filename = temp_filename.substr(temp_filename.find_last_of("\\") + 1);
			if (temp_filename.find("/") != std::string::npos)
				temp_filename = temp_filename.substr(temp_filename.find_last_of("/") + 1);
            filename_pcdata.set_value(temp_filename.c_str());
            file_data_pcdata.set_value(
                 get_binary_file(
                     playlist->get_playlist().values(playlist_name).at(j).first));
            pause_pcdata.set_value(boost::lexical_cast<std::string>(
                playlist->get_playlist().values(playlist_name).at(j).second).c_str());
        }
    }
#ifdef _DEBUG
	std::cout << " - Emitting xml_creation complete signal " << std::endl;
#endif // _DEBUG
	xml_string_writer writer;
	transmit_document.print(writer);
	emit xml_creation_complete(writer.result);
}

