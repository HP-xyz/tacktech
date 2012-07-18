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
    groups = 0;
    playlist = 0;
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
#ifdef _DEBUG
	std::cout << "= Upload_Data_Container::get_binary_file()" << std::endl;
#endif // _DEBUG
    char *file_encoded = NULL;
    std::ifstream file(filename.toStdString().c_str(), std::ios::binary);
    if (file.is_open())
    {
#ifdef _DEBUG
		std::cout << " - File is open" << std::endl;
#endif // _DEBUG
		base64::encoder E;
		std::stringstream file_out;
		E.encode(file, file_out);

		file_out.seekg(0, std::ios::end);
		int file_length(file_out.tellg());
#ifdef _DEBUG
		std::cout << " - Creating char of size: " << file_length
<< std::endl;
#endif // _DEBUG
        file_out.seekg(0, std::ios::beg);

		file_encoded = new char[file_length + 1];
		file_out.read(file_encoded, file_length);
		file_encoded[file_length] = 0;
        file.close();
    }
    return file_encoded;
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
  *     Type TYPE=TYPE_HERE
  *     Type
  *     Upload <Upload_Time=TIME_HERE>
  *         Computer Computer_IP = COMPUTER_IP
  *             Item
  *                 <Filename>FILENAME_HERE</Filename>
  *                 <File_Data>FILE_DATA_HERE</File_Data>
  *                 <Pause>PAUSE_HERE</Pause>
  *             Item
  *         Computer
  *     Upload
  * Tacktech
  *
  */
void Upload_Data_Container::get_xml_upload()
{
#ifdef _DEBUG
	std::cout << "= Upload_Data_Container::get_xml_upload()" << std::endl;
#endif // _DEBUG
    pugi::xml_document transmit_document;
    pugi::xml_node root_node = transmit_document.append_child("Tacktech");
    pugi::xml_node type_node = root_node.append_child("Type");
	type_node.append_attribute("TYPE") = "UPLOAD";
    pugi::xml_node upload_node = type_node.append_child("Upload");
    upload_node.append_attribute("Upload_Time") = upload_time.toStdString().c_str();
    int computer_count = 0;
    for (int i = 0; i < groups->get_groups_and_computers().values(group_name).size(); i++)
    {
        pugi::xml_node computer_node = upload_node.append_child("Computer");
        computer_node.append_attribute("Computer_IP") =
            groups->get_groups_and_computers().values(group_name).at(i).toStdString().c_str();
        computer_count += 1;

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

            if (computer_count == 1)
            {
                file_data_pcdata.set_value(get_binary_file(
                        playlist->get_playlist().values(playlist_name).at(j).first));
                pause_pcdata.set_value(boost::lexical_cast<std::string>(
                    playlist->get_playlist().values(playlist_name).at(j).second).c_str());
            }
            else
            {
                pugi::xml_node temp_node = upload_node.child("Computer");
                file_data_pcdata.set_value(temp_node.child_value("File_Data"));
                pause_pcdata.set_value(temp_node.child_value("Pause"));
            }
        }
    }
#ifdef _DEBUG
	std::cout << " - Emitting xml_creation complete signal " << std::endl;
#endif // _DEBUG
	xml_string_writer writer;
	transmit_document.print(writer);
	emit xml_creation_complete(writer.result);
}

