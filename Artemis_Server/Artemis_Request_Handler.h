/* 
 * File:   Artemis_Request_Handler.h
 * Author: dvorak
 *
 * Created on September 11, 2011, 12:12 PM
 */

#ifndef ARTEMIS_REQUEST_HANDLER_H
#define	ARTEMIS_REQUEST_HANDLER_H
#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <pugixml.hpp>
#include "Artemis_SQL_Handler.h"
namespace Artemis
{
enum STATUS {NO_RESULT, MULTIPLE_RESULTS, SINGLE_RESULT}; 
class Artemis_Request_Handler : private boost::noncopyable
{
public:
    /** Construct Artemis_Request_Handler object */
    explicit Artemis_Request_Handler();
    
    /** Handle a request and produce a reply*/
    void handle_request(const std::string&,
	std::vector<std::vector<char> >&, std::map<std::string, std::string>&);

	STATUS result_status;
private:
    /** SQL query to run*/
    std::string sql_query;
	/** Result xml document to be returned */
    pugi::xml_document document;
	/** Vector of SQL queries to be executed */
	std::vector< std::pair<std::string, std::string> > *queries;

	/** Function to parse received XML to find queries to be run */
	void generate_queries(const std::string&);
};
}
#endif	/* ARTEMIS_REQUEST_HANDLER_H */

