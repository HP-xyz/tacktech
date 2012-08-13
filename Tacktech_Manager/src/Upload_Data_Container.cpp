#include "Upload_Data_Container.h"

bool string_allowed_char_predicate(char c)
{
	if (!(isalnum(c) || ispunct(c) || iscntrl(c)))
		return true;
	else
		return false;
}

/** Struct to define a xml_writer to string.
 ** Copied directly from the pugixml quickstart */
struct xml_string_writer: pugi::xml_writer
{
	std::string result;
	virtual void write(const void* data, std::size_t size)
	{
#ifdef _DEBUG
		std::cout << " - (XML_WRITER)write of size: " << size << std::endl;
#endif // _DEBUG
		result += static_cast<const char*>(data);
		//result += std::string(static_cast<const char*>(data),
		//	size);
	}
};

Upload_Data_Container::Upload_Data_Container(
		std::map<std::string, std::string>& p_parms)
{
	parameters = p_parms;
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
void Upload_Data_Container::set_groups(Group_Container_Ptr p_group_container)
{
	groups = p_group_container;
}

/** @brief set_playlist
 *
 * Sets the playlist variable to that provided in the
 * parameter.
 */
void Upload_Data_Container::set_playlist(
		Playlist_Container_Ptr p_playlist_container)
{
	playlist = p_playlist_container;
}

/** @brief get_binary_file
 *
 * Reads a file into memory and returns a std::string to
 * the read file
 */
std::string Upload_Data_Container::get_binary_file(QString filename)
{
#ifdef _DEBUG
	std::cout << "= Upload_Data_Container::get_binary_file()" << std::endl;
#endif // _DEBUG
	std::string file_encoded;
	std::string test;
	std::ifstream file(filename.toStdString().c_str(), std::ios::binary);
	if (file.is_open())
	{
#ifdef _DEBUG
		std::cout << " - File is open" << std::endl;
		file.seekg(0, std::ios::end);
		std::cout << " - Tellg(): " << file.tellg() << std::endl;
		file.seekg(0, std::ios::beg);
#endif // _DEBUG
		std::stringstream *file_in = new std::stringstream();
		*file_in << file.rdbuf();
		std::cout << "file_in size: " << file_in->str().size() << std::endl;
		*file_in << std::ends;
		base64::encoder E;
		std::stringstream file_out;
		E.encode(*file_in, file_out);
		delete file_in;
		file_out << std::ends;
		std::cout << "file_out size: " << file_out.str().size() << std::endl;
		file_encoded = file_out.str();
#ifdef _DEBUG
		std::cout << " - Encoded filesize: " << file_encoded.size()
				<< std::endl;
#endif // _DEBUG
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

void Upload_Data_Container::set_command(QString p_command)
{
	command = p_command;
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
 *     Playlist PLAYLIST=PLAYLIST_NAME_HERE
 *     Upload <Upload_Time=TIME_HERE>
 *         Item
 *             <Filename>FILENAME_HERE</Filename>
 *             <File_Data>FILE_DATA_HERE</File_Data>
 *             <Pause>PAUSE_HERE</Pause>
 *         Item
 *     Upload
 * Tacktech
 *
 */
void Upload_Data_Container::get_xml_upload()
{
	if (command == "GET_VARIABLES")
	{
		emit xml_creation_complete(
				Upload_Data_Container::get_variables_request_xml());
	}
	else if (command == "SET_VARIABLES")
	{
		emit xml_creation_complete(
				Upload_Data_Container::set_variable_command_xml());
	}
	else if (command == "UPLOAD")
	{
		emit xml_creation_complete(Upload_Data_Container::upload_file());
	}
}

/** @brief get_variable_request_xml
 *
 * This function creates a std::string variable that contains
 * the xml generated by this function.
 * The XML is generated in this format:
 *
 * Tacktech
 *     Type TYPE=GET_VARIABLES
 *     Type
 *     Return ip IP=IP HERE
 *     Return ip
 * Tacktech
 *
 */
std::string Upload_Data_Container::get_variables_request_xml()
{
#ifdef _DEBUG
	std::cout << "= pload_Data_Container::get_variables_request_xml()"
			<< std::endl;
#endif // _DEBUG
	pugi::xml_document transmit_document;
	pugi::xml_node root_node = transmit_document.append_child("Tacktech");
	pugi::xml_node type_node = root_node.append_child("Type");
	type_node.append_attribute("TYPE") = "GET_VARIABLES";
	pugi::xml_node ip_node = root_node.append_child("Return_IP");
	ip_node.append_attribute("IP") = parameters["general.manager_ip"].c_str();
	ip_node.append_attribute("PORT") =
			parameters["general.manager_port"].c_str();

	xml_string_writer writer;
	transmit_document.print(writer);
#ifdef _DEBUG
	std::cout << "  - Sending variable request:" << std::endl << writer.result
			<< std::endl << "  - Variable request c_str():" << std::endl
			<< writer.result.c_str() << std::endl;
#endif // _DEBUG
	return writer.result;
}

/** @brief set_variable_command_xml
 *
 * This function creates a std::string variable that contains
 * the xml generated by this function.
 * The XML is generated in this format:
 *
 * Tacktech
 *     Type TYPE=SET_VARIABLES
 *     Type
 *     Variables
 *			Playlist
 *			Playlist
 *			Groups_and_Computers
 *			Groups_and_Computers
 *			Group_Playlist
 *			Group_Playlist
 *     Variables
 * Tacktech
 *
 */
std::string Upload_Data_Container::set_variable_command_xml()
{
#ifdef _DEBUG
	std::cout << "= Upload_Data_Container::set_variable_command_xml()"
			<< std::endl;
#endif // _DEBUG
	std::string return_str;
	return_str += "<Tacktech>";
	return_str += "<Type TYPE=\"SET_VARIABLES\" />";
	return_str += "<Variables>";
	return_str += playlist->get_playlists_xml();
	return_str += groups->get_groups_and_computers_xml();
	return_str += group_playlist->get_group_playlist_xml();
	return_str += "</Variables>";
	return_str += "</Tacktech>";
#ifdef _DEBUG
	std::cout << "  - Set variable command:" << std::endl << return_str
			<< std::endl << "  - Set variable command c_str():" << std::endl
			<< return_str.c_str() << std::endl;
#endif // _DEBUG
	return return_str;
}

void Upload_Data_Container::set_group_playlist(
		Group_Playlist_Container_Ptr p_group_playlist)
{
	group_playlist = p_group_playlist;
}

/** @brief upload_file
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
std::string Upload_Data_Container::upload_file()
{
	//TODO MOVE THIS TO ARTEMIS_SERVER
#ifdef _DEBUG
	std::cout << "= Upload_Data_Container::get_xml_upload()" << std::endl;
#endif // _DEBUG
	pugi::xml_document transmit_document;
	pugi::xml_node root_node = transmit_document.append_child("Tacktech");
	pugi::xml_node type_node = root_node.append_child("Type");
	type_node.append_attribute("TYPE") = "UPLOAD";
	pugi::xml_node upload_node = type_node.append_child("Upload");
	upload_node.append_attribute("Upload_Time") =
			upload_time.toStdString().c_str();
	pugi::xml_node playlist_node = root_node.append_child("Playlist");
	playlist_node.append_attribute("PLAYLIST") =
			playlist_name.toStdString().c_str();
	Playlist_Range range2 = playlist->get_playlist()->equal_range(
			playlist_name.toStdString());
	Playlist_Multimap::iterator it2 = range2.first;
	for (it2; it2 != range2.second; ++it2)
	{
		pugi::xml_node item_node = playlist_node.append_child("Item");

		/* Creating item children */
		pugi::xml_node filename_node = item_node.append_child("Filename");
		pugi::xml_node file_data_node = item_node.append_child("File_Data");
		pugi::xml_node pause_node = item_node.append_child("Pause");

		/* Creating item children's pcdata */
		pugi::xml_node filename_pcdata = filename_node.append_child(
				pugi::node_pcdata);
		pugi::xml_node file_data_pcdata = file_data_node.append_child(
				pugi::node_pcdata);
		pugi::xml_node pause_pcdata = pause_node.append_child(
				pugi::node_pcdata);

		/* Giving pcdata a value */
		std::string temp_filename = it2->second.first;

		/* This will remove the path from the filename */
		if (temp_filename.find("\\") != std::string::npos)
			temp_filename = temp_filename.substr(
					temp_filename.find_last_of("\\") + 1);
		if (temp_filename.find("/") != std::string::npos)
			temp_filename = temp_filename.substr(
					temp_filename.find_last_of("/") + 1);

		filename_pcdata.set_value(temp_filename.c_str());
		file_data_pcdata.set_value(
				get_binary_file(QString::fromStdString(it2->second.first)).c_str());
		pause_pcdata.set_value(
				boost::lexical_cast<std::string>(it2->second.second).c_str());
	}
	xml_string_writer upload_writer;
	transmit_document.print(upload_writer);
	return upload_writer.result;
}

