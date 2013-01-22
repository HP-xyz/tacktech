#include "Edit_Times.h"


Edit_Times::Edit_Times(QWidget *parent, Qt::WFlags flags):QWidget(parent, flags)
{
	ui.setupUi(this);
}


Edit_Times::~Edit_Times(void)
{
}

void Edit_Times::ok_clicked()
{
	emit times_changed(ui.start_time->time(), ui.end_time->time());
	this->close();
}

void Edit_Times::cancel_clicked()
{
	this->close();
}
