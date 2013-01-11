/*
 * File:   Artemis_Request_Handler.h
 * Author: dvorak
 *
 * Created on September 11, 2011, 12:12 PM
 */

#ifndef ARTEMIS_REQUEST_HANDLER_H
#define	ARTEMIS_REQUEST_HANDLER_H
#ifdef _SHOW_DEBUG_OUTPUT
#define _IMPORTANT_OUTPUT _SHOW_DEBUG_OUTPUT
#endif // _SHOW_DEBUG_OUTPUT
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <map>
#include <utility>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <pugixml.hpp>
#include <b64/decode.h>
#include <b64/encode.h>
#include <Group_Container.h>
#include <Playlist_Container.h>
#include <Display_Client_Container.h>
#include <Group_Playlist_Container.h>
#include <Filelist.h>
namespace Artemis
{
enum STATUS
{
	NO_RESULT, MULTIPLE_RESULTS, SINGLE_RESULT
};
class Artemis_Request_Handler: private boost::noncopyable
{
public:
	/** Construct Artemis_Request_Handler object */
	explicit Artemis_Request_Handler(
			Playlist_Container_Ptr p_playlist,
			Display_Client_Container_Ptr p_display_client_container,
			Filelist_Ptr p_filelist);
	~Artemis_Request_Handler();

	/** Handle a request and produce a reply*/
	void handle_request(const std::string&,
			boost::shared_ptr<std::string> return_xml,
			std::map<std::string, std::string>&);

	STATUS result_status;
private:
	/** Result xml document to be returned */
	pugi::xml_document document;

	void handle_upload(std::string upload_xml, std::string dest_ip,
			std::string dest_port);

	/** Function to parse received XML to find queries to be run */
	void generate_queries(const std::string&, boost::shared_ptr<std::string> return_xml);

	bool save_uploaded_file(pugi::xml_node tacktech);
	//std::string get_binary_file(std::string filename);

	std::map<std::string, std::string> parameters;
	Filelist_Ptr filelist;


	/* Variable for the playlist */
	Playlist_Container_Ptr playlist_container;

	Display_Client_Container_Ptr display_client_container;
};
}
#endif	/* ARTEMIS_REQUEST_HANDLER_H */

