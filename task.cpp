#include "task.hpp"


namespace Time {
    int
    hours(const double &time)
    {
        return static_cast<int>(time / 3600);
    }
}


Task::Task(QSharedPointer<QSqlDatabase> &database)
{
    this->database = database;
}

void
Task::startTask() noexcept
{
    createdAt = QDateTime::currentDateTime();
    isRunning = true;
}

void
Task::stopTask() noexcept
{
    closedAt = QDateTime::currentDateTime();
    isRunning = false;
    if (!writeTaskToDb())
        qWarning("Couldn't save record");
}

void
Task::setClientName(const std::string &clientName) noexcept
{
    this->clientName = clientName;
}

void
Task::setTaskName(const std::string &taskName) noexcept
{
    this->taskName = taskName;
}

void
Task::setPricePerHour(const float &price) noexcept
{
    if (price > 0.00)
        probono = false;
    else
        probono = true;

    pricePerHour = price;
}

double
Task::totalTime() noexcept
{
    return createdAt.secsTo(closedAt);
}

double
Task::totalPrice() noexcept
{
    auto time = totalTime();
    int hours = Time::hours(time);

    return hours * pricePerHour;
}

double
Task::priceUntilNow() noexcept
{
    if (!isRunning)
        return totalPrice();

    auto time = createdAt.secsTo(QDateTime::currentDateTime());
    int hours = Time::hours(time);

    return hours * pricePerHour;
}

QString
Task::getClientName() noexcept
{
    return QString::fromStdString(clientName);
}

QString
Task::getTaskName() noexcept
{
    return QString::fromStdString(taskName);
}

QString
Task::getCreatedAt() noexcept
{
    return createdAt.toString(Qt::DateFormat::ISODate);
}

bool Task::writeTaskToDb()
{
    database->open();

    if (database->isOpen())
    {
        QSqlQuery query;
        query.prepare("INSERT INTO tasks (client, task, price_per_hour, probono, created_at, closed_at) "
                      "VALUES(:client, :task, :pph, :probono, :created, :closed)");
        query.bindValue(":client", QString::fromStdString(clientName));
        query.bindValue(":task", QString::fromStdString(taskName));
        query.bindValue(":pph", pricePerHour);
        query.bindValue(":probono", probono);
        query.bindValue(":created", createdAt);
        query.bindValue(":closed", closedAt);

        auto result = query.exec();

        database->close();

        return result;
    }
    return false;
}

bool
Task::getLastRecord(QString &client, QString &task, QString &price, QSharedPointer<QSqlDatabase> database)
{
    database->open();

    if (database->isOpen())
    {

        QSqlQuery query;
        auto result = query.exec("SELECT * FROM tasks ORDER BY closed_at DESC LIMIT 1;");
        query.next();
        client = query.value(1).toString();
        task = query.value(2).toString();
        price = QString::number(query.value(3).toFloat());

        database->close();

        return result;
    }
    return false;
}

QList<QString>
Task::getUsedTaskNames() noexcept
{
    QList<QString> results = {};
    database->open();

    if (database->isOpen())
    {
        QSqlQuery query;
        query.exec("SELECT DISTINCT(task) from tasks ORDER BY task;");

        while (query.next())
            results.append(query.value(0).toString());

        database->close();
    }
    return results;
}

QList<QString>
Task::getUsedClientNames() noexcept
{
    QList<QString> results = {};
    database->open();

    if (database->isOpen())
    {
        QSqlQuery query;
        query.exec("SELECT DISTINCT(client) from tasks ORDER BY client;");

        while (query.next())
            results.append(query.value(0).toString());

        database->close();
    }
    return results;
}

bool Task::is_running() noexcept
{
    return isRunning;
}
