#ifndef EDIT_TIMES_H
#define EDIT_TIMES_H
#include <QWidget>
#include <QTime>
#include "ui_Edit_Times.h"
class Edit_Times: public QWidget
{
	Q_OBJECT
public:
	Edit_Times(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Edit_Times(void);
private:
	Ui::Edit_Times_Dialog ui;
private slots:
	void ok_clicked();
	void cancel_clicked();
signals:
	void times_changed(QTime, QTime);
};
#endif //EDIT_TIMES_H
