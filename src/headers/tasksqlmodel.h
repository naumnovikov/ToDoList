#ifndef TASKSQLMODEL_H
#define TASKSQLMODEL_H

#include <QSqlTableModel>
#include <QSqlDatabase>

class TaskSqlModel : public QSqlTableModel
{
public:
    TaskSqlModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase()); //создаем объект таблицы

    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const override; //const т.к. не меняем сам класс

    bool setData(const QModelIndex &idx, const QVariant &value, int role = Qt::EditRole) override; //сохранение изменений

    Qt::ItemFlags flags(const QModelIndex &index) const override; //базово выдает флаги для каждой ячейки, но кастомизирована, чтобы не рисовать у чек бокса 1 или 0
};

#endif // TASKSQLMODEL_H
