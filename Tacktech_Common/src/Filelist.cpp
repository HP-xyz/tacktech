#include "Filelist.h"

/** Struct to define a xml_writer to string.
 ** Copied directly from the pugixml quickstart */
struct xml_string_writer: pugi::xml_writer
{
	std::string result;

	virtual void write(const void* data, size_t size)
	{
		result += std::string(static_cast<const char*>(data), size);
	}
};

Filelist::Filelist(std::map<std::string, std::string>& params)
{
	filelist.reset(new Filelist_Data());
	parameters = params;
	scan_playlist_directory();
}

Filelist::Filelist( std::string xml_string)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Filelist()" << std::endl;
	std::cout << "Recieved String: " << std::endl;
	std::cout << xml_string << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	filelist.reset(new Filelist_Data());
	pugi::xml_document doc;
	doc.load(xml_string.c_str());
	pugi::xml_node filelist_node = doc.child("Filelist").child("Filelist_Items");
	std::string organization_name = doc.child("Filelist").child("Organization").attribute("Organization_Name").as_string();
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Organization_Name: " << organization_name << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	for (pugi::xml_attribute attr = filelist_node.first_attribute(); attr; attr = attr.next_attribute())
	{
			add_file(attr.value(), organization_name);
	}
}


Filelist::~Filelist(void)
{
}

void Filelist::scan_playlist_directory()
{
	filelist->clear();
	boost::filesystem::path current_dir(parameters["general.playlist_directory"]);
	for (boost::filesystem::recursive_directory_iterator iter(current_dir), end;
		iter != end;
		++iter)
	{
		std::string name = iter->path().generic_string();
		Filelist_Data::iterator it
			= get_iter_to_organization(name.substr(0, name.find("_")));
		if (it == filelist->end())
		{//Group does not exist in filelist
			add_new_file_and_organization(name);
		}
		else
		{
			it->first.push_back(name.substr(name.find("_") + 1));
			it->second = name.substr(0, name.find("_"));
		}
	}
#ifdef _SHOW_DEBUG_OUTPUT
	print_contents();
#endif // _SHOW_DEBUG_OUTPUT
}

Filelist_Data::iterator Filelist::get_iter_to_organization( std::string organization_name)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=get_iter_to_organization()" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	for (Filelist_Data::iterator it = filelist->begin();
		it != filelist->end(); ++it)
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " -- " << it->second << " <-> " << organization_name << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
		if(it->second == organization_name)
			return it;
	}
	return filelist->end();
}

void Filelist::add_new_file_and_organization( std::string full_filename)
{
	std::string filename = full_filename.substr(full_filename.find("_") + 1);
	std::string organization = full_filename.substr(0, full_filename.find("_"));
	std::vector<std::string> temp_vector;
	temp_vector.push_back(filename);
	filelist->push_back(std::pair<std::vector<std::string>,
		std::string>(temp_vector, organization));
}

#ifdef _SHOW_DEBUG_OUTPUT
void Filelist::print_contents()
{
	std::cout << "=Filelist::print_contents()" << std::endl;
	for (Filelist_Data::iterator it = filelist->begin();
		it != filelist->end(); ++it)
	{
		std::cout << " - Organization: " << it->second << std::endl;
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
	Filelist_Data::iterator it = get_iter_to_organization(organization_name);
	if (it == filelist->end())
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

std::string Filelist::get_filelist_xml( std::string organization_name)
{
	pugi::xml_document doc;
	pugi::xml_node root_node = doc.append_child("Filelist");
	pugi::xml_node filelist_node = root_node.append_child("Filelist_Items");
	pugi::xml_node organization_node = root_node.append_child("Organization");
	organization_node.append_attribute("Organization_Name") = organization_name.c_str();
	Filelist_Data::iterator it = get_iter_to_organization(parameters["general.playlist_directory"] + organization_name);
	if(it != filelist->end())
	{
        for (std::vector<std::string>::iterator it2 = it->first.begin();
            it2 != it->first.end(); ++it2)
        {
            filelist_node.append_attribute("Filename") = it2->substr(it2->find("_") + 1).c_str();
        }
	}
	xml_string_writer writer;
	doc.print(writer);
	return writer.result;
}

std::vector<std::string> Filelist::get_filelist( std::string organization_name)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=get_filelist()" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	std::vector<std::string> return_vector;
	Filelist_Data::iterator it = get_iter_to_organization(organization_name);
	if (it != filelist->end())
	{
		for (std::vector<std::string>::iterator it2 = it->first.begin();
			it2 != it->first.end(); ++it2)
		{
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << " - Adding: " << *it2  << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
			return_vector.push_back(*it2);
		}
	}
	return return_vector;
}

std::string Filelist::get_binary_file( std::string organization_name, std::string filename)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Filelist::get_binary_file()" << std::endl;
	std::cout << " - Getting file: " << filename << std::endl;
#endif // _DEBUG
	std::string file_encoded;
	std::string test;
	std::ifstream file(get_true_filename(organization_name, filename).c_str(), std::ios::binary);
	if (file.is_open())
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - File is open" << std::endl;
		file.seekg(0, std::ios::end);
		std::cout << " - Tellg(): " << file.tellg() << std::endl;
		file.seekg(0, std::ios::beg);
#endif // _DEBUG
		std::stringstream *file_in = new std::stringstream();
		*file_in << file.rdbuf();
		std::cout << "file_in size: " << file_in->str().size() << std::endl;
		*file_in << std::ends;
		base64::encoder E;
		std::stringstream file_out;
		E.encode(*file_in, file_out);
		delete file_in;
		file_out << std::ends;
		std::cout << "file_out size: " << file_out.str().size() << std::endl;
		file_encoded = file_out.str();
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Encoded filesize: " << file_encoded.size()
			<< std::endl;
#endif // _DEBUG
		file.close();
	}
	return (file_encoded);
}

std::string Filelist::get_true_filename( std::string organization_name, std::string filename)
{
	return parameters["general.playlist_directory"] + organization_name + "_" + filename;
}

