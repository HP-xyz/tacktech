#ifndef TACKTECH_NETWORK_MANAGER_H
#define TACKTECH_NETWORK_MANAGER_H
#include <QObject>
#include <QString>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/foreach.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/lexical_cast.hpp>
#include <pugixml.hpp>
#include <exception>
#include <vector>
#include <list>
#include <iostream>

class Tacktech_Network_Manager: public QObject,
	public boost::enable_shared_from_this<
	Tacktech_Network_Manager>, private boost::noncopyable
{
	Q_OBJECT
public:
	explicit Tacktech_Network_Manager(
		boost::asio::io_service &io_service,
		std::map<std::string, std::string>&);
	virtual ~Tacktech_Network_Manager();

	void connect(std::string dest_ip, std::string dest_port);
	void start_write(boost::shared_ptr<std::string> _xml_string);
	void do_close();
private:
	void read();
	/** Handle completion of a write operation */
	void handle_write(const boost::system::error_code& error, std::size_t);
	void handle_read(const boost::system::error_code& error,
		std::size_t bytes_transferred);
	/** Strand to ensure the connections handlers are not called concurrently */
	boost::asio::io_service::strand strand;
	/** Socket for the connection */
	boost::shared_ptr<boost::asio::ip::tcp::socket> m_socket;

	/** Buffer for incoming data */
	boost::asio::streambuf buffer;

	std::map<std::string, std::string> parms;
	boost::shared_ptr<std::string> xml_string;
	unsigned long long sent_buffer_count;
	unsigned long long start_index;
	unsigned long long bytes_sent;
	void do_write();
	void do_connect(boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
	void handle_connect(const boost::system::error_code &error);
signals:
	void data_recieved(QString xml_data);
};
typedef boost::shared_ptr<Tacktech_Network_Manager> Tacktech_Network_Manager_Ptr;

#endif // TACKTECH_NETWORK_MANAGER_H
