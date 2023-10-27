#ifndef TASK_H
#define TASK_H

#include <string>
#include <chrono>
#include <QString>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QSharedPointer>


class Task
{
public:
    Task(QSharedPointer<QSqlDatabase> &database);
    double totalTime() noexcept;
    double totalPrice() noexcept;
    double priceUntilNow() noexcept;
    void stopTask() noexcept;
    void startTask() noexcept;
    void setClientName(const std::string &clientName) noexcept;
    void setTaskName(const std::string &taskName) noexcept;
    void setPricePerHour(const float &price = 0.00) noexcept;
    QString getClientName() noexcept;
    QString getTaskName() noexcept;
    QString getCreatedAt() noexcept;
    QList<QString> getUsedTaskNames() noexcept;
    QList<QString> getUsedClientNames() noexcept;

    static bool getLastRecord(QString &client, QString &task, QString &price, QSharedPointer<QSqlDatabase> database);

private:
    std::string clientName;
    std::string taskName;
    bool probono = true;
    QDateTime createdAt;
    QDateTime closedAt;
    float pricePerHour;
    bool isRunning = false;
    bool writeTaskToDb();
    QSharedPointer<QSqlDatabase> database;
};

#endif // TASK_H
