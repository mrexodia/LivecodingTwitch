#ifndef BOTSYNCER_H
#define BOTSYNCER_H

#include <QObject>
#include "LivecodingBot.h"
#include "TwitchBot.h"

class BotSyncer : public QObject
{
    Q_OBJECT
public:
    explicit BotSyncer(QObject* parent = 0);
    void setLivecodingBot(LivecodingBot* bot);
    void setTwitchBot(TwitchBot* bot);

private slots:
    void livecodingMessageReceived(const QString & from, const QString & message);
    void twitchMessageReceived(const QString & from, const QString & message);

private:
    LivecodingBot* mLivecodingBot;
    TwitchBot* mTwitchBot;
};

#endif // BOTSYNCER_H
