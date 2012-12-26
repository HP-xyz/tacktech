#ifndef FILELIST_H
#define FILELIST_H
#ifdef _SHOW_DEBUG_OUTPUT
#include <iostream>
#endif // _SHOW_DEBUG_OUTPUT
#include <utility>
#include <vector>
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
#ifdef _SHOW_DEBUG_OUTPUT
	void print_contents();
#endif // _SHOW_DEBUG_OUTPUT
private:
	boost::shared_ptr<Filelist_Data> filelist;
	std::map<std::string, std::string> parameters;

	void scan_playlist_directory();
	Filelist_Data::iterator get_iter_to_organization(std::string);
	void add_new_file_and_organization(std::string);
};
typedef boost::shared_ptr<Filelist> Filelist_Ptr;
#endif //FILELIST_H