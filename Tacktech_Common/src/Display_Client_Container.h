#ifndef DISPLAY_CLIENT_CONTAINER_H
#define DISPLAY_CLIENT_CONTAINER_H
#include <vector>
#include <boost/shared_ptr.hpp>
#include "Display_Client.h"
#include "Playlist_Container.h"
#ifdef _SHOW_DEBUG_OUTPUT
#include <iostream>
#endif // _SHOW_DEBUG_OUTPUT
class Display_Client_Container
{
public:
	Display_Client_Container(void);
	Display_Client_Container(std::string);
	~Display_Client_Container(void);
	/** Returns a std::vector containing the Display_Client_Container*/
	std::vector<Display_Client_Ptr> get_display_client_container();
	/** Returns a std::vector containing the Display_Clients that belong
	 ** to the group specified in the parameter. */

	/** Adds a Display_Client to the vector. If a Display_Client with the
	 ** same identification already exists in the Display_Client_Container,
	 ** the aforementioned Display_Client's last ping will be updated.*/
	void add_display_client(Display_Client_Ptr);
	/** Removes a Display_Client at a certain index */
	void remove_display_client(int);

	/** Returns a std::string representation of the Display_Client_Container,
	 ** containing only the Display_Clients that belong to a certain organization. */
	std::string get_display_client_container_xml(std::string);
#ifdef _SHOW_DEBUG_OUTPUT
	void print_contents();
#endif // _SHOW_DEBUG_OUTPUT
private:
	std::vector<Display_Client_Ptr> m_display_client_container;
	/** Searches the display_client_container for a Display_CLient
	 ** identified by the string sent as parameter. Retuns iterator
	 ** to the Dislpay_Client if found, else return iterator to
	 ** m_display_client_container.end(). */
	std::vector<Display_Client_Ptr>::iterator find_display_client_by_ident(std::string);
};
typedef boost::shared_ptr<Display_Client_Container> Display_Client_Container_Ptr;
#endif //DISPLAY_CLIENT_CONTAINER_H

