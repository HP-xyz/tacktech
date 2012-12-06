#ifndef DISPLAY_CLIENT_CONTAINER_H
#define DISPLAY_CLIENT_CONTAINER_H
#include <vector>
#include <boost/shared_ptr.hpp>
#include "Display_Client.h"
class Display_Client_Container
{
public:
	Display_Client_Container(void);
	~Display_Client_Container(void);
	/** Returns a std::vector containing the Display_Client_Container*/
	std::vector<Display_Client_Ptr> get_display_client_container();
	/** Adds a Display_Client to the vector */
	void add_display_client(Display_Client_Ptr);
	/** Removes a Display_Client at a certain index */
	void remove_display_client(int);
private:
	std::vector<Display_Client_Ptr> m_display_client_container;
};
typedef boost::shared_ptr<Display_Client_Container> Display_Client_Container_Ptr;
#endif //DISPLAY_CLIENT_CONTAINER_H

