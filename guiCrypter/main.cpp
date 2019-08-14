#include "StdAfx.h"
#include "GCMainWindow.h"
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication Application(argc, argv);
    Application.setFont(QFont("Lato", 9, QFont::Normal));
    GCMainWindow mainWindow;
    mainWindow.show();
    return Application.exec();
}
//-----------------------------------------------------------------------------
