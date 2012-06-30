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
	QMap<QString, QList<QString>* > *p_groups_and_computer_names)
{
#ifdef _DEBUG
	std::cout << " - Printing received computer names:" << std::endl;
	foreach(QString key, p_groups_and_computer_names->keys())
		foreach(QString computer_name,
			p_groups_and_computer_names->value(key))
			std::cout << "  - " << qPrintable(computer_name) << std::endl;
#endif // _DEBUG
	groups_and_computer_names = p_groups_and_computer_names;
}

/** Function to handle the accepted() signal from the class buttonbox */
void Add_Computer_Dialog::ok_clicked()
{
#ifdef _DEBUG
	std::cout << "= Add_Computer_Dialog::ok_clicked()" << std::endl;
	std::cout << " - Testing that computer is not already in group" 
		<< std::endl;
#endif // _DEBUG
	bool computer_already_in_group = false;
	foreach(QString key, groups_and_computer_names->keys())
		foreach(QString computer, groups_and_computer_names->value(key))
			if(ui.computer_name_line_edit->text() == computer 
				|| computer == nullptr)
				computer_already_in_group = true;
#ifdef _DEBUG
	std::cout << " - Computer in a group: " << computer_already_in_group
		<< std::endl;
#endif // _DEBUG
	if (!computer_already_in_group)
	{
		QList<QString> *new_list = new QList<QString>;
		groups_and_computer_names->insert(
			ui.computer_name_line_edit->text(), *new_list);
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

void Add_Computer_Dialog::set_group_index( int p_group_index)
{
	group_index = p_group_index;
}
