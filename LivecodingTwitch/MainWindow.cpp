#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QScrollBar>
#include <QMessageBox>

MainWindow::MainWindow(const ChatConfig & livecoding, const ChatConfig & twitch, QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      mLivecodingBot(new LivecodingBot(livecoding, this)),
      mTwitchBot(new TwitchBot(twitch, this)),
      mCanClose(false),
      mCloseThread(new MainWindowCloseThread)
{
    ui->setupUi(this);
    connect(mCloseThread, SIGNAL(canClose()), this, SLOT(canClose()));

    connect(mLivecodingBot, SIGNAL(logMessage(QString)), this, SLOT(addLogMessage(QString)));
    connect(mLivecodingBot, SIGNAL(connected()), this, SLOT(livecodingConnected()));
    connect(mLivecodingBot, SIGNAL(disconnected()), this, SLOT(livecodingDisconnected()));
    connect(mLivecodingBot, SIGNAL(messageReceived(QString,QString)), this, SLOT(livecodingMessageReceived(QString,QString)));

    connect(mTwitchBot, SIGNAL(logMessage(QString)), this, SLOT(addLogMessage(QString)));
    connect(mTwitchBot, SIGNAL(connected()), this, SLOT(twitchConnected()));
    connect(mTwitchBot, SIGNAL(disconnected()), this, SLOT(twitchDisconnected()));
    connect(mTwitchBot, SIGNAL(messageReceived(QString,QString)), this, SLOT(twitchMessageReceived(QString,QString)));
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
    addLogMessage("[Livecoding] Connected!");
    mSyncer.setLivecodingBot(mLivecodingBot);
    ui->buttonConnectLivecoding->setEnabled(true);
    ui->buttonConnectLivecoding->setText("Disconnect from Livecoding");
}

void MainWindow::livecodingDisconnected()
{
    addLogMessage("[Livecoding] Disconnected...");
    mSyncer.setLivecodingBot(nullptr);
    ui->buttonConnectLivecoding->setEnabled(true);
    ui->buttonConnectLivecoding->setText("Connect to Livecoding");
}

void MainWindow::livecodingMessageReceived(const QString & from, const QString & message)
{
    addLogMessage(QString("[Livecoding] <b>%1</b> %2").arg(from, message));
}

void MainWindow::twitchConnected()
{
    addLogMessage("[Twitch] Connected!");
    mSyncer.setTwitchBot(mTwitchBot);
    ui->buttonConnectTwitch->setEnabled(true);
    ui->buttonConnectTwitch->setText("Disconnect from Twitch");
}

void MainWindow::twitchDisconnected()
{
    addLogMessage("[Twitch] Disconnected...");
    mSyncer.setTwitchBot(nullptr);
    ui->buttonConnectTwitch->setEnabled(true);
    ui->buttonConnectTwitch->setText("Connect to Twitch");
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
    handleConnectDisconnect(mLivecodingBot, ui->buttonConnectLivecoding);
}

void MainWindow::on_buttonConnectTwitch_clicked()
{
    handleConnectDisconnect(mTwitchBot, ui->buttonConnectTwitch);
}

void MainWindow::on_buttonSend_clicked()
{
    QString message = ui->editMessage->text();
    if(!message.length())
        return;
    addLogMessage(QString("<b>me</b> %1").arg(EscapeHtml(message)));
    ui->editMessage->clear();
    if(mLivecodingBot)
        mLivecodingBot->SendMessage(message);
    if(mTwitchBot)
        mTwitchBot->SendMessage(message);
}

void MainWindow::on_actionAbout_triggered()
{
    QString about = "LivecodingTwitch v1.0<br><br>Coded by <a href=\"http://mrexodia.cf\">mrexodia</a>.<br>Licensed under <a href=\"https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html\">GPLv2</a>.<br>Code on <a href=\"https://github.com/mrexodia/LivecodingTwitch\">GitHub</a>.<br>Icon by <a href=\"https://icons8.com\">Icons8</a>.";
    QMessageBox::information(this, "About", about);
}

void MainWindow::handleConnectDisconnect(ChatBot* bot, QPushButton* button)
{
    button->setEnabled(false);

    if(bot->IsConnected())
        bot->Disconnect();
    else
    {
        if(bot->CheckConfig())
            bot->Connect();
        else
            addLogMessage(QString("<font color=\"red\">Invalid configuration for <b>%1</b>...</font>").arg(bot->Name()));
    }
}
