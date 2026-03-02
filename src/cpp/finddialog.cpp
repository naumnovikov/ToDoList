#include "finddialog.h"
#include "ui_finddialog.h"

finddialog::finddialog(QWidget *parent): QDialog(parent), ui(new Ui::finddialog)
{
    ui->setupUi(this);
}

finddialog::~finddialog()
{
    delete ui;
}

QString finddialog::title() const
{
    return ui->findText->text();
}
