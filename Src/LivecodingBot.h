#ifndef LIVECODINGBOT_H
#define LIVECODINGBOT_H

#include "ChatBot.h"
#include <QXmppClient.h>
#include <QXmppMucManager.h>
#include <QXmppMessage.h>
#include <QXmppPresence.h>

class LivecodingBot : public ChatBot
{
    Q_OBJECT
public:
    explicit LivecodingBot(const ChatConfig & config, QObject* parent = 0);
    void Connect() override;
    void Disconnect() override;
    bool IsConnected() override;
    void SendMessage(const QString & message) override;
    QString Name() override;

private slots:
    void handleXmppPresence(QXmppPresence room);
    void handleXmppMessage(QXmppMessage message);
    void handleXmppConnect();
    void handleXmppError(QXmppClient::Error error);

private:
    QXmppMucManager* mMucManager;
    QXmppMucRoom* mRoom;
    QXmppClient mClient;
};

#endif // LIVECODINGBOT_H
