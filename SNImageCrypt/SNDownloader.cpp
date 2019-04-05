#include "SNDownloader.h"
#include "SNUrl.h"
#include "SNFileInfo.h"
#include "Prefs.h"
#include <iostream>
#include <string.h>
#include "netdb.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fstream>
#include <unistd.h>
//-----------------------------------------------------------------------------
SNDownloader::SNDownloader(const std::string &url) :
    ErrorString("No error."),
    Url(url),
    Vector(new std::vector<char>())
{

}
//-----------------------------------------------------------------------------
SNDownloader::~SNDownloader()
{
    delete Vector;
}
//-----------------------------------------------------------------------------
std::string SNDownloader::GetErrorString() const
{
    return ErrorString;
}
//-----------------------------------------------------------------------------
std::string SNDownloader::GetFilePath() const
{
    return FilePath;
}
//-----------------------------------------------------------------------------
bool SNDownloader::Download()
{
    SNUrl URL(Url);
    if (!URL.GetIsValid())
    {
        ErrorString = "Url invalid";
        return false;
    }

    char CurrentDir[FILENAME_MAX];
    getcwd(CurrentDir, FILENAME_MAX);
    //FilePath = std::string(CurrentDir) + "/" + SNFileInfo(URL.GetPath()).FileName();

    int Socket = socket(AF_INET , SOCK_STREAM , 0); //Создание сокета
    if (Socket == -1)
    {
        ErrorString = "Could not create socket.";
        return false;
    }

    struct sockaddr_in Server;
    Server.sin_addr.s_addr = inet_addr(URL.GetHostAddress().c_str());
    Server.sin_family = AF_INET;
    Server.sin_port = htons(80);

    if (connect(Socket, (struct sockaddr*)&Server, sizeof(Server)) < 0) //Если подключение не было установлено
    {
        ErrorString = "Connect error.";
        return false;
    }

    std::string Message =
            "GET " + URL.GetPath() + " HTTP/1.1\r\n"
            "Host: " + URL.GetHostName() + "\r\n\r\n"
            " Connection: keep-alive\r\n\r\n"
            " Keep-Alive: 300\r\n";

    if (send(Socket, Message.c_str(), Message.length(), 0) == -1) //Если запрос не был отправлен
    {
        ErrorString = "Send failed.";
        return false;
    }

    remove(FilePath.c_str());
    std::ofstream File(FilePath, std::ios::binary);
    if (File == nullptr)
    {
        ErrorString = "File could not opened.";
        return false;
    }

    char Buffer[SocketBufferSize]; //Буфер с данными пакета
    long Length = 0; //Длинна пакета

    while ((Length = recv(Socket, Buffer, SocketBufferSize, 0)) > 0) //Получение данных с сокета
    {
        for (int i = 0; i < SocketBufferSize; i++) //Заполнение вектора очередной порцией данных
        {
            Vector->push_back(Buffer[i]);
        }
    }

    close(Socket);

    size_t DataPosition = 0;
    for (size_t Iterator = 0; Iterator < Vector->size(); Iterator++) //Обход вектора с данными
    {
        if (Iterator == Vector->size() - 3)
        {
            break;
        }

        char CharP = Vector->at(Iterator + 1);
        char CharN = Vector->at(Iterator + 2);
        char CharG = Vector->at(Iterator + 3);
        if (CharP == 80 && CharN == 78 && CharG == 71) //Если очередные три байта равны сигнатуре PNG - запоминаем позицию окончания заголовка и выходим
        {
            DataPosition = Iterator;
            break;
        }
    }

    if (!DataPosition)
    {
        ErrorString = "File is not a PNG format.";
        return false;
    }

    size_t Iterator = 0;
    while (true) //Удаление заголовка из вектора
    {
        Vector->erase(Vector->begin());
        Iterator++;
        if (Iterator == DataPosition)
        {
            break;
        }
    }

    File.write(Vector->data(), Vector->size());
    File.close();
    return true;
}
//-----------------------------------------------------------------------------
