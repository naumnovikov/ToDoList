#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addtaskdialog.h"
#include <QSqlRecord>
#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>
#include "finddialog.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this); //рисуем интерфейс

    ui->filterDateEdit->setCalendarPopup(true);

    if (m_dbManager.initDatabase()) {
        setupModel();
        connect(m_model, &TaskSqlModel::dataChanged, this, &MainWindow::onModelDataChanged);
    } else {
        qWarning() << "Не удалось инициализировать БД!";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setupModel()
{
    m_model = new TaskSqlModel(this, m_dbManager.database());
    m_model->setTable("tasks");
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit); //отключаем автосохранение, саим вызываем submitAll

    m_model->setSort(2, Qt::AscendingOrder); //сортируем задачи по дате



    m_model->setHeaderData(1, Qt::Horizontal, "Задача");
    m_model->setHeaderData(2, Qt::Horizontal, "Срок выполнения");
    m_model->setHeaderData(3, Qt::Horizontal, "Готово");

    ui->tasksView->setModel(m_model);

    ui->tasksView->hideColumn(0); //прячу технический id


    ui->tasksView->setItemDelegateForColumn(3, new NoEditorDelegate(this)); //используем делегаты для чек бокса и часов
    ui->tasksView->setItemDelegateForColumn(2, new TimeEditDelegate(this));

    ui->tasksView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); //растягиваем максимальное 1 и 2 колоны
    ui->tasksView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch); //3 минимально чтобы места занимало
    ui->tasksView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);


    ui->filterDateEdit->setDate(QDate::currentDate()); //по дефолту сегодняшние задачи
    on_searchButton_clicked();
}

void MainWindow::onModelDataChanged()
{
    m_model->submitAll(); //когда ставим чек бокс вызвается сохранение
}

void MainWindow::on_addButton_clicked()
{
    AddTaskDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString title{dialog.title()};
        if (title.isEmpty()){
            QMessageBox::warning(this, "Ошибка", "Название задачи не может быть пустым!");
            return;
        }


        QDateTime dateTime{dialog.dateTime()};
        QString isoDateTime{dateTime.toString(Qt::ISODate)};


        QSqlQuery query(m_dbManager.database());

        query.prepare("INSERT INTO tasks (title, due_datetime, status) "
                      "VALUES (:title, :datetime, :status)"); //cоздаем строку с 3 полями куда ниже вставим данные


        query.bindValue(":title", title);
        query.bindValue(":datetime", isoDateTime);
        query.bindValue(":status", 0);


        if (!query.exec()) {
            return;
        }


        m_model->select(); //обновляем интерфейс

    }
}

void MainWindow::on_deleteButton_clicked()
{
    QModelIndex index{ui->tasksView->currentIndex()};
    if (!index.isValid()) return; //если нажал не на задачу, то выход

    m_model->removeRow(index.row());
    m_model->submitAll(); //сохранение
    m_model->select();
}

void MainWindow::on_searchButton_clicked() //по сути отвечает за отображение таблицы
{
    QDate date{ui->filterDateEdit->date()}; //сохраняем введенную пользователем дату

    QString prettyDate{QLocale(QLocale::Russian).toString(date, "d MMMM yyyy")};

    ui->headerLabel->setText(prettyDate);

    ui->tasksView->setItemDelegateForColumn(3, new NoEditorDelegate(this)); //использую кастомный делегат для 3 колонки

    QString dateString{date.toString(Qt::ISODate)};

    QString filter{QString("due_datetime LIKE '%1%'").arg(dateString)}; //ищем все что начинается с этой даты


    m_model->setFilter(filter); //применяем фильтр

    m_model->select();
}

void MainWindow::on_overdueButton_clicked() //просто вывожу все что меньше чем сейчас и не выполнено
{
    QString filter{"datetime(due_datetime) < datetime('now', 'localtime') AND status = 0"}; //не забываю все даты приводить к одному виду


    m_model->setFilter(filter);

    ui->headerLabel->setText("Просроченные задачи");
}
void MainWindow::on_findButton_clicked(){
    finddialog dialog(this);
    if (dialog.exec() == QDialog::Accepted){
        QString title{dialog.title()};
        if (title.isEmpty()){
            QMessageBox::warning(this, "Ошибка", "Название задачи не может быть пустым!");
            return;
        }

        QString filter{QString("title LIKE '%1%'").arg(title)};
        m_model->setFilter(filter);
        ui->headerLabel->setText("Найденнные");
    }
}
