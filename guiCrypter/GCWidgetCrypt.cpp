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

    Button = new QPushButton(QIcon(":Resources/Arrow.Right.png"), "Crypt", this);
    Button->setEnabled(false);
    Button->setSizePolicy(QSizePolicy::Maximum, Button->sizePolicy().verticalPolicy());
    connect(Button, &QPushButton::clicked, this, &GCWidgetCrypt::ButtonClicked);
    LayoutTextEdit->addWidget(Button, 0, Qt::AlignHCenter);

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
    Button->setEnabled(!TextEdit->toPlainText().isEmpty() && !ImageLeft->GetIsNull() ? true : false);
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
    Button->setText("Please, wait...");
    Button->setEnabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);
}
//-----------------------------------------------------------------------------
void GCWidgetCrypt::ThreadFinished()
{
    Button->setText("Crypt");
    Button->setEnabled(true);
    QApplication::restoreOverrideCursor();
}
//-----------------------------------------------------------------------------
void GCWidgetCrypt::Crypted(bool Result, const QString &ErrorString, const QString &PathOutput)
{
    Result ? ImageRight->LoadPixmap(PathOutput) : QMessageBox::critical(this, "Critical", ErrorString, QMessageBox::Ok);
}
//-----------------------------------------------------------------------------
