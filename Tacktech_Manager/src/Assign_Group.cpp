#include "Assign_Group.h"

Assign_Group::Assign_Group( Display_Client_Container_Ptr p_display_client_container, std::vector<std::string> p_selected_names)
{
#ifdef _SHOW_DEBUG_OUPUT
	std::cout << "=Assign_Group::Assign_Group()" << std::endl;
#endif // _SHOW_DEBUG_OUPUT
	m_display_client_container = p_display_client_container;
	m_selected_names = p_selected_names;
	ui.setupUi(this);
	
	std::vector<std::string> unique_group_names = m_display_client_container->get_unique_group_names();
	for (int i = 0; i < unique_group_names.size(); ++i)
	{
		ui.group_combo_box->addItem(QString::fromStdString(unique_group_names[i]));
	}
	connect(ui.add_new_group_pushbutton, SIGNAL(clicked()), this, SLOT(new_group_added()));
}


Assign_Group::~Assign_Group(void)
{
}

void Assign_Group::new_group_added()
{
	if (ui.new_group_lineedit->text() != "")
	{
		ui.group_combo_box->addItem(ui.new_group_lineedit->text());
		ui.group_combo_box->currentText() = ui.new_group_lineedit->text();
	}
	else
	{
		QMessageBox messagebox;
		messagebox.setText("Please enter a group name");
		messagebox.show();
	}
}

void Assign_Group::ok_clicked()
{
	for(int i = 0; i < m_selected_names.size(); ++i)
	{
		std::vector<Display_Client_Ptr>::iterator it = 
			m_display_client_container->find_display_client_by_ident(m_selected_names[i]);
		if (it == m_display_client_container->get_display_client_container().end())
		{
			it->get()->add_group(m_selected_names[i]);
		}
	}
	this->close();
}

void Assign_Group::cancel_clicked()
{
	this->close();
}
