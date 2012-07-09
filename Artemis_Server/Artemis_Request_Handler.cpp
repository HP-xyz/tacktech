/*
 * File:   Artemis_Request_Handler.cpp
 * Author: dvorak
 *
 * Created on September 11, 2011, 12:12 PM
 */

#include "Artemis_Request_Handler.h"
namespace Artemis
{

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
    std::cout << "  - Request: " << request << std::endl;
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
    std::cout << "  - Request -> " << request << std::endl;
#endif // _DEBUG
    document.load(request.c_str());
#ifdef _DEBUG
    std::cout << "  -- Root: " << document.root() << std::endl;
#endif // _DEBUG
    pugi::xml_node tacktech = document.child("Tacktech");
	std::string type_string =
		tacktech.child("Type").attribute("TYPE").as_string();
	if (type_string == "UPLOAD")
	{//Handle uploads

	}

}
}