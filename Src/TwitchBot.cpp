#include "TwitchBot.h"
#include <IrcBuffer>

TwitchBot::TwitchBot(const ChatConfig & config, QObject* parent)
    : ChatBot(config, parent),
      mConnection(config.server, this)
{
    if(!mConfig.channel.startsWith("#"))
        mConfig.channel.prepend('#');
    connect(&mConnection, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    connect(&mConnection, SIGNAL(connected()), this, SIGNAL(connected()));
    connect(&mConnection, SIGNAL(messageReceived(IrcMessage*)), this, SLOT(processIrcMessage(IrcMessage*)));
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
#ifdef DEBUG
        auto nam = (IrcNamesMessage*)message;
        auto names = nam->names();
        for(int i=0;i<names.length();i++)
        {
            if(!i)
                addLogMessage("[Twitch] Names: " + names[i]);
            else
                addLogMessage("                " + names[i]);
        }
#endif //DEBUG
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
#ifdef DEBUG
        auto num = (IrcNumericMessage*)message;
        addLogMessage(QString("[Twitch] Code: %1").arg(num->code()));
#endif //DEBUG
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
