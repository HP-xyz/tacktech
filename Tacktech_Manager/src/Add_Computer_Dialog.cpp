#include "Add_Computer_Dialog.h"

Add_Computer_Dialog::Add_Computer_Dialog(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	computer_names = new QList<QString>;
	connect(ui.add_computer_buttonbox, SIGNAL(accepted()),
		this , SLOT(ok_clicked()));
	connect(ui.add_computer_buttonbox, SIGNAL(rejected()),
		this , SLOT(cancel_clicked()));
}

Add_Computer_Dialog::~Add_Computer_Dialog()
{
	delete computer_names;
}

/** Function will set the computer_names variable to the value the calling
 ** class provides */
void Add_Computer_Dialog::set_computer_names( QList<QString> &p_computer_names)
{
	std::cout << " - Printing received computer names:" << std::endl;
	for(int i = 0; i < p_computer_names.size(); i++)
		std::cout << "   - " << qPrintable(p_computer_names[i]) << std::endl;
	computer_names = &p_computer_names;
}

/** Function to handle the accepted() signal from the class buttonbox */
void Add_Computer_Dialog::ok_clicked()
{
	if (!computer_names->contains(ui.computer_name_line_edit->text()))
	{
		computer_names->append(ui.computer_name_line_edit->text());
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
