#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DatabaseManager::DatabaseManager()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE"); //подключение драйвера sqlite
    m_db.setDatabaseName("todo.sqlite");
}

QSqlDatabase DatabaseManager::database() const
{
    return m_db;
}

bool DatabaseManager::initDatabase()
{
    if (!m_db.open()) {
        qWarning() << "Ошибка: не удалось открыть БД.\n";
        return false;
    }

    QSqlQuery query(m_db); //создание запроса к m_db
    //R - чтобы не нужно было экранировать
    bool success{query.exec(R"(
        CREATE TABLE IF NOT EXISTS tasks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            due_datetime TEXT,
            status INTEGER NOT NULL DEFAULT 0
        );
    )")};

    if (!success) {
        qWarning() << "Ошибка: не удалось создать таблицу:" << query.lastError().text();
    }

    return success;
}
