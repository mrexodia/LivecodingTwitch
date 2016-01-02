#include "LivecodingBot.h"
#include <QXmppLogger.h>
#include <QXmppMucManager.h>
#include <QtGui/qtextdocument.h>

LivecodingBot::LivecodingBot(const ChatConfig & config, QObject* parent)
    : ChatBot(config, parent)
{
    connect(&mClient, SIGNAL(connected()), this, SIGNAL(connected()));
    connect(&mClient, SIGNAL(connected()), this, SLOT(handleXmppConnect()));
    connect(&mClient, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    connect(&mClient, SIGNAL(presenceReceived(QXmppPresence)), this, SLOT(handleXmppPresence(QXmppPresence)));
    connect(&mClient, SIGNAL(error(QXmppClient::Error)), this, SLOT(handleXmppError(QXmppClient::Error)));
    mClient.logger()->setLoggingType(QXmppLogger::StdoutLogging);

    mMucManager = new QXmppMucManager;
    mClient.addExtension(mMucManager);
}

void LivecodingBot::Connect()
{
    QString jid = QString("%1@%2").arg(mConfig.user, mConfig.server);
    addLogMessage(QString("[Livecoding] Connecting to <i>%1</i> as <i>%2</i>...").arg(mConfig.server, jid));
    mClient.connectToServer(jid, mConfig.pass);
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
    if(!message.length())
        return;
    if(!IsConnected())
    {
        addLogMessage("[Livecoding] <font color=\"red\">Not connected...</font>");
        return;
    }
    mLastSent = message;
    mRoom->sendMessage(message);
}

QString LivecodingBot::Name()
{
    return "Livecoding";
}

void LivecodingBot::handleXmppPresence(QXmppPresence presence)
{
#ifdef _DEBUG
    addLogMessage(QString("[Livecoding] Got presence from %1").arg(presence.from()));
#endif //_DEBUG
}

void LivecodingBot::handleXmppMessage(QXmppMessage message)
{
    QString jid = message.from();
    int idx = jid.lastIndexOf("/");
    if(idx == -1)
        return;
    QString username = jid.right(jid.length() - idx - 1);
    QString body = message.body();
    if(body != mLastSent) //don't repeat messages sent by this client
        messageReceived(username, EscapeHtml(body));
    mLastSent = "";
}

void LivecodingBot::handleXmppConnect()
{
    mRoom = mMucManager->addRoom(mConfig.channel);
    connect(mRoom, SIGNAL(messageReceived(QXmppMessage)), this, SLOT(handleXmppMessage(QXmppMessage)));
    mRoom->setNickName(mConfig.user);
    mRoom->join();
    SendMessage(mConfig.welcome);
}

void LivecodingBot::handleXmppError(QXmppClient::Error error)
{
    static const char* errors[] =
    {
        "No error",
        "Error with TCP socket.",
        "Error, no response to a keep alive.",
        "Error in XML stream."
    };
    QString message = errors[error];
    if(error == QXmppClient::XmppStreamError && mClient.xmppStreamError() == QXmppStanza::Error::NotAuthorized)
            message = "Authentication failure!";
    addLogMessage(QString("[Livecoding] <font color=\"red\">%1</font>").arg(message));
}
