#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class finddialog;
}
QT_END_NAMESPACE

class finddialog : public QDialog
{
    Q_OBJECT

public:
    explicit finddialog(QWidget *parent = nullptr);
    ~finddialog();
    QString title() const;
private:
    Ui::finddialog *ui;
};

#endif // FINDDIALOG_H
