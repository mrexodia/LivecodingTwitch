#ifndef TWITCHBOT_H
#define TWITCHBOT_H

#include "ChatBot.h"
#include <QTimer>
#include <QMutex>
#include <IrcConnection>
#include <IrcCommand>
#include <Irc>

class TwitchBot : public ChatBot
{
    Q_OBJECT
public:
    explicit TwitchBot(const ChatConfig & config, QObject* parent = 0);
    void Connect() override;
    void Disconnect() override;
    bool IsConnected() override;
    void SendMessage(const QString & message) override;
    QString Name() override;

signals:

private slots:
    void processIrcMessage(IrcMessage* message);
    void resetLimit();

private:
    IrcConnection mConnection;
    int mMessagesSent;
    int mMessageLimit;
    QList<QString> mMessageQueue;
    QTimer* mLimitTimer;
};

#endif // TWITCHBOT_H
