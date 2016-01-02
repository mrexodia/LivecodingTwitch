#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Twitch/TwitchBot.h"

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

private slots:
    void addLogMessage(const QString & message);
    void twitchConnected();
    void twitchDisconnected();
    void twitchMessageReceived(const QString & from, const QString & message);

    void on_actionExit_triggered();
    void on_buttonConnect_clicked();

private:
    Ui::MainWindow *ui;
    ChatConfig mLivecoding;
    ChatConfig mTwitch;
    TwitchBot* mTwitchBot;
};

#endif // MAINWINDOW_H
