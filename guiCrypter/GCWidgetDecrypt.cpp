#include "GCWidgetDecrypt.h"
//-----------------------------------------------------------------------------
GCWidgetDecrypt::GCWidgetDecrypt(QWidget *parent) : GCWidgetBase(parent)
{
    QVBoxLayout *Layout = new QVBoxLayout();
    setLayout(Layout);

    PanelImage = new GCPanelImage(true, this);
    connect(PanelImage, &GCPanelImage::ImageChaned, this, &GCWidgetDecrypt::ButtonEnabled);
    Layout->addWidget(PanelImage);

    PushButton = new QPushButton(QIcon(":Resources/Arrow.Down.png"), "Decrypt", this);
    PushButton->setSizePolicy(QSizePolicy::Maximum, PushButton->sizePolicy().verticalPolicy());
    connect(PushButton, &QPushButton::clicked, this, &GCWidgetDecrypt::ButtonClicked);
    Layout->addWidget(PushButton,0, Qt::AlignHCenter);

    QGroupBox *GroupBox = new QGroupBox("Result", this);
    GroupBox->setLayout(new QVBoxLayout());
    Layout->addWidget(GroupBox);

    TextEdit = new QTextEdit(this);
    TextEdit->setReadOnly(true);
    GroupBox->layout()->addWidget(TextEdit);

    Thread = new GCThread(this);
    connect(Thread, &GCThread::started, this, &GCWidgetDecrypt::ThreadStarted);
    connect(Thread, &GCThread::finished, this, &GCWidgetDecrypt::ThreadFinished);
    connect(Thread, &GCThread::Decrypted, this, &GCWidgetDecrypt::Decrypted);
}
//-----------------------------------------------------------------------------
GCWidgetDecrypt::~GCWidgetDecrypt()
{

}
//-----------------------------------------------------------------------------
void GCWidgetDecrypt::ButtonEnabled()
{
    PushButton->setEnabled(!PanelImage->GetIsNull() ? true : false);
}
//-----------------------------------------------------------------------------
void GCWidgetDecrypt::ButtonClicked()
{
    if (!Thread->isRunning())
    {
        Thread->Decrypt(PanelImage->GetFilePath());
    }
}
//-----------------------------------------------------------------------------
void GCWidgetDecrypt::ThreadStarted()
{
    PushButton->setText("Please, wait...");
    PushButton->setEnabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);
}
//-----------------------------------------------------------------------------
void GCWidgetDecrypt::ThreadFinished()
{
    PushButton->setText("Decrypt");
    PushButton->setEnabled(true);
    QApplication::restoreOverrideCursor();
}
//-----------------------------------------------------------------------------
void GCWidgetDecrypt::Decrypted(bool Result, const QString &ErrorString, const QString &Message)
{
    Result ? TextEdit->setText(Message) : QMessageBox::critical(this, "Critical", ErrorString, QMessageBox::Ok);
}
//-----------------------------------------------------------------------------
