#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include "Bot.h"
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QCoreApplication Application(argc, argv);

    qDebug() << "Bot is starting...";
    Bot bot("919167651:AAFWwPj2gRIyQ4xs2p7zWSA6noIzHxbaju8");
    bot.Start();
    qDebug() << "Bot is started";
    return Application.exec();
}
//-----------------------------------------------------------------------------
