#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(ChatConfig livecoding, ChatConfig twitch, QWidget* parent)
    : mLivecoding(livecoding),
      mTwitch(twitch),
      QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

#include <QMessageBox>

void MainWindow::addLogMessage(const QString & message)
{
    ui->textLog->append(QString("[%1] %2").arg(QTime::currentTime().toString(Qt::SystemLocaleShortDate), message));
}

void MainWindow::twitchConnected()
{
    addLogMessage("[Twitch] Connected!");
}

void MainWindow::twitchDisconnected()
{
    addLogMessage("[Twitch] Disconnected...");
}

void MainWindow::twitchMessageReceived(const QString & from, const QString & message)
{
    addLogMessage(QString("[Twitch] <b>%1</b> %2").arg(from, message));
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_buttonConnect_clicked()
{
    ui->buttonConnect->setEnabled(false);

    mTwitchBot = new TwitchBot(mTwitch);
    connect(mTwitchBot, SIGNAL(addLogMessage(QString)), this, SLOT(addLogMessage(QString)));
    connect(mTwitchBot, SIGNAL(connected()), this, SLOT(twitchConnected()));
    connect(mTwitchBot, SIGNAL(disconnected()), this, SLOT(twitchDisconnected()));
    connect(mTwitchBot, SIGNAL(messageReceived(QString,QString)), this, SLOT(twitchMessageReceived(QString,QString)));
    mTwitchBot->Connect();
}
