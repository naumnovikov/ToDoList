#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStyledItemDelegate>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include "databasemanager.h"
#include "tasksqlmodel.h"
#include <QTimeEdit>



class NoEditorDelegate : public QStyledItemDelegate  //чтобы убрать мусор с ячейки с галочкой
{
public:
    NoEditorDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    QWidget* createEditor(QWidget*, const QStyleOptionViewItem&, const QModelIndex&) const override
    {
        return nullptr;
    }
};

class TimeEditDelegate : public QStyledItemDelegate
{
public:
    TimeEditDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex&) const override
    { //возвращаем часы вместо обычного текстового поля
        QTimeEdit *editor{new QTimeEdit(parent)};
        editor->setDisplayFormat("HH:mm");
        return editor;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const override
    {//когда пользователь хочет изменить время, показываем ему старое время
        QString dateTimeString{index.model()->data(index, Qt::EditRole).toString()}; //берем дату из таблицы в строке
        QDateTime dateTime{QDateTime::fromString(dateTimeString, Qt::ISODate)};      //и переводим в объект даты

        QTimeEdit *timeEditor{static_cast<QTimeEdit*>(editor)}; //static_cast т.к. при множественном наследовании
        timeEditor->setTime(dateTime.time());                   //reinterpret забивает на сдвиги
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override
    {//когда пользователь выбрал дату, сохраняем изменения в БД
        QTimeEdit *timeEditor{static_cast<QTimeEdit*>(editor)};
        QTime newTime{timeEditor->time()};  //выставляем заданное пользователем время

        QString originalString{model->data(index, Qt::EditRole).toString()};
        QDateTime dateTime{QDateTime::fromString(originalString, Qt::ISODate)};//достаем всю дату

        dateTime.setTime(newTime);                   //и ставим ей новое выставленное время (число месяц не меняем)

        model->setData(index, dateTime.toString(Qt::ISODate), Qt::EditRole);
    }
};



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addButton_clicked();
    void on_deleteButton_clicked();

    void on_searchButton_clicked();

    void onModelDataChanged();

    void on_overdueButton_clicked();

    void on_findButton_clicked();
private:
    Ui::MainWindow *ui;
    DatabaseManager m_dbManager;
    TaskSqlModel *m_model;

    void setupModel(); //настройка db
};
#endif // MAINWINDOW_H
