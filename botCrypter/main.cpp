#include "StdAfx.h"
#include "BCBot.h"
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication Application(argc, argv);
    qDebug() << "Bot is starting...";
    BCBot bot("919167651:AAFWwPj2gRIyQ4xs2p7zWSA6noIzHxbaju8");
    bot.Start();
    qDebug() << "Bot is started";
    return Application.exec();
}
//-----------------------------------------------------------------------------
