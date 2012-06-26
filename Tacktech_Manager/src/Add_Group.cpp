#include "Add_Group.h"

Add_Group::Add_Group(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
#ifdef _DEBUG
	std::cout << "= Starting and setting up Add_Group GUI" << std::endl;
#endif // _DEBUG
	ui.setupUi(this);
	connect(ui.buttonBox, SIGNAL(accepted()),
		this, SLOT(ok_clicked()));
	connect(ui.buttonBox, SIGNAL(rejected()),
		this, SLOT(cancel_clicked()));
	this->setFocus();
}

Add_Group::~Add_Group()
{
#ifdef _DEBUG
	std::cout << "= Cleaning up Add_Group GUI" << std::endl;
#endif // _DEBUG
}
 /** Slot fired on the accepted() action. Will not validate input, input
  ** validation must be done in the calling class.*/
void Add_Group::ok_clicked()
{
#ifdef _DEBUG
	std::cout << " - ok_clicked slot activated" << std::endl;
#endif // _DEBUG
	emit add_group_input_complete(ui.group_name_line_edit->text());
	this->close();
}

/** Slot fired on the rejected() action. Will simply close the popup window
*/
void Add_Group::cancel_clicked()
{
#ifdef _DEBUG
	std::cout << " - cancel_clicked() slot activated" << std::endl;
#endif // _DEBUG
	this->close();
}
