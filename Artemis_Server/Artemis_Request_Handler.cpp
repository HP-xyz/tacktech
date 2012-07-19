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
#ifdef _DEBUG
    std::cout << "=Artemis_Request_Handler::Artemis_Request_Handler()" << std::endl;
#endif //_DEBUG
}

Artemis_Request_Handler::~Artemis_Request_Handler()
{
#ifdef _DEBUG
    std::cout << "=Artemis_Request_Handler::~Artemis_Request_Handler()" << std::endl;
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
void Artemis_Request_Handler::handle_request( const std::string &request,
        std::vector<std::vector<char> > &reply,
        std::map<std::string, std::string> &parms)
{
#ifdef _DEBUG
    std::cout << " = Handle Request " << std::endl;
    //std::cout << "  - Request: " << request << std::endl;
#endif
    parameters = parms;
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
#ifdef _DEBUG
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
    if (type_string == "UPLOAD")
    {
        //Handle uploads
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
            computer.print(writer);
            boost::thread upload_thread(boost::bind(
                                            &Artemis::Artemis_Request_Handler::handle_upload, this,
                                            writer.result, dest_ip));
			upload_thread.join();
            //handle_upload(writer.result, dest_ip);

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
    std::cout << " - Dest_Ip: " << dest_ip << std::endl;
    std::cout << " - Sending file of size: " << upload_xml.size() << std::endl;
#endif // _DEBUG
    boost::shared_ptr<boost::asio::io_service> 
		io_service(new boost::asio::io_service);
    Artemis_Network_Sender_Connection_ptr network_send_connector(
        new Artemis_Network_Sender_Connection(*io_service, parameters,
		upload_xml));
    network_send_connector->connect(dest_ip);
    network_send_connector->start_write();
    boost::thread t(boost::bind(&boost::asio::io_service::run, boost::ref(io_service)));
    t.join();
}
}
