#ifndef TWITCHBOT_H
#define TWITCHBOT_H

#include "ChatBot.h"
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

signals:

private slots:
    void processIrcMessage(IrcMessage* message);

private:
    IrcConnection mConnection;
};

#endif // TWITCHBOT_H
