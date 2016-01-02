#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "TwitchBot.h"
#include "LivecodingBot.h"
#include "BotSyncer.h"
#include "MainWindowCloseThread.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ChatConfig livecoding, ChatConfig twitch, QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void addLogMessage(const QString & message);
    void livecodingConnected();
    void livecodingDisconnected();
    void livecodingMessageReceived(const QString & from, const QString & message);
    void twitchConnected();
    void twitchDisconnected();
    void twitchMessageReceived(const QString & from, const QString & message);
    void canClose();

    void on_actionExit_triggered();
    void on_buttonConnectLivecoding_clicked();
    void on_buttonConnectTwitch_clicked();
    void on_buttonSend_clicked();

private:
    Ui::MainWindow *ui;
    ChatConfig mLivecoding;
    LivecodingBot* mLivecodingBot;
    ChatConfig mTwitch;
    TwitchBot* mTwitchBot;
    BotSyncer mSyncer;
    bool mCanClose;
    MainWindowCloseThread* mCloseThread;
};

#endif // MAINWINDOW_H
