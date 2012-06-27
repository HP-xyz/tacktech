#include "Edit_Group.h"

Edit_Group::Edit_Group(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
#ifdef _DEBUG
	std::cout << "= Setting up Edit_Group GUI" << std::endl;
#endif // _DEBUG
	ui = new Ui::Edit_GroupClass();
#ifdef _DEBUG
	std::cout << " - Giving group_names null pointer" << std::endl;
#endif // _DEBUG
	group_names = 0;

#ifdef _DEBUG
	std::cout << " - Creating node menu" << std::endl;
#endif // _DEBUG
	node_menu = new QMenu(ui->main_tree_widget);
#ifdef _DEBUG
	std::cout << " - Setting ContextMenuPolicy" << std::endl;
#endif // _DEBUG
	ui->main_tree_widget->setContextMenuPolicy(Qt::ActionsContextMenu);
#ifdef _DEBUG
	std::cout << " - Creating insert_child QAction" << std::endl;
#endif // _DEBUG
	insert_child = new QAction(tr("Add Computer"), node_menu);
#ifdef _DEBUG
	std::cout << " - Adding action to main_tree_widget" << std::endl;
#endif // _DEBUG
	ui->main_tree_widget->addAction(insert_child);

#ifdef _DEBUG
	std::cout << " - Setting up UI" << std::endl;
#endif // _DEBUG
	ui->setupUi(this);
}

Edit_Group::~Edit_Group()
{
	delete ui;
	delete group_names;
	delete node_menu;
	delete insert_child;
}

void Edit_Group::set_group_names( QList<QString>& p_group_names)
{
	group_names = &p_group_names;
}
