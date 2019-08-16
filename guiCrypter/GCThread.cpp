#include "GCThread.h"
#include "libCrypter_global.h"
//-----------------------------------------------------------------------------
GCThread::GCThread(QObject *parent)
    : QThread(parent),
    ErrorString("No error."),
    LastResult(true),
    CurrentMode(CryptMode::CM_Unknown)
{
    connect(this, &GCThread::finished, this, &GCThread::ResultSignal);
}
//-----------------------------------------------------------------------------
GCThread::~GCThread()
{

}
//-----------------------------------------------------------------------------
void GCThread::Crypt(const QString &path_source, const QString &path_output, const QString &message)
{
    CurrentMode = CryptMode::CM_Crypt;
    PathSource = path_source;
    PathOutput = path_output;
    Message = message;
    start();
}
//-----------------------------------------------------------------------------
void GCThread::Decrypt(const QString &path_source)
{
    CurrentMode = CryptMode::CM_Decrypt;
    PathSource = path_source;
    start();
}
//-----------------------------------------------------------------------------
void GCThread::run()
{
    if (CurrentMode == CryptMode::CM_Crypt)
    {
        LastResult = CryptMessage(PathSource.toStdString().c_str(), PathOutput.toStdString().c_str(), Message.toStdString().c_str()) ? true : false;
        if (!LastResult)
        {
            ErrorString = GetError();
        }
    }
    else if (CurrentMode == CryptMode::CM_Decrypt)
    {
        const char *Result = DecryptMessage(PathSource.toStdString().c_str());
        LastResult = Result ? true : false;
        if (LastResult)
        {
            Message = Result;
        }
        else
        {
            ErrorString = GetError();
        }
    }
}
//-----------------------------------------------------------------------------
void GCThread::ResultSignal()
{
    switch (CurrentMode)
    {
    case CryptMode::CM_Crypt:
        emit Crypted(LastResult, ErrorString, PathOutput);
        break;

    case CryptMode::CM_Decrypt:
        emit Decrypted(LastResult, ErrorString, Message);
        break;

    default:
        break;
    }
}
//-----------------------------------------------------------------------------
