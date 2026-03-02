#ifndef ADDTASKDIALOG_H
#define ADDTASKDIALOG_H

#include <QDialog>
#include <QDateTime>

QT_BEGIN_NAMESPACE //связываю диаологовое окно с графическим интерфейсом
namespace Ui {
class AddTaskDialog;
}
QT_END_NAMESPACE

class AddTaskDialog : public QDialog
{
    Q_OBJECT //чтобы работали сигналы и слоты
public:
    explicit AddTaskDialog(QWidget *parent = nullptr);
    ~AddTaskDialog();
    QString title() const;  //эти функции читают заголовок и дату с временем и возвращает их
                            //ничего больше не делают в диалоговом окне, поэтому const
    QDateTime dateTime() const;
private:
    Ui::AddTaskDialog *ui; //храним графический интерфейс
};

#endif // ADDTASKDIALOG_H
