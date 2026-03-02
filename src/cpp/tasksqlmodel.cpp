#include "tasksqlmodel.h"
#include <QDateTime>

TaskSqlModel::TaskSqlModel(QObject *parent, QSqlDatabase db): QSqlTableModel(parent, db) {}



Qt::ItemFlags TaskSqlModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlTableModel::flags(index);

    if (index.column() == 3) { //column возвращает порядковый номер столбца (0, 1, 2, 3)
        flags |= Qt::ItemIsUserCheckable; //добавляем побитово флаг-расширение чекбокса ко другим флагам
    }

    return flags;
}


QVariant TaskSqlModel::data(const QModelIndex &idx, int role) const  //QVariant чтобы можно было возвращать разные виджеты
{ //QModelIndex - адрес ячейки(column, row, model)
    if (idx.column() == 2 && role == Qt::DisplayRole) {
        QString rawDateTime{QSqlTableModel::data(idx, role).toString()}; //оригинальный data() возвращает QVariant дату
        QDateTime dt{QDateTime::fromString(rawDateTime, Qt::ISODate)}; //ISODate = YYYY-MM-DDTHH:mm:ss

        if (dt.isValid()) {
            if (dt.date() == QDate::currentDate()) {
                return "Сегодня, " + dt.time().toString("HH:mm");
            }
            return dt.toString("dd.MM.yyyy HH:mm");
        }
    }

    if (idx.column() == 3) {
        if (role == Qt::CheckStateRole) { //если нужно рисовать чек бокс
            int status{QSqlTableModel::data(idx, Qt::DisplayRole).toInt()};
            return (status == 1) ? Qt::Checked : Qt::Unchecked;
        }
        if (role == Qt::DisplayRole) {
            return "";
        }
    }

    return QSqlTableModel::data(idx, role);
}


bool TaskSqlModel::setData(const QModelIndex &idx, const QVariant &value, int role)
{
    if (idx.column() == 3 && role == Qt::CheckStateRole) {
        int status{(value.toInt() == Qt::Checked) ? 1 : 0};

        return QSqlTableModel::setData(idx, status, Qt::EditRole);
    }

    return QSqlTableModel::setData(idx, value, role);
}
