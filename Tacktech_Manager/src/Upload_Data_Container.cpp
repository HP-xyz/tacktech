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
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - (XML_WRITER)write of size: " << size << std::endl;
#endif // _DEBUG
		//result += static_cast<const char*>(data);
		result += std::string(static_cast<const char*>(data), size);
	}
};

Upload_Data_Container::Upload_Data_Container(
		std::map<std::string, std::string>& p_parms)
{
	parameters = p_parms;
	//read_config();
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "Parameters: " << std::endl << "==================="
		<< std::endl;
	for (std::map<std::string,std::string>::iterator it =
		parameters.begin(); it != parameters.end(); it++)
	{
		std::cout << " - " << it->first << " = " << it->second << std::endl;
	}
#endif
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

void Upload_Data_Container::set_organization_name( QString p_org_name)
{
	organization_name = p_org_name;
}

/** @brief set_playlist
 *
 * Sets the playlist variable to that provided in the
 * parameter.
 */
void Upload_Data_Container::set_playlist(
		Playlist_Container_Ptr p_playlist_container)
{
	playlist_container = p_playlist_container;
}

/** @brief get_binary_file
 *
 * Reads a file into memory and returns a std::string to
 * the read file
 */
std::string Upload_Data_Container::get_binary_file(QString filename)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Upload_Data_Container::get_binary_file()" << std::endl;
#endif // _DEBUG
	std::string file_encoded;
	std::string test;
	std::ifstream file(filename.toStdString().c_str(), std::ios::binary);
	if (file.is_open())
	{
#ifdef _SHOW_DEBUG_OUTPUT
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
#ifdef _SHOW_DEBUG_OUTPUT
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
	read_config();
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
	else if (command == "FILE_UPLOAD")
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
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Upload_Data_Container::get_variables_request_xml()"
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
	pugi::xml_node organization_node = root_node.append_child("Organization");
	organization_node.append_attribute("ORGANIZATION_NAME") =
		parameters["general.organization_name"].c_str();

	xml_string_writer writer;
	transmit_document.print(writer);
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "  - Sending variable request:" << std::endl << writer.result
			<< std::endl;
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
 *     Organization ORGANIZATION=ORGANGIZATION_NAME_HERE
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
//#ifdef _SHOW_DEBUG_OUTPUT
//	std::cout << "= Upload_Data_Container::set_variable_command_xml()"
//			<< std::endl;
//	std::cout << " -> ORGANIZATION = " 
//		<< parameters["general.organization_name"].c_str() << std::endl;
//#endif // _DEBUG
	std::string return_str;
//	return_str += "<Tacktech>";
//	return_str += "<Type TYPE=\"SET_VARIABLES\" />";
//	return_str += "<Organization ORGANIZATION_NAME=\"";
//	return_str += parameters["general.organization_name"].c_str();
//	return_str += "\"/>";
//	return_str += "<Variables>";
//	return_str += playlist->get_playlists_xml();
//	return_str += groups->get_groups_and_computers_xml();
//	return_str += group_playlist->get_group_playlist_xml();
//	return_str += "</Variables>";
//	return_str += "</Tacktech>";
//#ifdef _SHOW_DEBUG_OUTPUT
//	std::cout << "  - Set variable command:" << std::endl << return_str
//			<< std::endl << "  - Set variable command c_str():" << std::endl
//			<< return_str.c_str() << std::endl;
//#endif // _DEBUG
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
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Upload_Data_Container::get_xml_upload()" << std::endl;
#endif // _DEBUG
	pugi::xml_document transmit_document;
	pugi::xml_node root_node = transmit_document.append_child("Tacktech");
	pugi::xml_node type_node = root_node.append_child("Type");
	type_node.append_attribute("TYPE") = "FILE_UPLOAD";
	root_node.append_attribute("ORGANIZATION_NAME") =
		parameters["general.organization_name"].c_str();
	pugi::xml_node upload_node = root_node.append_child("Upload");
	for (std::vector<std::string>::iterator it = upload_items.begin();
		it != upload_items.end(); ++it)
	{
		pugi::xml_node item_node = upload_node.append_child("Item");

		/* Creating item children */
		pugi::xml_node filename_node = item_node.append_child("Filename");
		pugi::xml_node file_data_node = item_node.append_child("File_Data");

		/* Creating item children's pcdata */
		pugi::xml_node filename_pcdata = filename_node.append_child(
				pugi::node_pcdata);
		pugi::xml_node file_data_pcdata = file_data_node.append_child(
				pugi::node_pcdata);

		std::string temp_filename = *it;

		/* This will remove the path from the filename */
		if (temp_filename.find("\\") != std::string::npos)
			temp_filename = temp_filename.substr(
					temp_filename.find_last_of("\\") + 1);
		if (temp_filename.find("/") != std::string::npos)
			temp_filename = temp_filename.substr(
					temp_filename.find_last_of("/") + 1);

		/* Giving pcdata a value */
		filename_pcdata.set_value(temp_filename.c_str());
		file_data_pcdata.set_value(
				get_binary_file(QString::fromStdString(*it)).c_str());
	}
	xml_string_writer upload_writer;
	transmit_document.print(upload_writer);
	return upload_writer.result;
}

void Upload_Data_Container::read_config()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Tacktech_Manager::read_config()" << std::endl;
#endif // _DEBUG
	std::ifstream config("config.ini");
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Created config ifstream" << std::endl;
#endif // _DEBUG
	if (!config)
	{
		std::cerr << " == Error -> No config found" << std::endl;
	}
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Creating options.insert" << std::endl;
#endif // _DEBUG
	options.insert("*");
	try
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << "Config file read: " << std::endl << "==================="
			<< std::endl;
#endif
		for (boost::program_options::detail::config_file_iterator i(config,
			options), e; i != e; ++i)
		{
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << i->string_key << " " << i->value[0] << std::endl;
#endif
			parameters[i->string_key] = i->value[0];
		}
	} catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}

void Upload_Data_Container::set_display_client_container( Display_Client_Container_Ptr p_display_client_container)
{
	display_client_container = p_display_client_container;
}

void Upload_Data_Container::set_upload_items( std::vector<std::string> p_items)
{
	upload_items = p_items;
}
