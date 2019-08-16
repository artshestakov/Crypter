#include "GCMainWindow.h"
//-----------------------------------------------------------------------------
GCMainWindow::GCMainWindow(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *MainLayout = new QVBoxLayout();
    MainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(MainLayout);

    QHBoxLayout *LayoutButtons = new QHBoxLayout();
    LayoutButtons->setContentsMargins(5, 5, 5, 5);
    MainLayout->addLayout(LayoutButtons);

    LayoutButtons->addWidget(new QLabel("Mode:", this));

    ButtonCrypt = new QRadioButton(this);
    ButtonCrypt->setText("Crypt");
    ButtonCrypt->setChecked(true);
    connect(ButtonCrypt, &QRadioButton::clicked, this, &GCMainWindow::ModeChanged);
    LayoutButtons->addWidget(ButtonCrypt);

    ButtonDecrypt = new QRadioButton(this);
    ButtonDecrypt->setText("Decrypt");
    connect(ButtonDecrypt, &QRadioButton::clicked, this, &GCMainWindow::ModeChanged);
    LayoutButtons->addWidget(ButtonDecrypt);

    LayoutButtons->addStretch();

    QFrame *Frame = new QFrame(this);
    Frame->setFrameShadow(QFrame::Sunken);
    Frame->setFrameShape(QFrame::HLine);
    MainLayout->addWidget(Frame);

    StackWidget = new QStackedWidget(this);
    MainLayout->addWidget(StackWidget);

    WidgetCrypt = new GCWidgetCrypt(this);
    StackWidget->addWidget(WidgetCrypt);
    
    WidgetDecrypt = new GCWidgetDecrypt(this);
    StackWidget->addWidget(WidgetDecrypt);

    QStatusBar *StatusBar = new QStatusBar(this);
    MainLayout->addWidget(StatusBar);

    QAction *ActionEscape = new QAction(this);
    ActionEscape->setShortcut(QKeySequence(Qt::Key_Escape));
    connect(ActionEscape, &QAction::triggered, this, &GCMainWindow::close);
    addAction(ActionEscape);
}
//-----------------------------------------------------------------------------
GCMainWindow::~GCMainWindow()
{

}
//-----------------------------------------------------------------------------
void GCMainWindow::closeEvent(QCloseEvent *e)
{
    QMessageBox::question(this, "Question", "Exit?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes ? QWidget::closeEvent(e) : e->ignore();
}
//-----------------------------------------------------------------------------
void GCMainWindow::ModeChanged()
{
    StackWidget->setCurrentWidget(StackWidget->currentWidget() == WidgetCrypt ? dynamic_cast<QWidget*>(WidgetDecrypt) : dynamic_cast<QWidget*>(WidgetCrypt));
}
//-----------------------------------------------------------------------------
