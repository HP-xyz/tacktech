#ifndef FILELIST_H
#define FILELIST_H
#include <iostream>
#include <utility>
#include <vector>
#include <sstream>
#include <fstream>
#include <b64/decode.h>
#include <b64/encode.h>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <pugixml.hpp>
typedef std::vector<std::pair<std::vector<std::string>, std::string> > Filelist_Data;
class Filelist
{
public:
	Filelist(std::map<std::string, std::string>&);
	Filelist(std::string);
	~Filelist(void);
	void add_file(std::string, std::string);
	std::string get_filelist_xml(std::string);
	std::vector<std::string> get_filelist(std::string);
	void scan_playlist_directory();

	/* Returns a std::string representation of a binary file,
	 * belonging to an organization identified by the first parameter
	 * and has a filename, identified by the second parameter */
	std::string get_binary_file(std::string, std::string);

#ifdef _SHOW_DEBUG_OUTPUT
	void print_contents();
#endif // _SHOW_DEBUG_OUTPUT
private:
	boost::shared_ptr<Filelist_Data> filelist;
	std::map<std::string, std::string> parameters;

	Filelist_Data::iterator get_iter_to_organization(std::string);
	void add_new_file_and_organization(std::string);

	std::string get_true_filename(std::string, std::string);
};
typedef boost::shared_ptr<Filelist> Filelist_Ptr;
#endif //FILELIST_H
