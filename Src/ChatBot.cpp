#include "ChatBot.h"
#include <QMessageBox>

ChatBot::ChatBot(const ChatConfig & config, QObject* parent)
    : QObject(parent),
      mConfig(config)
{
}

void ChatBot::addLogMessage(const QString & message)
{
    emit logMessage(message);
}
