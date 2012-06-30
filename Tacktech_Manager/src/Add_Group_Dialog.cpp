#include "Add_Group_Dialog.h"

Add_Group_Dialog::Add_Group_Dialog(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	connect(ui.add_group_dialog_buttonbox, SIGNAL(accepted()),
		this, SLOT(ok_clicked()));
	connect(ui.add_group_dialog_buttonbox, SIGNAL(rejected()),
		this, SLOT(cancle_clicked()));
}

Add_Group_Dialog::~Add_Group_Dialog()
{
}

/** Function to set the group names to the value provided by the calling
 ** class */
void Add_Group_Dialog::set_group_and_computer_names(
	Group_Container *p_group_names )
{
#ifdef _DEBUG
	p_group_names->print_contents();
#endif // _DEBUG
	group_and_computer_names = p_group_names;
}

/** Function to handle the accepted() signal from the class buttonbox */
void Add_Group_Dialog::ok_clicked()
{
	if(group_and_computer_names->
		add_group_name(ui.group_name_line_edit->text()))
	{
		/* Emitting the group_name_added signal that is to be caught by
		 * the calling program */
		emit group_name_added();
		this->close();
	}
	else
	{
#ifdef _DEBUG
		std::cout << " - Group names already contain provided value" 
			<< std::endl;
#endif // _DEBUG
		QMessageBox msgBox;
		msgBox.setText("That group name is already in the list");
		msgBox.exec();
	}
}
/** Function to handle the rejected() signal from the class buttonbox */
void Add_Group_Dialog::cancle_clicked()
{
	this->close();
}

