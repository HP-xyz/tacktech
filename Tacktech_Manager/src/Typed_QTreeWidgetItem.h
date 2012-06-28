#ifndef TYPED_QTREEWIDGETITEM_H
#define TYPED_QTREEWIDGETITEM_H

#include <QTreeWidgetItem>
#include <QString>
class Typed_QTreeWidgetItem : public QTreeWidgetItem
{
public:
	Typed_QTreeWidgetItem(QTreeWidgetItem *parent = 0);
	~Typed_QTreeWidgetItem();
	void set_type(QString);
	QString get_type();
private:
	QString widget_type;
};

#endif // TYPED_QTREEWIDGETITEM_H