#include "LivecodingBot.h"
#include <QXmppLogger.h>
#include <QXmppMucManager.h>

LivecodingBot::LivecodingBot(const ChatConfig & config, QObject* parent)
    : ChatBot(config, parent)
{
    connect(&mClient, SIGNAL(connected()), this, SIGNAL(connected()));
    connect(&mClient, SIGNAL(connected()), this, SLOT(handleXmppConnect()));
    connect(&mClient, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    connect(&mClient, SIGNAL(presenceReceived(QXmppPresence)), this, SLOT(handleXmppPresence(QXmppPresence)));
    mClient.logger()->setLoggingType(QXmppLogger::StdoutLogging);

    mMucManager = new QXmppMucManager;
    mClient.addExtension(mMucManager);
}

void LivecodingBot::Connect()
{
    mClient.connectToServer(QString("%1@%2").arg(mConfig.user, mConfig.server), mConfig.pass);
}

void LivecodingBot::Disconnect()
{
    mClient.disconnectFromServer();
}

bool LivecodingBot::IsConnected()
{
    return mClient.isConnected();
}

void LivecodingBot::SendMessage(const QString & message)
{
    mRoom->sendMessage(message);
}

void LivecodingBot::handleXmppPresence(QXmppPresence presence)
{
    addLogMessage(QString("[Livecoding] Got presence from %1").arg(presence.from()));
}

void LivecodingBot::handleXmppMessage(QXmppMessage message)
{
    QString jid = message.from();
    int idx = jid.lastIndexOf("/");
    if(idx == -1)
        return;
    QString username = jid.right(jid.length() - idx - 1);
    if(username != mConfig.user)
        messageReceived(username, message.body());
}

void LivecodingBot::handleXmppConnect()
{
    mRoom = mMucManager->addRoom(mConfig.channel);
    connect(mRoom, SIGNAL(messageReceived(QXmppMessage)), this, SLOT(handleXmppMessage(QXmppMessage)));
    mRoom->setNickName(mConfig.user);
    mRoom->join();
    SendMessage(mConfig.welcome);
}
