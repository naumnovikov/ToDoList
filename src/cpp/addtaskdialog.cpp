#include "addtaskdialog.h"
#include "ui_addtaskdialog.h"


AddTaskDialog::AddTaskDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AddTaskDialog) //1
{
    ui->setupUi(this);

    QDateTime now{QDateTime::currentDateTime()};

    ui->dateTimeEdit->setDateTime(now);
    ui->dateTimeEdit->setCalendarPopup(true);
    ui->dateTimeEdit->setMinimumDateTime(now);
}

AddTaskDialog::~AddTaskDialog()
{
    delete ui;
}


QString AddTaskDialog::title() const
{
    return ui->titleEdit->text();
}

QDateTime AddTaskDialog::dateTime() const
{
    return ui->dateTimeEdit->dateTime();
}
