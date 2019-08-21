#include "StdAfx.h"
#include "BCBot.h"
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication Application(argc, argv);

    QSqlDatabase DB = QSqlDatabase::addDatabase("QPSQL");
    DB.setHostName("127.0.0.1");
    DB.setPort(5432);
    DB.setDatabaseName("one_channel");
    DB.setUserName("postgres");
    DB.setPassword("adm777");
    if (DB.open())
    {
        qDebug() << "Bot is starting...";
        BCBot bot("919167651:AAFWwPj2gRIyQ4xs2p7zWSA6noIzHxbaju8");
        bot.Start();
        qDebug() << "Bot is started";
    }
    else
    {
        qDebug() << DB.lastError().text();
    }
    
    return Application.exec();
}
//-----------------------------------------------------------------------------
