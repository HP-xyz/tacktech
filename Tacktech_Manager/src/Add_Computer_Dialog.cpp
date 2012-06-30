#include "Add_Computer_Dialog.h"

Add_Computer_Dialog::Add_Computer_Dialog(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	connect(ui.add_computer_buttonbox, SIGNAL(accepted()),
		this , SLOT(ok_clicked()));
	connect(ui.add_computer_buttonbox, SIGNAL(rejected()),
		this , SLOT(cancel_clicked()));
}

Add_Computer_Dialog::~Add_Computer_Dialog()
{
}

/** Function will set the computer_names variable to the value the calling
 ** class provides */
void Add_Computer_Dialog::set_groups_and_computer_names(
	Group_Container *p_groups_and_computer_names)
{
#ifdef _DEBUG
	p_groups_and_computer_names->print_contents();
#endif // _DEBUG
	groups_and_computer_names = p_groups_and_computer_names;
}

/** Function to handle the accepted() signal from the class buttonbox */
void Add_Computer_Dialog::ok_clicked()
{
#ifdef _DEBUG
	std::cout << "= Add_Computer_Dialog::ok_clicked()" << std::endl;
#endif // _DEBUG
	if (groups_and_computer_names->add_computer_name(group_name,
		ui.computer_name_line_edit->text()))
	{
		emit computer_name_added();
		this->close();
	}
	else
	{
#ifdef _DEBUG
		std::cout << " - Computer names already contain provided value" 
			<< std::endl;
#endif // _DEBUG
		QMessageBox msgBox;
		msgBox.setText("That computer name is already in the list");
		msgBox.exec();
	}
}
/** Function to handle the rejected() signal from the class buttonbox */
void Add_Computer_Dialog::cancel_clicked()
{
	this->close();
}

void Add_Computer_Dialog::set_group_name( QString selected_group_name)
{
	group_name = selected_group_name;
}
