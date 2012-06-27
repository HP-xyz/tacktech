#include "Edit_Group.h"
/** Default constructor for the Edit_Group class */
Edit_Group::Edit_Group(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
#ifdef _DEBUG
	std::cout << "= Setting up Edit_Group GUI" << std::endl;
#endif // _DEBUG
	ui = new Ui::Edit_GroupClass();
	ui->setupUi(this);

	add_computer_dialog = new Add_Computer_Dialog();
	add_group_dialog = new Add_Group_Dialog();

	/* Giving members that need to be given values by the calling class
	 * initial null pointer values. In case the class gets deleted before
	 * the initialization by the calling class is complete */
	group_names = 0;
	computer_names = 0;

	node_menu = new QMenu(ui->main_tree_widget);
	ui->main_tree_widget->setContextMenuPolicy(Qt::ActionsContextMenu);

	/* Creating context menu actions */
	add_computer = new QAction(tr("Add Computer To Group"), node_menu);
	remove_computer = new QAction(tr("Remove Computer From"), node_menu);
	add_group = new QAction(tr("Add Group"), node_menu);
	remove_group = new QAction(tr("Remove Group"), node_menu);

	/* Adding actions to context_menu */
	ui->main_tree_widget->addAction(add_computer);
	ui->main_tree_widget->addAction(remove_computer);
	ui->main_tree_widget->addAction(add_group);
	ui->main_tree_widget->addAction(remove_group);

	connect(add_computer, SIGNAL(triggered()),
		this, SLOT(add_computer_slot()));
	connect(remove_computer, SIGNAL(triggered()),
		this, SLOT(remove_computer_slot()));
	connect(add_group, SIGNAL(triggered()),
		this, SLOT(add_group_slot()));
	connect(remove_group, SIGNAL(triggered()),
		this, SLOT(remove_group_slot()));
}

Edit_Group::~Edit_Group()
{
	delete ui;
	delete group_names;
	delete computer_names;
	delete node_menu;
	delete add_computer;
	delete remove_computer;
	delete add_group;
	delete remove_group;
	delete add_computer_dialog;
	delete add_group_dialog;
}

/** Function to populate the global group_names variable with a pointer to
 ** the calling class' group_names variable */
void Edit_Group::set_group_names( QList<QString>& p_group_names)
{
	group_names = &p_group_names;
}

/** Function to populate the global computer_names variable with a pointer
 ** to the calling class' computer_names variable */
void Edit_Group::set_computer_names( QList<QString>& p_computer_names)
{
	computer_names = &p_computer_names;
}

/** Slot to start the add_computer GUI and show the GUI. Check to see
 ** if the selected item is a group node. If it is not, simply returns
 ** without doing anything */
void Edit_Group::add_computer_slot()
{
#ifdef _DEBUG
	std::cout << "Tree widget has '" 
		<< ui->main_tree_widget->selectedItems().count() 
		<< "' items selected" << std::endl;
#endif // _DEBUG
	if (!ui->main_tree_widget->selectedItems().count()  == 0)
	{
#ifdef _DEBUG
		std::cout << "= add_computer_slot()" << std::endl;
#endif // _DEBUG
		if (ui->main_tree_widget->selectedItems().at(0)->childCount() == 0)
		{/* A group is selected */
#ifdef _DEBUG
			std::cout << " - Group node selected" << std::endl;
#endif // _DEBUG
		add_computer_dialog->show();
		add_computer_dialog->set_computer_names(*computer_names);
		}
		else
		{
#ifdef _DEBUG
			std::cout << " - Computer node selected" << std::endl;
#endif // _DEBUG
			return;
		}
	}
	else
	{
		//TODO
		//ADD MESSAGEBOX HERE
		return;
	}
}

void Edit_Group::remove_computer_slot()
{

}

void Edit_Group::add_group_slot()
{
	add_group_dialog->show();
	add_group_dialog->set_group_names(*group_names);
}

void Edit_Group::remove_group_slot()
{

}
