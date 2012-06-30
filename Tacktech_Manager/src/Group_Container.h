#ifndef GROUP_CONTAINER_H
#define GROUP_CONTAINER_H
#include <QMultiMap>
#include <QString>
#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG
class Group_Container
{
public:
	Group_Container();
	~Group_Container();
	bool add_group_name(QString);
	bool add_computer_name(QString ,QString);
	QMultiMap<QString, QString> get_groups_and_computers();
#ifdef _DEBUG
	void print_contents();
#endif // _DEBUG
private:
	QMultiMap<QString, QString> groups_and_computers;
};
#endif //GROUP_CONTAINER_H
