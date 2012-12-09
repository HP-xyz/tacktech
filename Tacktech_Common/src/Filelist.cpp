#include "Filelist.h"


Filelist::Filelist(std::map<std::string, std::string>& params)
{
	parameters = params;
	scan_playlist_directory();
}


Filelist::~Filelist(void)
{
}

void Filelist::scan_playlist_directory()
{
	boost::filesystem::path current_dir(parameters["general.playlist_directory"]);
	for (boost::filesystem::recursive_directory_iterator iter(current_dir), end;
		iter != end;
		++iter)
	{
		std::string name = iter->path().generic_string();
		Filelist_Data::iterator it 
			= get_iter_to_group(name.substr(0, name.find("_")));
		if (it == filelist.end())
		{//Group does not exist in filelist
			add_new_file_and_organization(name);
		}
		else
		{
			it->first.push_back(name.substr(name.find("_") + 1));
			it->second = name.substr(0, name.find("_"));
		}
	}
}

Filelist_Data::iterator Filelist::get_iter_to_group( std::string organization_name)
{
	for (Filelist_Data::iterator it = filelist.begin();
		it != filelist.end(); ++it)
	{
		if(it->second == organization_name)
			return it;
	}
	return filelist.end();
}

void Filelist::add_new_file_and_organization( std::string full_filename)
{
	std::string filename = full_filename.substr(full_filename.find("_") + 1);
	std::string organization = full_filename.substr(0, full_filename.find("_"));
	std::vector<std::string> temp_vector;
	temp_vector.push_back(filename);
	filelist.push_back(std::pair<std::vector<std::string>,
		std::string>(temp_vector, organization));
}

#ifdef _SHOW_DEBUG_OUTPUT
void Filelist::print_contents()
{
	std::cout << "=Filelist::print_contents()" << std::endl;
	for (Filelist_Data::iterator it = filelist.begin();
		it != filelist.end(); ++it)
	{
		std::cout << " - Group: " << it->second << std::endl;
		for (std::vector<std::string>::iterator it2 = it->first.begin();
			it2 != it->first.end(); ++it2)
		{
			std::cout << "  - " << *it2 << std::endl;
		}
	}
}
#endif // _SHOW_DEBUG_OUTPUT

void Filelist::add_file( std::string filename, std::string organization_name)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Filelist::add_file" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	Filelist_Data::iterator it = get_iter_to_group(organization_name);
	if (it == filelist.end())
	{//Organization does not exist yet
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " ++ " << filename << " TO NEW ORG: " << organization_name << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
		std::string full_filename;
		full_filename += organization_name;
		full_filename += "_";
		full_filename += filename;
		add_new_file_and_organization(full_filename);
	}
	else
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " ++ " << filename << " TO EXISTING ORG: " << organization_name << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
		it->first.push_back(filename);
	}
}

