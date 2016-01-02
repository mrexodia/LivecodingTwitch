#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QScrollBar>

MainWindow::MainWindow(ChatConfig livecoding, ChatConfig twitch, QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      mLivecoding(livecoding),
      mLivecodingBot(nullptr),
      mTwitch(twitch),
      mTwitchBot(nullptr),
      mCanClose(false),
      mCloseThread(new MainWindowCloseThread)
{
    ui->setupUi(this);
    connect(mCloseThread, SIGNAL(canClose()), this, SLOT(canClose()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    static volatile bool bExecuteThread = true;
    if(bExecuteThread)
    {
        bExecuteThread = false;
        addLogMessage("Closing connections...");
        mCloseThread->addBot(mLivecodingBot);
        mCloseThread->addBot(mTwitchBot);
        mCloseThread->start();
    }
    if(mCanClose)
        event->accept();
    else
        event->ignore();
}

void MainWindow::addLogMessage(const QString & message)
{
    ui->textLog->append(QString("[%1] %2").arg(QTime::currentTime().toString(Qt::SystemLocaleShortDate), message));
    ui->textLog->verticalScrollBar()->setValue(ui->textLog->verticalScrollBar()->maximum());
}

void MainWindow::livecodingConnected()
{
    mSyncer.setLivecodingBot(mLivecodingBot);
    addLogMessage("[Livecoding] Connected!");
}

void MainWindow::livecodingDisconnected()
{
    mSyncer.setLivecodingBot(nullptr);
    addLogMessage("[Livecoding] Disconnected...");
}

void MainWindow::livecodingMessageReceived(const QString & from, const QString & message)
{
    addLogMessage(QString("[Livecoding] <b>%1</b> %2").arg(from, message));
}

void MainWindow::twitchConnected()
{
    mSyncer.setTwitchBot(mTwitchBot);
    addLogMessage("[Twitch] Connected!");
}

void MainWindow::twitchDisconnected()
{
    mSyncer.setTwitchBot(nullptr);
    addLogMessage("[Twitch] Disconnected...");
}

void MainWindow::twitchMessageReceived(const QString & from, const QString & message)
{
    addLogMessage(QString("[Twitch] <b>%1</b> %2").arg(from, message));
}

void MainWindow::canClose()
{
    mCanClose = true;
    close();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_buttonConnectLivecoding_clicked()
{
    ui->buttonConnectLivecoding->setEnabled(false);

    mLivecodingBot = new LivecodingBot(mLivecoding, this);
    connect(mLivecodingBot, SIGNAL(addLogMessage(QString)), this, SLOT(addLogMessage(QString)));
    connect(mLivecodingBot, SIGNAL(connected()), this, SLOT(livecodingConnected()));
    connect(mLivecodingBot, SIGNAL(disconnected()), this, SLOT(livecodingDisconnected()));
    connect(mLivecodingBot, SIGNAL(messageReceived(QString,QString)), this, SLOT(livecodingMessageReceived(QString,QString)));
    mLivecodingBot->Connect();
}

void MainWindow::on_buttonConnectTwitch_clicked()
{
    ui->buttonConnectTwitch->setEnabled(false);

    mTwitchBot = new TwitchBot(mTwitch, this);
    connect(mTwitchBot, SIGNAL(addLogMessage(QString)), this, SLOT(addLogMessage(QString)));
    connect(mTwitchBot, SIGNAL(connected()), this, SLOT(twitchConnected()));
    connect(mTwitchBot, SIGNAL(disconnected()), this, SLOT(twitchDisconnected()));
    connect(mTwitchBot, SIGNAL(messageReceived(QString,QString)), this, SLOT(twitchMessageReceived(QString,QString)));
    mTwitchBot->Connect();
}

void MainWindow::on_buttonSend_clicked()
{
    QString message = ui->editMessage->text();
    ui->editMessage->clear();
    if(mLivecodingBot)
        mLivecodingBot->SendMessage(message);
    if(mTwitchBot)
        mTwitchBot->SendMessage(message);
    addLogMessage(QString("<b>me</b> %1").arg(message));
}
