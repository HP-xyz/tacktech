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
        queries = new std::vector< std::pair<std::string, std::string> >;
        if (request.size() > 0)
        {
            Artemis_Request_Handler::generate_queries(request);
            for (unsigned int i = 0; i < Artemis_Request_Handler::queries->size(); i++)
            {
                if (queries->at(i).first == "DIRECT")
                {
                    //std::vector<char> result_xml(return_string.begin(),
                    //	return_string.end());
                    //reply.push_back(result_xml);
                }
            }
            if (queries->size() == 0)
                result_status = NO_RESULT;
            else if(queries->size() == 1)
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
        delete queries;
    }
    catch(std::exception &e)
    {
        if (queries == 0)
            delete queries;
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
    std::cout << "  -- Root.child(Artemis).name: " << document.child("Artemis").name() << std::endl;
    std::cout << "  -- Root.child(Artemis).child(query).name: " << document.child("Artemis").child("query").name() << std::endl;
#endif // _DEBUG
    pugi::xml_node artemis = document.child("Artemis");
    for (pugi::xml_node query = artemis.child("query"); query; query = query.next_sibling("query"))
    {
#ifdef _DEBUG
        std::cout << "   -> Type: " << query.child_value("type") << std::endl;
        std::cout << "   -> Data: " << query.child_value("data") << std::endl;
#endif // _DEBUG
    }
}

}
