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
		Playlist_Container_Ptr p_playlist,
		Display_Client_Container_Ptr p_display_client_container,
		Filelist_Ptr p_filelist)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Artemis_Request_Handler::Artemis_Request_Handler()"
			<< std::endl;
#endif //_DEBUG
	playlist_container = p_playlist;
	display_client_container = p_display_client_container;
	filelist = p_filelist;
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - display_client_container.get() ---> " << display_client_container.get() << std::endl;
#endif // _SHOW_DEBUG_OUPTU
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
	}
	catch (std::exception &e)
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
	if (type_string == "FILE_UPLOAD")
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
#ifdef _IMPORTANT_OUTPUT
        std::cout << " - Received GET_VARIABLES command from Tacktech_Manager" << std::endl;
        //tacktech.print(std::cout);
#endif // _IMPORTANT_OUTPUT
        if(display_client_container.get() != 0)
        {
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
            upload_xml += "<Tacktech>";
            upload_xml += "<Type TYPE=\"SET_VARIABLES\" />";
            //upload_xml += playlist_container->get_playlist_container_xml(organization_name);
#ifdef _SHOW_DEBUG_OUTPUT
            std::cout << "  - Adding display_client_container XML" << std::endl;
#endif // _DEBUG
            upload_xml += display_client_container->get_display_client_container_xml(organization_name);
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << "  - Adding filelist XML" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
			filelist.reset(new Filelist(parameters));
			filelist->scan_playlist_directory();
			upload_xml += filelist->get_filelist_xml(organization_name);
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << "  - Sending: Upload_XML" << std::endl;
            std::cout << upload_xml << std::endl;
#endif //_SHOW_DEBUG_OUTPUT
            return_xml->append(upload_xml);
            result_status = SINGLE_RESULT;
        }
	}
	else if (type_string == "UPDATE_DISPLAY_CONTAINER")
    {
#ifdef _IMPORTANT_OUTPUT
		std::cout << " - Received UPDATE_DISPLAY_CONTAINER command" << std::endl;
#endif // _IMPORTANT_OUTPUT
        xml_string_writer writer;
        pugi::xml_node container_node = tacktech.child("CONTAINER").child("Display_Client_Container");
        container_node.print(writer);
        Display_Client_Container container(writer.result);
        display_client_container->update_groups_and_playlist(container);
    }
	else if (type_string == "UPDATE_PLAYLIST_CONTAINER")
	{
#ifdef _IMPORTANT_OUTPUT
		std::cout << " - Received UPDATE_PLAYLIST_CONTAINER command" << std::endl;
#endif // _IMPORTANT_OUTPUT
		//TODO, CONSIDER REMOVING THIS
		//xml_string_writer writer;
		//pugi::xml_node container_node = tacktech.child("CONTAINER").child("Playlist_Container");
		//container_node.print(writer);
		//Playlist_Container container(writer.result);
		//playlist_container->update_playlist(container,
		//	tacktech.child("Organization").attribute("ORGANIZATION_NAME").as_string());
	}
	else if (type_string == "UPDATE_DISPLAY_CLIENT")
	{
#ifdef _IMPORTANT_OUTPUT
		std::cout << " - Received UPDATE_DISPLAY_CLIENT command from " << tacktech.child("Identification").attribute("Computer_Name").as_string() << std::endl;
#endif // _IMPORTANT_OUTPUT
		std::string organization_name = tacktech.child("Identification").attribute("ORGANIZATION").as_string();

		std::string upload_string = "<Tacktech>";
		upload_string += "<Type TYPE=\"UPDATE\" />";

		xml_string_writer writer;
		tacktech.child("CONTAINER").child("Display_Client_Item").print(writer);
		Display_Client client(writer.result);

		Display_Client_Ptr client_to_send =
			display_client_container->get_display_client(organization_name, client.get_identification());

		if(client_to_send.get() != 0)
		{
			/* We need to set the item that is currently playing on the display */
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << "CLIENT" << std::endl;
			client.print_contents();
			std::cout << "CLIENT_TO_SEND" << std::endl;
			client_to_send->print_contents();
#endif // _SHOW_DEBUG_OUTPUT	
			upload_string += "<Identification GROUPS=\"" + client_to_send->get_groups_string() + "\" />";
			upload_string += "<CONTAINER>";
			upload_string += "<Display_Client_Item>";
			upload_string += client_to_send->get_display_client_xml();
			upload_string += "<Display_Client_Item>";
			upload_string += "</CONTAINER>";
			upload_string += "</Tacktech>";
		}
		else
		{
#ifdef _IMPORTANT_OUTPUT
			std::cout << "CRITICAL: Server could not find the client that asked for an update." << std::endl
				<< "	This normally happens if the client asks for an update before it has" << std::endl
				<< "	identified itself to the server" << std::endl;
#endif // _IMPORTANT_OUTPUT
		}
		return_xml->append(upload_string);
		result_status = SINGLE_RESULT;
	}
	else if (type_string == "GET_UPDATES")
	{
#ifdef _IMPORTANT_OUTPUT
		std::cout << " - Received GET_UPDATES command from " << tacktech.child("Identification_Node").attribute("Identification").as_string() << std::endl;
#endif // _IMPORTANT_OUTPUT
		std::string identification =
			tacktech.child("Identification_Node").attribute("Identification").as_string();
		std::string organization_name =
			tacktech.child("Identification_Node").attribute("Organization").as_string();
		std::vector<std::string> remote_files;
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Remote needs files: " << std::endl;
#endif // _DEBUG
		for (pugi::xml_attribute_iterator it =
				tacktech.child("File_Node").attributes().begin();
				it != tacktech.child("File_Node").attributes().end(); it++)
		{
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << "  - " << it->value() << std::endl;
#endif // _DEBUG
			remote_files.push_back(it->value());
		}
		Display_Client_Ptr identified_display =
			display_client_container->get_display_client(organization_name, identification);
		if (identified_display.get() != 0)
		{
			std::vector<std::string> items_to_upload =
				identified_display->get_playlist_container()->get_needed_items(remote_files);
			if(items_to_upload.size() > 0)
			{
#ifdef _SHOW_DEBUG_OUTPUT
				std::cout << "   !!! Need to upload '" << items_to_upload[0] << "'" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
				pugi::xml_document upload_document;
				pugi::xml_node tacktech_node = upload_document.append_child("Tacktech");
				pugi::xml_node type_node = tacktech_node.append_child("Type");
				type_node.append_attribute("TYPE") = "UPLOAD";
				if (items_to_upload.size() > 0)
				{//There are items to upload			
					pugi::xml_node items_node = tacktech_node.append_child("Items_Node");
				
					/* I propose a hack here, which only allows one file to upload
					 * at a time. It should send a string postscript saying that
					 * there are more files to upload.*/
					//for (std::vector<std::string>::iterator it = items_to_upload.begin();
					//	it != items_to_upload.end(); ++it)
					//{
					pugi::xml_node item_node = items_node.append_child("Item");
					pugi::xml_node filename_node = item_node.append_child("Filename");
					pugi::xml_node file_data_node = item_node.append_child("File_Data");

					pugi::xml_node filename_pcdata = filename_node.append_child(
						pugi::node_pcdata);
					pugi::xml_node file_data_pcdata = file_data_node.append_child(
						pugi::node_pcdata);

					filename_pcdata.set_value(items_to_upload[0].c_str());
					file_data_pcdata.set_value(
						filelist->get_binary_file(organization_name, items_to_upload[0]).c_str());
					//}
					pugi::xml_node status_node = tacktech_node.append_child("Status");
					status_node.append_attribute("STATUS") = "MORE_ITEMS";
				}
				else
				{
#ifdef _IMPORTANT_OUTPUT
					std::cout << "WARNING: Could not find items to upload (Display playlist probably out of sync)" << std::endl;
#endif //_IMPORTANT_OUTPUT
				}
				xml_string_writer writer;
				upload_document.print(writer);
				return_xml->append(writer.result);
				result_status = SINGLE_RESULT;
			}
		}
		else
		{
#ifdef _IMPORTANT_OUTPUT
			std::cout << " CRITICAL ERROR: Could not find display '" << identification << "'" << std::endl;
#endif // _IMPORTANT_OUTPUT
		}
	}
	else if (type_string == "IDENTIFY")
	{
#ifdef _IMPORTANT_OUTPUT
		std::cout << " - Received IDENTIFY command" << std::endl;
		std::cout << "  ## Organization_Name: " << tacktech.child("Identity").attribute("Organization_Name").as_string() << std::endl;
		std::cout << "  ## Computer_Name: " << tacktech.child("Identify").attribute("Computer_Name").as_string() << std::endl;
#endif // _IMPORTANT_OUTPUT
		pugi::xml_node indentification_node = tacktech.child("Identity");

		xml_string_writer writer;
		tacktech.child("CONTAINER").child("Playlist_Container").print(writer);
		std::set<std::string> organization_name_set;
		organization_name_set.insert(indentification_node.attribute("Organization_Name").as_string());

		Playlist_Container_Ptr temp_container(new Playlist_Container(writer.result, organization_name_set));
		std::cout << "Temp_Container current playing item:" << temp_container->get_current_playing_item() << std::endl;
		Display_Client_Ptr display_client(new Display_Client());

		display_client->add_organization(indentification_node.attribute("Organization_Name").as_string());
		display_client->set_identification(indentification_node.attribute("Computer_Name").as_string());
		display_client->set_last_ping(boost::posix_time::second_clock::universal_time());
		display_client_container->add_display_client(display_client);
		display_client_container->find_display_client_by_ident(indentification_node.attribute("Computer_Name").as_string())
			->get()->get_playlist_container()->update_playlist_on_server(*temp_container, indentification_node.attribute("Organization_Name").as_string());

#ifdef _IMPORTANT_OUTPUT
		std::cout << " # Display_Client: '" << indentification_node.attribute("Organization_Name").as_string() 
			<< "' is playing '" << display_client_container->find_display_client_by_ident(indentification_node.attribute("Computer_Name").as_string())
			->get()->get_playlist_container()->get_current_playing_item() << "'" << std::endl;
#endif // _IMPORTANT_OUTPUT

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
// */
//std::string Artemis_Request_Handler::get_binary_file(std::string filename)
//{
//#ifdef _SHOW_DEBUG_OUTPUT
//	std::cout << "= Artemis_Request_Handler::get_binary_file()" << std::endl;
//	std::cout << " - Getting file: " << filename << std::endl;
//#endif // _DEBUG
//	std::string file_encoded;
//	std::string test;
//	std::ifstream file(filename.c_str(), std::ios::binary);
//	if (file.is_open())
//	{
//#ifdef _SHOW_DEBUG_OUTPUT
//		std::cout << " - File is open" << std::endl;
//		file.seekg(0, std::ios::end);
//		std::cout << " - Tellg(): " << file.tellg() << std::endl;
//		file.seekg(0, std::ios::beg);
//#endif // _DEBUG
//		std::stringstream *file_in = new std::stringstream();
//		*file_in << file.rdbuf();
//		std::cout << "file_in size: " << file_in->str().size() << std::endl;
//		*file_in << std::ends;
//		base64::encoder E;
//		std::stringstream file_out;
//		E.encode(*file_in, file_out);
//		delete file_in;
//		file_out << std::ends;
//		std::cout << "file_out size: " << file_out.str().size() << std::endl;
//		file_encoded = file_out.str();
//#ifdef _SHOW_DEBUG_OUTPUT
//		std::cout << " - Encoded filesize: " << file_encoded.size()
//				<< std::endl;
//#endif // _DEBUG
//		file.close();
//	}
//	return (file_encoded);
//}

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
	std::string organization_name = tacktech.attribute("ORGANIZATION_NAME").as_string();
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Organization_Name: " << organization_name << std::endl;
#endif // _DEBUG
	bool successfull_save = false;
	for (pugi::xml_node item_node =
		tacktech.child("Upload").child("Item");
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

		filename = organization_name + "_" + filename;
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << "   - New Filename: " << filename << std::endl;
#endif
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
			//Add to server_filelist here
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
