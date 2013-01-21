#include "Add_Pause_Dialog.h"

Add_Pause_Dialog::Add_Pause_Dialog(QWidget *parent, Qt::WFlags flags) :
		QDialog(parent, flags)
{
	ui.setupUi(this);
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(ok_clicked()));
	connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(cancel_clicked()));
}

Add_Pause_Dialog::~Add_Pause_Dialog()
{

}

void Add_Pause_Dialog::ok_clicked()
{
	emit pause_changed(ui.spinBox->value());
	this->close();
}

void Add_Pause_Dialog::cancel_clicked()
{
	emit pause_unchanged();
	this->close();
}

void Add_Pause_Dialog::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Enter:
	{
		ok_clicked();
		break;
	}
	case Qt::Key_Return:
	{
		ok_clicked();
		break;
	}
	default:
		QWidget::keyPressEvent(event);
	}
}
