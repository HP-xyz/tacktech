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
Artemis_Request_Handler::Artemis_Request_Handler()
{

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
void Artemis_Request_Handler::handle_request( const std::string &request,
        std::vector<std::vector<char> > &reply,
        std::map<std::string, std::string> &parms)
{
#if _DEBUG
    std::cout << " = Handle Request " << std::endl;
    //std::cout << "  - Request: " << request << std::endl;
#endif
    try
    {
        return_messages = new std::vector< std::string >;
        if (request.size() > 0)
        {
            Artemis_Request_Handler::generate_queries(request);
            for (unsigned int i = 0; i < Artemis_Request_Handler::return_messages->size(); i++)
            {
                if (return_messages->at(i) == "DIRECT")
                {
                    //std::vector<char> result_xml(return_string.begin(),
                    //	return_string.end());
                    //reply.push_back(result_xml);
                }
            }
            if (return_messages->size() == 0)
                result_status = NO_RESULT;
            else if(return_messages->size() == 1)
                result_status = SINGLE_RESULT;
            else
                result_status = MULTIPLE_RESULTS;
        }
        else
        {
#if _DEBUG
            std::cout << " - No request received " << std::endl;
#endif
            result_status = NO_RESULT;
        }
        delete return_messages;
    }
    catch(std::exception &e)
    {
        if (return_messages == 0)
            delete return_messages;
        std::cerr << "EXCEPTION_SQL: " << e.what() << std::endl;
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
void Artemis_Request_Handler::generate_queries(const std::string &request)
{
#ifdef _DEBUG
    std::cout << " == Generate Queries " << std::endl;
#endif // _DEBUG
    document.load(request.c_str());
    pugi::xml_node tacktech = document.child("Tacktech");
	std::string type_string =
		tacktech.child("Type").attribute("TYPE").as_string();
#ifdef _DEBUG
	tacktech.print(std::cout);
#endif // _DEBUG
	if (type_string == "UPLOAD")
	{//Handle uploads
#ifdef _DEBUG
		std::cout << " - Received upload command" << std::endl;
#endif // _DEBUG
		pugi::xml_node upload = tacktech.child("Type").child("Upload");
		for (pugi::xml_node computer = upload.child("Computer"); computer;
			computer = computer.next_sibling("Computer"))
		{
			std::string dest_ip =
				computer.attribute("Computer_IP").as_string();
			
#ifdef _DEBUG
            std::cout << " - Sending to: " << dest_ip << std::endl;
#endif //_DEBUG
			xml_string_writer writer;
			computer.child("Item").print(writer);
			boost::thread upload_thread(boost::bind(
				&Artemis::Artemis_Request_Handler::handle_upload, this,
				writer.result, dest_ip));
			
		}
	}

}

/** Function handles the upload of data to a display client. Receives
 ** the xml to upload in parameter, as well as the ip to that the upload
 ** should be sent to.*/
void Artemis_Request_Handler::handle_upload(
	std::string upload_xml, std::string dest_ip )
{
#ifdef _DEBUG
	std::cout << "= Artemis_Request_Handler::handle_upload()" << std::endl;
#endif // _DEBUG
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::resolver resolver(io_service);
	boost::asio::ip::tcp::resolver::query query(dest_ip,
		"9000");
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator =
		resolver.resolve(query);
	Artemis_Network_Sender network_sender(io_service, endpoint_iterator);
#ifdef _DEBUG
	std::cout << " - Calling write" << std::endl;
#endif // _DEBUG
	network_sender.write(upload_xml);
}
}
