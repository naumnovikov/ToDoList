#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>

class DatabaseManager
{
public:
    DatabaseManager();
    bool initDatabase();
    QSqlDatabase database() const;
private:
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
