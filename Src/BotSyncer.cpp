#include "BotSyncer.h"

BotSyncer::BotSyncer(QObject* parent)
    : QObject(parent),
      mLivecodingBot(nullptr),
      mTwitchBot(nullptr)
{
}

void BotSyncer::setLivecodingBot(LivecodingBot* bot)
{
    if(!bot)
        return;
    mLivecodingBot = bot;
    connect(mLivecodingBot, SIGNAL(messageReceived(QString,QString)), this, SLOT(livecodingMessageReceived(QString,QString)));
}

void BotSyncer::setTwitchBot(TwitchBot* bot)
{
    if(!bot)
        return;
    mTwitchBot = bot;
    connect(mTwitchBot, SIGNAL(messageReceived(QString,QString)), this, SLOT(twitchMessageReceived(QString,QString)));
}

void BotSyncer::livecodingMessageReceived(const QString & from, const QString & message)
{
    if(!mTwitchBot)
        return;
    mTwitchBot->SendMessage(QString("[Livecoding] <%1> %2").arg(from, message));
}

void BotSyncer::twitchMessageReceived(const QString & from, const QString & message)
{
    if(!mLivecodingBot)
        return;
    mLivecodingBot->SendMessage(QString("[Twitch] <%1> %2").arg(from, message));
}
