/*
 * File:   Artemis_Request_Handler.cpp
 * Author: dvorak
 *
 * Created on September 11, 2011, 12:12 PM
 */

#include "Artemis_Request_Handler.h"
namespace Artemis
{

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

//************************************
// Method:    Artemis_Request_Handler
// FullName:  Artemis::Artemis_Request_Handler::Artemis_Request_Handler
// Access:    public
// Returns:
// Qualifier:
//************************************
Artemis_Request_Handler::Artemis_Request_Handler(
		Group_Container_Server_Ptr p_groups_and_computers,
		Playlist_Container_Ptr p_playlist,
		Group_Playlist_Container_Server_Ptr p_group_playlist,
		Display_Client_Container_Ptr p_display_client_container)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Artemis_Request_Handler::Artemis_Request_Handler()"
			<< std::endl;
#endif //_DEBUG
	groups_and_computers = p_groups_and_computers;
	playlist_container = p_playlist;
	group_playlist = p_group_playlist;
	display_client_container = p_display_client_container;
}

Artemis_Request_Handler::~Artemis_Request_Handler()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Artemis_Request_Handler::~Artemis_Request_Handler()"
			<< std::endl;
#endif //_DEBUG
}

//************************************
// Method:    handle_request
// FullName:  Artemis::Artemis_Request_Handler::handle_request
// Access:    public
// Returns:   void
// Qualifier:
// Parameter: const std::string & request
// Parameter: std::vector<std::vector<char> > & reply
// Parameter: std::map<std::string
// Parameter: std::string> & parms
//************************************
void Artemis_Request_Handler::handle_request(const std::string &request,
		boost::shared_ptr<std::string> return_xml,
		std::map<std::string, std::string> &parms)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " = Handle Request " << std::endl;
	//std::cout << "  - Request: " << request << std::endl;
#endif
	parameters = parms;
	try
	{
		if (request.size() > 0)
		{
			Artemis_Request_Handler::generate_queries(request, return_xml);
		}
		else
		{
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << " - No request received " << std::endl;
#endif
			result_status = NO_RESULT;
		}
	} catch (std::exception &e)
	{
		std::cerr << "Handle Request exception: " << e.what() << std::endl;
	}
}

//************************************
// Method:    generate_queries
// FullName:  Artemis::Artemis_Request_Handler::generate_queries
// Access:    private
// Returns:   void
// Qualifier:
// Parameter: const std::string & request
//************************************
void Artemis_Request_Handler::generate_queries(const std::string &request, boost::shared_ptr<std::string> return_xml)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " == Generate Queries " << std::endl;
#endif // _DEBUG
	document.load(request.c_str());
	pugi::xml_node tacktech = document.child("Tacktech");
	std::string type_string =
			tacktech.child("Type").attribute("TYPE").as_string();
	if (type_string == "UPLOAD")
	{
		//Handle uploads
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Received UPLOAD command" << std::endl;
#endif // _DEBUG
		std::string upload_xml;
		if(Artemis::Artemis_Request_Handler::save_uploaded_file(tacktech))
		{
			upload_xml += "<Tacktech>";
			upload_xml += "<Type TYPE=\"UPLOAD_RESULT\" />";
			upload_xml += "<Success SUCCESS=\"TRUE\" />";
			upload_xml += "</Tacktech>";
		}
		else
		{
			upload_xml += "<Tacktech>";
			upload_xml += "<Type TYPE=\"UPLOAD_RESULT\" />";
			upload_xml += "<Success SUCCESS=\"FALSE\" />";
			upload_xml += "</Tacktech>";
		}
		return_xml->append(upload_xml);
		result_status = SINGLE_RESULT;
	}
	else if (type_string == "GET_VARIABLES")
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Received GET_VARIABLES command" << std::endl;
		tacktech.print(std::cout);
#endif // _DEBUG
		std::string dest_ip =
				tacktech.child("Return_IP").attribute("IP").as_string();
		std::string dest_port =
				tacktech.child("Return_IP").attribute("PORT").as_string();
		std::string upload_xml;
		std::string organization_name =
				tacktech.child("Organization")
				.attribute("ORGANIZATION_NAME").as_string();
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << "  -> ORGANIZATION_NAME: " << organization_name 
			<< std::endl;
		std::cout << "  - Adding playlist_container XML" << std::endl;
