#include "GCWidgetCrypt.h"
//-----------------------------------------------------------------------------
GCWidgetCrypt::GCWidgetCrypt(QWidget *parent) : GCWidgetBase(parent)
{
    QHBoxLayout *Layout = new QHBoxLayout();
    setLayout(Layout);

    ImageLeft = new GCPanelImage(true, this);
    connect(ImageLeft, &GCPanelImage::ImageChaned, this, &GCWidgetCrypt::ButtonEnabled);
    Layout->addWidget(ImageLeft);

    QVBoxLayout *LayoutTextEdit = new QVBoxLayout();
    Layout->addLayout(LayoutTextEdit);

    LayoutTextEdit->addStretch();

    TextEdit = new QTextEdit(this);
    TextEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    connect(TextEdit, &QTextEdit::textChanged, this, &GCWidgetCrypt::ButtonEnabled);
    LayoutTextEdit->addWidget(TextEdit, 0, Qt::AlignHCenter);

    PushButton = new QPushButton(QIcon(":Resources/Arrow.Right.png"), "Crypt", this);
    PushButton->setEnabled(false);
    PushButton->setSizePolicy(QSizePolicy::Maximum, PushButton->sizePolicy().verticalPolicy());
    connect(PushButton, &QPushButton::clicked, this, &GCWidgetCrypt::ButtonClicked);
    LayoutTextEdit->addWidget(PushButton, 0, Qt::AlignHCenter);

    LayoutTextEdit->addStretch();

    ImageRight = new GCPanelImage(false, this);
    ImageLeft->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    Layout->addWidget(ImageRight);

    Thread = new GCThread(this);
    connect(Thread, &GCThread::started, this, &GCWidgetCrypt::ThreadStarted);
    connect(Thread, &GCThread::finished, this, &GCWidgetCrypt::ThreadFinished);
    connect(Thread, &GCThread::Crypted, this, &GCWidgetCrypt::Crypted);
}
//-----------------------------------------------------------------------------
GCWidgetCrypt::~GCWidgetCrypt()
{

}
//-----------------------------------------------------------------------------
void GCWidgetCrypt::ButtonEnabled()
{
    PushButton->setEnabled(!TextEdit->toPlainText().isEmpty() && !ImageLeft->GetIsNull() ? true : false);
}
//-----------------------------------------------------------------------------
void GCWidgetCrypt::ButtonClicked()
{
    if (!Thread->isRunning())
    {
        Thread->Crypt(ImageLeft->GetFilePath(), QApplication::applicationDirPath() + "/" + QUuid::createUuid().toString() + ".png", TextEdit->toPlainText());
    }
}
//-----------------------------------------------------------------------------
void GCWidgetCrypt::ThreadStarted()
{
    PushButton->setText("Please, wait...");
    PushButton->setEnabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);
}
//-----------------------------------------------------------------------------
void GCWidgetCrypt::ThreadFinished()
{
    PushButton->setText("Crypt");
    PushButton->setEnabled(true);
    QApplication::restoreOverrideCursor();
}
//-----------------------------------------------------------------------------
void GCWidgetCrypt::Crypted(bool Result, const QString &ErrorString, const QString &PathOutput)
{
    Result ? ImageRight->LoadPixmap(PathOutput) : QMessageBox::critical(this, "Critical", ErrorString, QMessageBox::Ok);
}
//-----------------------------------------------------------------------------
