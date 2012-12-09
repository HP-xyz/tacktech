#ifndef FILELIST_H
#define FILELIST_H
#ifdef _SHOW_DEBUG_OUTPUT
#include <iostream>
#endif // _SHOW_DEBUG_OUTPUT
#include <utility>
#include <Boost/shared_ptr.hpp>
#include <Boost/filesystem.hpp>
#include <Boost/regex.hpp>
typedef std::vector<std::pair<std::vector<std::string>, std::string> > Filelist_Data;
class Filelist
{
public:
	Filelist(std::map<std::string, std::string>&);
	~Filelist(void);
	void add_file(std::string, std::string);
#ifdef _SHOW_DEBUG_OUTPUT
	void print_contents();
#endif // _SHOW_DEBUG_OUTPUT
private:
	Filelist_Data filelist;
	std::map<std::string, std::string> parameters;

	void scan_playlist_directory();
	Filelist_Data::iterator get_iter_to_group(std::string);
	void add_new_file_and_organization(std::string);
};
typedef boost::shared_ptr<Filelist> Filelist_Ptr;
#endif //FILELIST_H