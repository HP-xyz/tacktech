#include "pugixml.hpp"

#include <string.h>
#include <iostream>

int main()
{
	pugi::xml_document doc;
	if (!doc.load_file("xgconsole.xml"))
		return -1;

	pugi::xml_node tools = doc.child("Profile").child("Tools");

	//[code_traverse_base_basic
	for (pugi::xml_node tool = tools.first_child(); tool;
			tool = tool.next_sibling())
	{
		std::cout << "Tool:";

		for (pugi::xml_attribute attr = tool.first_attribute(); attr; attr =
				attr.next_attribute())
		{
			std::cout << " " << attr.name() << "=" << attr.value();
		}

		std::cout << std::endl;
	}
	//]

	std::cout << std::endl;

	//[code_traverse_base_data
	for (pugi::xml_node tool = tools.child("Tool"); tool;
			tool = tool.next_sibling("Tool"))
	{
		std::cout << "Tool " << tool.attribute("Filename").value();
		std::cout << ": AllowRemote "
				<< tool.attribute("AllowRemote").as_bool();
		std::cout << ", Timeout " << tool.attribute("Timeout").as_int();
		std::cout << ", Description '" << tool.child_value("Description")
				<< "'\n";
	}
	//]

	std::cout << std::endl;

	//[code_traverse_base_contents
	std::cout << "Tool for *.dae generation: "
			<< tools.find_child_by_attribute("Tool", "OutputFileMasks", "*.dae").attribute(
					"Filename").value() << "\n";

	for (pugi::xml_node tool = tools.child("Tool"); tool;
			tool = tool.next_sibling("Tool"))
	{
		std::cout << "Tool " << tool.attribute("Filename").value() << "\n";
	}
	//]
}

// vim:et