#endif // _DEBUG
		return_xml->append(playlist_container->get_playlist_container_xml(organization_name));
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << "  - Adding display_client_container XML" << std::endl;
#endif // _DEBUG
		return_xml->append(display_client_container->get_display_client_container_xml(organization_name));
		result_status = SINGLE_RESULT;
	}
	else if (type_string == "SET_VARIABLES")
	{
//#ifdef _SHOW_DEBUG_OUTPUT
//		std::cout << " - Received SET_VARIABLES command" << std::endl;
//#endif // _DEBUG
//		xml_string_writer playlist_writer;
//		std::string organization_name =
//			tacktech.child("Organization")
//			.attribute("ORGANIZATION_NAME").as_string();
//		tacktech.child("Variables").
//			child("Playlist").print(playlist_writer);
//		playlist->construct_playlist(organization_name,
//			playlist_writer.result);
//
//#ifdef _SHOW_DEBUG_OUTPUT
//		std::cout << "  - ORGANIZATION: " << organization_name <<std::endl;
//		std::cout << "  - Printing new variables on server" << std::endl;
//		std::cout << "  ==================================" << std::endl;
//		std::cout << "   - Playlist" << std::endl;
//		std::cout << "   ==========" << std::endl;
//#endif // _DEBUG
//		playlist->get_organization_map()
//			[organization_name].print_contents();
//
//		xml_string_writer groups_and_computers_writer;
//		tacktech.child("Variables").child("Groups_And_Computers").print(
//				groups_and_computers_writer);
//		groups_and_computers->construct_groups_and_computers(
//			organization_name, groups_and_computers_writer.result);
//#ifdef _SHOW_DEBUG_OUTPUT
//		std::cout << "   - Groups_And_Computers" << std::endl;
//		std::cout << "   ======================" << std::endl;
//#endif // _DEBUG
//		groups_and_computers->get_organization_map()
//			[organization_name].print_contents();
//
//		xml_string_writer group_playlist_writer;
//		tacktech.child("Variables").child("Group_Playlist").print(
//				group_playlist_writer);
//		group_playlist->construct_group_playlist(organization_name,
//			group_playlist_writer.result);
	}
	else if (type_string == "GET_UPDATES")
	{
//#ifdef _SHOW_DEBUG_OUTPUT
//		std::cout << " - Received GET_UPDATES command" << std::endl;
//#endif // _DEBUG
//		std::string organization_name =
//			tacktech.child("Organization").attribute("Organization_Name").as_string();
//		pugi::xml_node has_files_node = tacktech.child("Files");
//		pugi::xml_node playlist_node = tacktech.child("Playlist");
//		std::map<std::string, std::string> filemap;
//#ifdef _SHOW_DEBUG_OUTPUT
//		std::cout << " - Remote has files: " << std::endl;
//#endif // _DEBUG
//		for (pugi::xml_attribute_iterator it =
//				has_files_node.attributes().begin();
//				it != has_files_node.attributes().end(); it++)
//		{
//#ifdef _SHOW_DEBUG_OUTPUT
//			std::cout << "  - " << it->value() << std::endl;
//#endif // _DEBUG
//			filemap.insert(
//					std::pair<std::string, std::string>(it->value(),
//							"FILE"));
//		}
//		Playlist_Range range = playlist->get_organization_map()
//			[organization_name].get_files_in_playlist(
//			playlist_node.attribute("PLAYLIST").as_string());
//		Playlist_Multimap::iterator it = range.first;
//		pugi::xml_document upload_document;
//		pugi::xml_node tacktech_node = upload_document.append_child("Tacktech");
//		pugi::xml_node type_node = tacktech_node.append_child("Type");
//		type_node.append_attribute("TYPE") = "UPLOAD";
//		upload_document.print(std::cout);
//		pugi::xml_node computer_node = tacktech_node.append_child("Computer");
//		for (it; it != range.second; ++it)
//		{
//			std::string temp_filename = it->second.first;
//
//			/* This will remove the path from the filename */
//			if (temp_filename.find("\\") != std::string::npos)
//				temp_filename = temp_filename.substr(
//				temp_filename.find_last_of("\\") + 1);
//			if (temp_filename.find("/") != std::string::npos)
//				temp_filename = temp_filename.substr(
//				temp_filename.find_last_of("/") + 1);
//			if (filemap.find(temp_filename) == filemap.end())
//			{ //Upload file here
//#ifdef _SHOW_DEBUG_OUTPUT
//				std::cout << " ++ " << temp_filename << std::endl;
//#endif // _DEBUG
//				pugi::xml_node item_node = computer_node.append_child("Item");
//
//				/* Creating item children */
//				pugi::xml_node filename_node = item_node.append_child("Filename");
//				pugi::xml_node file_data_node = item_node.append_child("File_Data");
//				pugi::xml_node pause_node = item_node.append_child("Pause");
//
//				/* Creating item children's pcdata */
//				pugi::xml_node filename_pcdata = filename_node.append_child(
//						pugi::node_pcdata);
//				pugi::xml_node file_data_pcdata = file_data_node.append_child(
//						pugi::node_pcdata);
//				pugi::xml_node pause_pcdata = pause_node.append_child(
//						pugi::node_pcdata);
//
//				/* Giving pcdata a value */
//				std::string true_filename = parameters["general.playlist_directory"];
//
//				true_filename += temp_filename;
//
//				filename_pcdata.set_value(temp_filename.c_str());
//				file_data_pcdata.set_value(
//						get_binary_file(true_filename).c_str());
//				pause_pcdata.set_value(
//						boost::lexical_cast<std::string>
//						(it->second.second).c_str());
//			}
//			else
//			{
//#ifdef _SHOW_DEBUG_OUTPUT
//				std::cout << " -- " << temp_filename << std::endl;
//#endif // _DEBUG
//			}
//		}
//		xml_string_writer writer;
//		upload_document.print(writer);
//		return_xml->append(writer.result);
//		result_status = SINGLE_RESULT;
	}
	else if (type_string == "IDENTIFY")
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Received IDENTIFY command" << std::endl;
#endif // _DEBUG
		pugi::xml_node indentification_node = tacktech.child("Identity");
		Display_Client_Ptr display_client(new Display_Client());
		display_client->add_group(indentification_node.attribute("Organization_Name").as_string());
		display_client->set_identification(indentification_node.attribute("Computer_Name").as_string());
		display_client->set_last_ping(boost::posix_time::second_clock::universal_time());
		display_client_container->add_display_client(display_client);

		std::string upload_xml;
		upload_xml += "<Tacktech>";
		upload_xml += "<Type TYPE=\"IDENTIFY\" />";
		upload_xml += "<Success SUCCESS=\"TRUE\">";
		upload_xml += "</Tacktech>";

		return_xml->append(upload_xml);
		result_status = SINGLE_RESULT;
	}
}

