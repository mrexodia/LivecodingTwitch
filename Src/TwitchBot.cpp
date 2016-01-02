#include "TwitchBot.h"
#include <IrcBuffer>
#include <QMessageBox>

TwitchBot::TwitchBot(const ChatConfig & config, QObject* parent)
    : ChatBot(config, parent),
      mConnection(config.server, this),
      mMessagesSent(0),
      mMessageLimit(20),
      mLimitTimer(new QTimer(this))
{
    if(!mConfig.channel.startsWith("#"))
        mConfig.channel.prepend('#');
    connect(&mConnection, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    connect(&mConnection, SIGNAL(connected()), this, SIGNAL(connected()));
    connect(&mConnection, SIGNAL(messageReceived(IrcMessage*)), this, SLOT(processIrcMessage(IrcMessage*)));

    //http://help.twitch.tv/customer/portal/articles/1302780-twitch-irc
    connect(mLimitTimer, SIGNAL(timeout()), this, SLOT(resetLimit()));
    mLimitTimer->start(29 * 1000);
}

void TwitchBot::Connect()
{
    mConnection.setUserName(mConfig.user);
    mConnection.setNickName(mConfig.user);
    mConnection.setRealName(mConfig.user);
    mConnection.setPassword(mConfig.pass);

    mConnection.sendCommand(IrcCommand::createJoin(mConfig.channel));
    SendMessage(mConfig.welcome);

    mConnection.open();
}

void TwitchBot::Disconnect()
{
    mConnection.close();
}

bool TwitchBot::IsConnected()
{
    return mConnection.isConnected();
}

void TwitchBot::SendMessage(const QString & message)
{
    if(!message.length())
        return;
    if(mMessagesSent >= mMessageLimit)
    {
        mMessageQueue.append(message);
#ifdef _DEBUG
        addLogMessage(QString("[Twitch] Queued message %1").arg(message));
#endif //_DEBUG
    }
    mMessagesSent++;
    mConnection.sendCommand(IrcCommand::createMessage(mConfig.channel, message));
}

void TwitchBot::processIrcMessage(IrcMessage* message)
{
    switch(message->type())
    {
    case IrcMessage::Capability:
    {
        auto cap = (IrcCapabilityMessage*)message;
        auto caps = cap->capabilities();
        for(int i=0;i<caps.length();i++)
        {
            if(!i)
                addLogMessage("[Twitch] Capabilities: " + caps[i]);
            else
                addLogMessage("                       " + caps[i]);
        }
    }
    break;

    case IrcMessage::Names:
    {
#ifdef _DEBUG
        auto nam = (IrcNamesMessage*)message;
        auto names = nam->names();
        for(int i=0;i<names.length();i++)
        {
            if(!i)
                addLogMessage("[Twitch] Names: " + names[i]);
            else
                addLogMessage("                " + names[i]);
        }
#endif //_DEBUG
    }
    break;

    case IrcMessage::Motd:
    {
        auto motd = (IrcMotdMessage*)message;
        auto lines = motd->lines();
        for(int i=0; i<lines.length(); i++)
        {
            if(!i)
                addLogMessage("[Twitch] MOTD: " + lines[i]);
            else
                addLogMessage("               " + lines[i]);
        }
    }
    break;

    case IrcMessage::Join:
    {
        auto join = (IrcJoinMessage*)message;
        addLogMessage(QString("[Twitch] Joined %1 as %2").arg(join->channel(), join->nick()));
    }
    break;

    case IrcMessage::Private:
    {
        auto priv = (IrcPrivateMessage*)message;
        messageReceived(priv->nick(), priv->content());
    }
    break;

    case IrcMessage::Numeric:
    {
#ifdef _DEBUG
        auto num = (IrcNumericMessage*)message;
        addLogMessage(QString("[Twitch] Code: %1").arg(num->code()));
#endif //_DEBUG
    }
    break;

    default:
    {
        static const char* types[] =
        {
            "Unknown",
            "Capability",
            "Error",
            "Invite",
            "Join",
            "Kick",
            "Mode",
            "Motd",
            "Names",
            "Nick",
            "Notice",
            "Numeric",
            "Part",
            "Ping",
            "Pong",
            "Private",
            "Quit",
            "Topic",
            "WhoReply",
            "Account",
            "Away",
            "Whois",
            "Whowas",
            "HostChange",
            "Batch"
        };
        addLogMessage(QString("[Twitch] Unimplemented IrcMessage::type() = %1").arg(types[message->type()]));
    }
    break;
    }
}

void TwitchBot::resetLimit()
{
    mMessagesSent = 0;
    auto queue = mMessageQueue;
    mMessageQueue.clear();
    for(auto message : queue)
    {
        SendMessage(message);
#ifdef _DEBUG
        addLogMessage(QString("[Twitch] Sent queued message %1").arg(message));
#endif //_DEBUG
    }
}