/** @brief get_binary_file
 *
 * Reads a file into memory and returns a std::string to
 * the read file
 */
std::string Artemis_Request_Handler::get_binary_file(std::string filename)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Artemis_Request_Handler::get_binary_file()" << std::endl;
	std::cout << " - Getting file: " << filename << std::endl;
#endif // _DEBUG
	std::string file_encoded;
	std::string test;
	std::ifstream file(filename.c_str(), std::ios::binary);
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
	return (file_encoded);
}

/** Function handles the upload of data to a IP. Receives
 ** the xml to upload in parameter, as well as the ip to that the upload
 ** should be sent to.*/
void Artemis_Request_Handler::handle_upload(std::string upload_xml,
		std::string dest_ip, std::string dest_port)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Artemis_Request_Handler::handle_upload()" << std::endl;
	std::cout << " ->>>> Dest_Ip: " << dest_ip << std::endl;
	std::cout << " ->>>> Dest_Port: " << dest_port << std::endl;
	std::cout << " - Sending file of size: " << upload_xml.size() << std::endl;
#endif // _DEBUG
	boost::shared_ptr<boost::asio::io_service> io_service(
			new boost::asio::io_service);
	/*Artemis_Network_Sender_Connection_ptr network_send_connector(
			new Artemis_Network_Sender_Connection(*io_service, parameters,
					upload_xml));
	network_send_connector->connect(dest_ip, dest_port);
	network_send_connector->start_write();
	boost::thread t(
			boost::bind(&boost::asio::io_service::run, boost::ref(io_service)));
	t.join();*/
}

bool Artemis_Request_Handler::save_uploaded_file(pugi::xml_node tacktech)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Artemis_Request_Handler::save_uploaded_file()" << std::endl;
#endif // _DEBUG
	bool successfull_save = false;
	std::string playlist_name =
			tacktech.child("Playlist").attribute("PLAYLIST").as_string();

	for (pugi::xml_node item_node =
		tacktech.child("Playlist").child("Item");
		item_node;
		item_node = item_node.next_sibling("Item"))
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << "  - Filename: " << item_node.child_value("Filename")
				<< std::endl;
		std::cout << "  - Pause: " << item_node.child_value("Pause") 
			<< std::endl;
#endif //_DEBUG
		std::string filename = item_node.child_value("Filename");
		std::string file_data = item_node.child_value("File_Data");
		std::stringstream encoded_stream;
		std::stringstream decoded_stream;

		encoded_stream << file_data;
		base64::decoder D;
		D.decode(encoded_stream, decoded_stream);
		file_data = decoded_stream.str();

#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << "   - File_Data size: " << file_data.size() << std::endl;
#endif
		std::string out_filename = parameters["general.playlist_directory"];
		out_filename += filename;

#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << "  - Saving to: " << out_filename.c_str() << std::endl;
#endif // _DEBUG
		std::ofstream out_file(out_filename.c_str(), std::ios::binary);
		if (out_file.good())
		{
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << "   - Output file is good" << std::endl;
#endif // _DEBUG
			successfull_save = true;
		}
		else
		{
#ifdef _SHOW_DEBUG_OUTPUT
			std::cerr << "   - Output file not good!" << std::endl;
#endif // _DEBUG
		}
		out_file << file_data;
		out_file.close();
	}
	return (successfull_save);
}

}
